#include <glog/logging.h>
#include <gtest/gtest.h>
#include <ir.h>

#include <cstdlib>

namespace pnrt {
namespace test {

class GraphTest : public ::testing::Test {
  void SetUp() override {
    const char* param_path = std::getenv("PARAM_PATH");
    const char* bin_path = std::getenv("BIN_PATH");
    ASSERT_TRUE(graph_.Load(param_path, bin_path));
  }

  void TearDown() override {}

 protected:
  Graph graph_;
};

TEST_F(GraphTest, Load) {
  const auto& ops = graph_.ops;
  LOG(INFO) << "-----------------------------------";
  for (const auto& [op_name, op] : ops) {
    LOG(INFO) << "op name: " << op_name;
    LOG(INFO) << "op type: " << op->type;
    LOG(INFO) << "op predecessors: ";
    for (Operator* predecessor : op->predecessors) {
      LOG(INFO) << predecessor->name << " ";
    }
    LOG(INFO) << "op successors: ";
    for (Operator* successor : op->successors) {
      LOG(INFO) << successor->name << " ";
    }
    for (const auto& [input_name, operand] : op->inputs) {
      LOG(INFO) << "input name: " << input_name;
      LOG(INFO) << "input operand name: " << operand->name;
      LOG(INFO) << "input operand shape: " << ShapeToString(operand->shape);
      LOG(INFO) << "\n";
      LOG(INFO) << "input operand data type: "
                << DataTypeToString(operand->data_type);
    }
    for (const auto& [output_name, operand] : op->outputs) {
      LOG(INFO) << "output name: " << output_name;
      LOG(INFO) << "output operand name: " << operand->name;
      LOG(INFO) << "output operand shape: " << ShapeToString(operand->shape);
      LOG(INFO) << "\n";
      LOG(INFO) << "output operand data type: "
                << DataTypeToString(operand->data_type);
    }
    for (const auto& [attr_name, attr] : op->attributes) {
      LOG(INFO) << "attribute name: " << attr_name;
      LOG(INFO) << "attribute value: " << attr.value;
    }
    for (const auto& [weight_name, weight] : op->weights) {
      LOG(INFO) << "weight name: " << weight_name;
      LOG(INFO) << "weight shape: " << ShapeToString(weight.shape);
      LOG(INFO) << "weight data type: " << DataTypeToString(weight.data_type);
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

}  // namespace test

}  // namespace pnrt