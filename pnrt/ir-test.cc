#include <glog/logging.h>
#include <gtest/gtest.h>
#include <ir.h>

#include <cstdlib>

namespace pnrt {
namespace test {

class GraphTest : public ::testing::Test {
 protected:
  static void SetUpTestSuite() {}

  void SetUp() override {
    const char* param_path = std::getenv("PARAM_PATH");
    const char* bin_path = std::getenv("BIN_PATH");
    ASSERT_TRUE(param_path != nullptr)
        << "PARAM_PATH environment variable is not set";
    ASSERT_TRUE(bin_path != nullptr)
        << "BIN_PATH environment variable is not set";
    ASSERT_TRUE(graph_.Load(param_path, bin_path));
  }

  void TearDown() override {}

  Graph graph_;
};

TEST_F(GraphTest, Load) {
  const auto& ops = graph_.ops;
  LOG(INFO) << "-----------------------------------";
  for (const auto& [op_name, op] : ops) {
    LOG(INFO) << "op name: " << op_name;
    LOG(INFO) << "op type: " << op->type;
    for (const auto& [internal_input_name, input_name] :
         op->internal_input_name_to_operand_input_name) {
      LOG(INFO) << "internal input name: " << internal_input_name << " --> "
                << "operand input name: " << input_name;
    }
    LOG(INFO) << "op predecessors: ";
    for (Operator* predecessor : op->predecessors) {
      LOG(INFO) << "\t" << predecessor->name << " ";
    }
    LOG(INFO) << "op successors: ";
    for (Operator* successor : op->successors) {
      LOG(INFO) << "\t" << successor->name << " ";
    }

    LOG(INFO) << "op inputs: ";
    for (const auto& [input_name, operand] : op->inputs) {
      LOG(INFO) << "\t" << input_name << " " << ShapeToString(operand->shape)
                << " " << DataTypeToString(operand->data_type);
    }
    LOG(INFO) << "op outputs: ";
    for (const auto& [output_name, operand] : op->outputs) {
      LOG(INFO) << "\t" << output_name << " " << ShapeToString(operand->shape)
                << " " << DataTypeToString(operand->data_type);
    }

    LOG(INFO) << "op attributes: ";
    for (const auto& [attr_name, attr] : op->attributes) {
      LOG(INFO) << "\t" << attr_name << " = " << attr.value;
    }

    LOG(INFO) << "op weights: ";
    for (const auto& [weight_name, weight] : op->weights) {
      LOG(INFO) << "\t" << weight_name << ": " << ShapeToString(weight.shape)
                << " " << DataTypeToString(weight.data_type);
    }
    LOG(INFO) << "-----------------------------------";
  }
}

TEST_F(GraphTest, TopoSort) {
  ASSERT_TRUE(graph_.TopoSort());
  const auto& topo_order = graph_.ops_topo_order;
  LOG(INFO) << "Topo order of operators:";
  for (Operator* op : topo_order) {
    LOG(INFO) << op->name;
  }
}

TEST_F(GraphTest, InitOperandSpace) {
  graph_.InitOperandSpace();
  for (const auto& [name, operand] : graph_.operands) {
    EXPECT_TRUE(!operand->tensors.empty());
  }
}

}  // namespace test
}  // namespace pnrt