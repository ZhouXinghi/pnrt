load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "pnrt-public-headers",
    hdrs = glob(["*.h"]),
    includes = ["."],
    visibility = ["//visibility:public"],
    deps = [
        "@armadillo",
    ],
)
