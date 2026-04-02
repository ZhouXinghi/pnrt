#ifndef PNRT_TENSOR_H
#define PNRT_TENSOR_H

#include <armadillo>
#include <cstdint>
#include <vector>

namespace pnrt {

template <class T>
class Tensor {};

template <>
class Tensor<float> {
 public:
  explicit Tensor() = default;
  explicit Tensor(uint32_t c, uint32_t h, uint32_t w) : cube_(h, w, c) {}

  bool empty() const { return cube_.empty(); };
  uint32_t c() const { return cube_.n_slices; }
  uint32_t h() const { return cube_.n_rows; }
  uint32_t w() const { return cube_.n_cols; }
  uint32_t size() const { return cube_.size(); }
  const arma::fcube& data() const { return cube_; }
  arma::fcube& data() { return cube_; }
  float at(uint32_t c, uint32_t h, uint32_t w) const { return cube_(h, w, c); }
  float& at(uint32_t c, uint32_t h, uint32_t w) { return cube_(h, w, c); }
  const arma::fmat& slice(uint32_t c) const { return cube_.slice(c); }
  arma::fmat& slice(uint32_t c) { return cube_.slice(c); }

  std::vector<float> Values(bool row_major = false) const;

  void Fill(float value) { cube_.fill(value); }
  void Fill(const std::vector<float>& values, bool row_major = false);
  void Reshape(uint32_t c, uint32_t h, uint32_t w, bool row_major = false);
  void Flatten(bool row_major = false) { Reshape(1, 1, size(), row_major); }

 private:
  arma::fcube cube_;
};

}  // namespace pnrt

#endif  // PNRT_TENSOR_H