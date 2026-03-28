pnrt_add_library(
  pnrt
  SHARED
  SRCS
  ir.cc
  store-zip.cc
  DEPS
  glog::glog)

pnrt_add_test(ir-test SRCS ir-test.cc DEPS pnrt glog::glog)
