load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "pnrt",
    srcs = [
        "Tensor.cc",
        "ir.cc",
        "store-zip.cc",
    ],
    hdrs = glob(["*.h"]),  # 在sandbox里生命这些头文件存在
    implementation_deps = [
        "@glog",
    ],
    includes = ["."],  # 指定构建这个库依赖的头文件路径
    visibility = ["//visibility:public"],
    deps = ["//include:pnrt-public-headers"],
)

cc_test(
    name = "ir-test",
    srcs = ["ir-test.cc"],
    data = [
        "//data:all_data",
    ],
    deps = [
        ":pnrt",
        "@glog",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "tensor-test",
    srcs = ["tensor-test.cc"],
    deps = [
        ":pnrt",
        "@glog",
        "@googletest//:gtest_main",
    ],
)
