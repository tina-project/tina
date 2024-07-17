cmake_minimum_required(VERSION 3.10)
project(tina VERSION 0.0.1)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

include(cmake/openssl-dep.cmake)

set(TINA_BINARY_OUTPUT_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

add_subdirectory(third_party)
add_subdirectory(core)
