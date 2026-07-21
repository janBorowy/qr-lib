cmake_minimum_required(VERSION 3.23)
project(qr-lib)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)

include(CTest)
include(lib/Catch2/extras/Catch.cmake)

project(qr-lib)

add_subdirectory(lib/Catch2)
add_subdirectory(lib/cli11)

add_subdirectory(core)
add_subdirectory(cli)

