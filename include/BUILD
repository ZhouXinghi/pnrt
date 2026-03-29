load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "pnrt-public-headers",
    hdrs = glob(["*.h"]),
    implementation_deps = [
        "@glog",
    ],
    includes = ["."],
    visibility = ["//visibility:public"],
    deps = [
        "@armadillo",
    ],
)
