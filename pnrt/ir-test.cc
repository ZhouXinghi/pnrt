#include <glog/logging.h>
#include <gtest/gtest.h>
#include <ir.h>

#include <cstdlib>

namespace pnrt {
namespace test {

TEST(GraphTest, Load) {
  const char* param_path = std::getenv("PARAM_PATH");
  const char* bin_path = std::getenv("BIN_PATH");
  Graph graph;
  ASSERT_TRUE(graph.Load(param_path, bin_path));

  const auto& ops = graph.ops;
  for (const auto& [op_name, op] : ops) {
    LOG(INFO) << "op name: " << op_name;
    LOG(INFO) << "op type: " << op->type;
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
      LOG(INFO) << "weight shape: ";
      for (int i : weight.shape) {
        LOG(INFO) << i << " ";
      }
      LOG(INFO) << "\n";
      LOG(INFO) << "weight data type: " << DataTypeToString(weight.data_type);
    }
  }
}

}  // namespace test

}  // namespace pnrt