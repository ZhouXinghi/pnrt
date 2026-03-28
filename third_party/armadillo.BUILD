load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")

cc_import(
    name = "armadillo_import",
    shared_library = "lib/libarmadillo.dylib",
)

cc_library(
    name = "armadillo",
    hdrs = glob(["include/**"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [":armadillo_import"],
)
