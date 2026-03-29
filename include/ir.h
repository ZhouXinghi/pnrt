#ifndef PNRT_IR_H
#define PNRT_IR_H

#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

namespace pnrt {

enum class DataType {
  kMin,
  kFloat32,
  kMax,
};

inline size_t SizeOfDataType(DataType data_type) {
  assert(data_type > DataType::kMin && data_type < DataType::kMax);
  switch (data_type) {
    case DataType::kFloat32:
      return 4;
    default:
      return 0;  // should never reach here
  }
}

inline const char* DataTypeToString(DataType data_type) {
  assert(data_type > DataType::kMin && data_type < DataType::kMax);
  switch (data_type) {
    case DataType::kFloat32:
      return "Float32";
    default: {
      return "Unsupported";
    }
  }
}

inline std::string ShapeToString(const std::vector<int>& shape) {
  std::string result = "(";
  for (size_t i = 0; i < shape.size(); ++i) {
    result += std::to_string(shape[i]);
    if (i != shape.size() - 1) {
      result += ",";
    }
  }
  result += ")";
  return result;
}

struct Attribute {
  std::string name;
  std::string value;
};

struct Weight {
  std::vector<int> shape;
  DataType data_type;
  std::vector<char> data;
};

struct Operator;
struct Operand {
  std::string name;
  std::vector<int> shape;
  DataType data_type;
  Operator* producer;
  std::vector<Operator*> consumers;
};

struct Operator {
  size_t InDegree() const noexcept { return predecessors.size(); }
  size_t OutDegree() const noexcept { return successors.size(); }

  std::string type;
  std::string name;
  std::unordered_map<std::string, std::string>
      internal_input_name_to_operand_input_name;
  std::unordered_map<std::string, Operand*> inputs;
  std::unordered_map<std::string, Operand*> outputs;
  std::unordered_map<std::string, Attribute> attributes;
  std::unordered_map<std::string, Weight> weights;

  std::vector<Operator*> predecessors;
  std::vector<Operator*> successors;
};

struct Graph {
  void clear() {
    ops.clear();
    operands.clear();
    ops_topo_order.clear();
  }
  bool Load(const std::string& param_path, const std::string& bin_path);
  void Save(const std::string& param_path, const std::string& bin_path) const;
  bool TopoSort();

  std::unordered_map<std::string, std::unique_ptr<Operator>> ops;
  std::unordered_map<std::string, std::unique_ptr<Operand>> operands;

  std::vector<Operator*> ops_topo_order;
};

}  // namespace pnrt

#endif  // PNRT_IR_H