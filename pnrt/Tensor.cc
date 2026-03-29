#include <Tensor.h>
#include <logging.h>

namespace pnrt {

void Tensor<float>::Fill(const std::vector<float>& values, bool row_major) {
  PNRT_CHECK(values.size() == size());
}
}  // namespace pnrt
