#include <Tensor.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

namespace pnrt {
namespace test {

void PrintTensor(const Tensor<float>& tensor) {
  LOG(INFO) << '\n' << tensor.data();
}

void PrintVector(const std::vector<float>& values) {
  std::string str;
  for (float value : values) {
    str += std::to_string(value) + " ";
  }
  LOG(INFO) << str;
}

TEST(TensorTest, Create1DTensor) {
  Tensor<float> tensor(1, 1, 4);
  tensor.Fill(1.0f);
  EXPECT_EQ(tensor.c(), 1);
  EXPECT_EQ(tensor.h(), 1);
  EXPECT_EQ(tensor.w(), 4);
  EXPECT_EQ(tensor.size(), 4);
  PrintTensor(tensor);
}

TEST(TensorTest, Create2DTensor) {
  Tensor<float> tensor(1, 3, 4);
  tensor.Fill(1.0f);
  EXPECT_EQ(tensor.c(), 1);
  EXPECT_EQ(tensor.h(), 3);
  EXPECT_EQ(tensor.w(), 4);
  EXPECT_EQ(tensor.size(), 12);
  PrintTensor(tensor);
}

TEST(TensorTest, Create3DTensor) {
  Tensor<float> tensor(2, 3, 4);
  tensor.Fill(1.0f);
  EXPECT_EQ(tensor.c(), 2);
  EXPECT_EQ(tensor.h(), 3);
  EXPECT_EQ(tensor.w(), 4);
  EXPECT_EQ(tensor.size(), 24);
  PrintTensor(tensor);
}

TEST(TensorTest, Values) {
  Tensor<float> tensor(2, 3, 4);
  std::vector<float> values(tensor.size());
  for (size_t i = 0; i < values.size(); ++i) {
    values[i] = static_cast<float>(i);
  }
  tensor.Fill(values);
  PrintTensor(tensor);
  PrintVector(tensor.Values());

  EXPECT_TRUE(tensor.Values() == values);
  tensor.Fill(values, true);
  PrintTensor(tensor);
  PrintVector(tensor.Values(true));
  EXPECT_TRUE(tensor.Values(true) == values);
}

TEST(TensorTest, Fill) {
  Tensor<float> tensor(2, 3, 4);
  std::vector<float> values(tensor.size());
  for (size_t i = 0; i < values.size(); ++i) {
    values[i] = static_cast<float>(i);
  }
  tensor.Fill(values);
  PrintTensor(tensor);
  tensor.Fill(values, true);
  PrintTensor(tensor);
}

TEST(TensorTest, Reshape) {
  Tensor<float> tensor(2, 3, 4);
  std::vector<float> values(tensor.size());
  for (size_t i = 0; i < values.size(); ++i) {
    values[i] = static_cast<float>(i);
  }
  tensor.Fill(values);
  LOG(INFO) << "Before reshape:";
  PrintTensor(tensor);
  tensor.Reshape(4, 3, 2);
  EXPECT_EQ(tensor.c(), 4);
  EXPECT_EQ(tensor.h(), 3);
  EXPECT_EQ(tensor.w(), 2);
  EXPECT_EQ(tensor.size(), 24);
  LOG(INFO) << "After reshape:";
  PrintTensor(tensor);
}

}  // namespace test
}  // namespace pnrt
