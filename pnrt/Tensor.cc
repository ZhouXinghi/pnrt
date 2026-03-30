#include <Tensor.h>
#include <logging.h>

namespace pnrt {

void Tensor<float>::Fill(const std::vector<float>& values,
                         bool values_in_row_major_style) {
  PNRT_CHECK(values.size() == size());
  if (!values_in_row_major_style) {
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
}  // namespace pnrt
