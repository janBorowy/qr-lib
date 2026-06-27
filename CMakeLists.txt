cmake_minimum_required(VERSION 3.31)
set (CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(qr-lib)
add_executable(qr-lib-demo)

add_subdirectory(lib/Catch2)

add_executable(qr-lib-tests test/test.cpp)
target_link_libraries(qr-lib-tests PRIVATE Catch2::Catch2WithMain)


target_sources(qr-lib-demo PRIVATE src/main.cpp)
