#include <Tensor.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

namespace pnrt {
namespace test {

void PrintTensor(const Tensor<float>& tensor) {
  LOG(INFO) << '\n' << tensor.data();
  // for (uint32_t c = 0; c < tensor.c(); ++c) {
  //   LOG(INFO) << "Channel " << c << ":";
  //   LOG(INFO) << tensor.data().slice(c);
  //   // for (uint32_t h = 0; h < tensor.h(); ++h) {
  //   //   std::string row_str;
  //   //   for (uint32_t w = 0; w < tensor.w(); ++w) {
  //   //     row_str += std::to_string(tensor.at(c, h, w)) + " ";
  //   //   }
  //   //   LOG(INFO) << row_str;
  //   // }
  // }
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

TEST(TensorTest, ReshapeTensor) {
  Tensor<float> tensor(2, 3, 4);
  std::vector<float> values(tensor.size());
  for (size_t i = 0; i < values.size(); ++i) {
    values[i] = static_cast<float>(i);
  }
  tensor.Fill(values);
  tensor.Reshape(4, 3, 2);
  EXPECT_EQ(tensor.c(), 4);
  EXPECT_EQ(tensor.h(), 3);
  EXPECT_EQ(tensor.w(), 2);
  EXPECT_EQ(tensor.size(), 24);
  PrintTensor(tensor);
}

}  // namespace test
}  // namespace pnrt
