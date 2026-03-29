load("@rules_cc//cc:defs.bzl", "cc_library")

# TODO: This only works for macOS, because it uses the Accelerate framework. For Linux, you must link against OpenBLAS or another BLAS implementation, and adjust the build file accordingly.
cc_library(
    name = "armadillo",
    hdrs = glob(["include/**"]),
    defines = [
        "ARMA_DONT_USE_WRAPPER",
        "ARMA_USE_ACCELERATE",
    ],
    includes = ["include"],
    linkopts = ["-framework Accelerate"],
    visibility = ["//visibility:public"],
)
