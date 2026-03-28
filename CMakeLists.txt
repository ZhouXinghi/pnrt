cmake_minimum_required(VERSION 3.20)
project(pnrt)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PNRT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Armadillo REQUIRED)
find_package(GTest REQUIRED)
find_package(GLog REQUIRED)

include_directories(${PNRT_DIR}/pnrt)
include_directories(${PNRT_DIR}/include)

list(APPEND CMAKE_MODULE_PATH "${PNRT_DIR}/cmake")
include(pnrt-common)

add_subdirectory(pnrt)
add_subdirectory(test)
