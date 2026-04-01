#include <Tensor.h>
#include <logging.h>

namespace pnrt {

std::vector<float> Tensor<float>::Values(bool row_major) const {
  std::vector<float> values(size());
  if (!row_major) {
    // If the caller wants column-major style, we can directly copy the data
    // from the cube's memory.
    std::copy(cube_.memptr(), cube_.memptr() + size(), values.begin());
  } else {
    // If the caller wants row-major style, we need to rearrange the data from
    // column-major to row-major.
    for (auto i = 0; i < c(); ++i) {
      arma::fmat slice_t = cube_.slice(i).t();
      std::copy(slice_t.begin(), slice_t.end(), values.begin() + i * h() * w());
    }
  }
  return values;
}

void Tensor<float>::Fill(const std::vector<float>& values, bool row_major) {
  PNRT_CHECK(values.size() == size());
  if (!row_major) {
    // If values are already in column-major style, we can directly copy them
    // into the cube's memory.
    std::copy(values.begin(), values.end(), cube_.memptr());
    return;
  }
  // If values are in row-major style, we need to rearrange them to match the
  // column-major style of Armadillo.
  for (auto i = 0; i < c(); ++i) {
    // Wrap the raw pointer directly without copying (copy_aux_mem=false).
    // Treating row-major h×w data as column-major w×h is equivalent,
    // so transposing gives the correct h×w column-major slice.
    // strict = true means the fmat is not allowed to be modified. Operations
    // like resize or set_size() will throw an error. This is to prevent
    // accidental modification of the original data.
    const arma::fmat slice_view(
        const_cast<float*>(values.data()) + i * h() * w(), w(), h(), false,
        true);
    cube_.slice(i) = slice_view.t();
  }
}

void Tensor<float>::Reshape(uint32_t c, uint32_t h, uint32_t w,
                            bool row_major) {
  PNRT_CHECK(c * h * w == size());
  if (!row_major) {
    cube_.reshape(h, w, c);
    return;
  }
}
}  // namespace pnrt
