#include <gtest/gtest.h>
#include <armadillo>

TEST(ArmaTest, Add) {
  arma::vec a = {1, 2, 3};
  arma::vec b = {4, 5, 6};
  arma::vec c = a + b;
  EXPECT_EQ(c(0), 5);
  EXPECT_EQ(c(1), 7);
  EXPECT_EQ(c(2), 9);
}