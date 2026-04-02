#include <ir.h>
#include <logging.h>
#include <store-zip.h>

#include <cassert>
#include <fstream>
#include <sstream>

namespace pnrt {

namespace {
void InitOpOutputSpace(const Operator& op) {
  const auto& operands = op.outputs;
  // We only support at most one output operand for each operator.
  PNRT_CHECK(operands.size() <= 1);
  if (operands.empty()) {
    // This operator has no output operand, so we don't need to initialize any
    // output space.
    return;
  }
  PNRT_CHECK(operands.size() == 1);
  const auto& [_, operand] = *operands.begin();
  if (!operand->tensors.empty()) {
    // The output space has already been initialized, so we don't need to do it
    // again.
    return;
  }
  const auto dims = operand->shape.size();
  CHECK(dims >= 2 && dims <= 4);
  const auto batch = operand->shape.at(0);
  CHECK(batch >= 1);
  CHECK(operand->data_type == DataType::kFloat32);
  for (auto i = 0; i < batch; ++i) {
    if (dims == 4) {
      operand->tensors.emplace_back(operand->shape[1], operand->shape[2],
                                    operand->shape[3]);
    } else if (dims == 3) {
      operand->tensors.emplace_back(1, operand->shape[1], operand->shape[2]);
    } else if (dims == 2) {
      operand->tensors.emplace_back(1, 1, operand->shape[1]);
    }
  }
}

}  // namespace

std::pair<std::vector<int>, DataType> ParseShapeAndTypeFromString(
    const std::string& value) {
  // shape
  std::vector<int> shape;
  std::string shape_str = value.substr(1, value.find_last_of(')') - 1);
  std::istringstream shape_iss(shape_str);
  while (!shape_iss.eof()) {
    std::string elem;
    std::getline(shape_iss, elem, ',');
    shape.push_back(std::stoi(elem));
  }
  // type
  std::string type_str = value.substr(value.find_last_of(')') + 1);
  assert(type_str == "f32");                // only support float32 for now
  DataType data_type = DataType::kFloat32;  // only support float32 for now

  return {shape, data_type};
}  // namespace pnrt

bool Graph::Load(const std::string& param_path, const std::string& bin_path) {
  clear();

  std::ifstream param_file(param_path);
  if (!param_file.good()) {
    PNRT_ERROR << "open param file failed: " << param_path;
    return false;
  }

  StoreZipReader szr;
  if (szr.open(bin_path) != 0) {
    LOG(ERROR) << "open bin file failed: " << bin_path;
    return false;
  }

  int magic = 0;
  {
    std::string line;
    std::getline(param_file, line);
    std::istringstream iss(line);
    iss >> magic;
  }

  int operator_count = 0;
  int operand_count = 0;
  {
    std::string line;
    std::getline(param_file, line);
    std::istringstream iss(line);
    iss >> operator_count >> operand_count;
  }

  for (auto i = 0; i < operator_count; ++i) {
    std::string line;
    std::getline(param_file, line);
    std::istringstream iss(line);
    auto op = std::make_unique<Operator>();
    int input_count = 0;
    int output_count = 0;
    iss >> op->type >> op->name >> input_count >> output_count;
    for (auto j = 0; j < input_count; ++j) {
      std::string operand_name;
      iss >> operand_name;
      // As an input operand, it must already exsist.
      Operand* operand = operands.at(operand_name).get();
      operand->consumers.push_back(op.get());
      operand->producer->successors.push_back(op.get());
      op->predecessors.push_back(operand->producer);
      op->inputs.insert({operand_name, operand});
    }
    for (auto j = 0; j < output_count; ++j) {
      std::string operand_name;
      iss >> operand_name;
      // As an output operand, it does no exsit yet, so we create a new operand.
      auto operand = std::make_unique<Operand>();
      operand->name = operand_name;
      operand->producer = op.get();
      op->outputs.insert({operand_name, operand.get()});
      operands.insert({operand_name, std::move(operand)});
    }

    // still have attributes or weights
    std::string attr_or_weight;
    while (iss >> attr_or_weight) {
      std::string key;
      std::string value;
      std::istringstream attr_or_weight_iss(attr_or_weight);
      std::getline(attr_or_weight_iss, key, '=');
      std::getline(attr_or_weight_iss, value);
      switch (key[0]) {
        case '@': {  // weight
          Weight weight;
          auto [shape, data_type] = ParseShapeAndTypeFromString(value);

          size_t num_elements = 1;
          for (int i : shape) {
            num_elements *= i;
          }
          size_t byte_size = num_elements * SizeOfDataType(data_type);

          std::string weight_name = op->name + "." + key.substr(1);
          assert(szr.get_file_size(weight_name) == byte_size);

          weight.shape = std::move(shape);
          weight.data_type = data_type;
          weight.data.resize(byte_size);
          szr.read_file(weight_name, (char*)weight.data.data());
          op->weights.insert({key.substr(1), weight});
          break;
        }
        case '#': {  // operand shape and type
          auto [shape, data_type] = ParseShapeAndTypeFromString(value);
          Operand* operand = operands.at(key.substr(1)).get();
          operand->shape = std::move(shape);
          operand->data_type = data_type;
          break;
        }
        case '$': {  // operator internal input name to operand name mapping
          op->internal_input_name_to_operand_input_name.insert(
              {key.substr(1), value});
          break;
        }
        default: {  // attribute
          Attribute attr = {.name = key, .value = value};
          op->attributes.insert({key, attr});
          break;
        }
      }
    }
    ops.insert({op->name, std::move(op)});
  }
  return true;
}

bool Graph::TopoSort() {
  ops_topo_order.clear();
  ops_topo_order.reserve(ops.size());

  // Compute in-degrees for each operator.
  std::unordered_map<Operator*, size_t> in_degree;
  in_degree.reserve(ops.size());
  for (const auto& [name, op] : ops) {
    in_degree[op.get()] = op->InDegree();
  }

  // Initialize queue with operators that have no predecessors.
  std::queue<Operator*> queue;
  for (const auto& [name, op] : ops) {
    if (in_degree[op.get()] == 0) {
      queue.push(op.get());
    }
  }

  while (!queue.empty()) {
    Operator* op = queue.front();
    queue.pop();
    ops_topo_order.push_back(op);

    for (Operator* successor : op->successors) {
      assert(in_degree[successor] > 0);
      if (--in_degree[successor] == 0) {
        queue.push(successor);
      }
    }
  }

  if (ops_topo_order.size() != ops.size()) {
    LOG(FATAL) << "TopoSort failed: graph has a cycle. Sorted "
               << ops_topo_order.size() << " out of " << ops.size()
               << " operators.";
    ops_topo_order.clear();
    return false;
  }

  return true;
}

void Graph::InitOperandSpace() {
  for (const auto& [_, op] : ops) {
    // We only need to initialize the output space for each operator, because
    // the input space should have already been initialized by its producer
    // operator.
    InitOpOutputSpace(*op);
  }
}

}  // namespace pnrt