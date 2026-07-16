cmake_minimum_required(VERSION 3.31)

project(qr-lib)

set (CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)

include(CTest)
include(lib/Catch2/extras/Catch.cmake)

project(qr-lib)

add_subdirectory(lib/Catch2)

add_executable(qr-lib-tests test/data_encoding_tests.cpp
			    test/ec_generation_tests.cpp
			    test/assembly_tests.cpp
		            test/qr_matrix_evaluation_tests.cpp
		            test/data_masking_tests.cpp)

target_compile_definitions(qr-lib-tests PRIVATE cimg_display=0)

target_link_libraries(qr-lib-tests PRIVATE Catch2::Catch2WithMain)
target_sources(qr-lib-tests PRIVATE src/steps/data_analysis.cpp
test/test_utils.cpp
src/steps/data_encoding.cpp
src/steps/error_correction/gf_polynomial.cpp
src/steps/error_correction/generator_polynomial.cpp
src/steps/error_correction/ec_generation.cpp
src/steps/codewords_assembly.cpp
src/data_masking/data_mask_resolver.cpp
src/data_masking/evaluators.cpp
src/qr_code_generator.cpp
src/qr_function_patterns.cpp
src/debug.cpp
)

add_executable(qr-lib-demo src/main.cpp)
target_sources(qr-lib-demo PRIVATE src/steps/data_analysis.cpp
src/steps/data_encoding.cpp
src/steps/error_correction/gf_polynomial.cpp
src/steps/error_correction/generator_polynomial.cpp
src/steps/error_correction/ec_generation.cpp
src/steps/codewords_assembly.cpp
src/data_masking/evaluators.cpp
src/data_masking/data_mask_resolver.cpp
src/qr_code_generator.cpp
src/qr_function_patterns.cpp
src/debug.cpp
)

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

target_link_libraries(qr-lib-demo ${X11_LIBRARIES})

catch_discover_tests(qr-lib-tests)

