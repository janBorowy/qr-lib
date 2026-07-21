add_executable(qr-lib-tests)

target_sources(qr-lib-tests
  PRIVATE
  	test/data_encoding_tests.cpp
	test/ec_generation_tests.cpp
	test/assembly_tests.cpp
	test/qr_matrix_evaluation_tests.cpp
	test/data_masking_tests.cpp)

target_compile_definitions(qr-lib-tests PRIVATE cimg_display=0)

target_link_libraries(qr-lib-tests PRIVATE Catch2::Catch2WithMain)

set(CPP_SOURCE_FILES
message_encoding/data_analysis.cpp
message_encoding/data_encoding.cpp
message_encoding/error_correction/gf_polynomial.cpp
message_encoding/error_correction/generator_polynomial.cpp
message_encoding/error_correction/ec_generation.cpp
message_encoding/codewords_assembly.cpp
module_placement/qr_function_patterns.cpp
data_masking/data_mask_resolver.cpp
data_masking/evaluators.cpp
qr_code_generator.cpp
qrlib.cpp
)
target_sources(qr-lib-tests PRIVATE ${CPP_SOURCE_FILES}
test/test_utils.cpp
)

add_library(qr-lib)
target_sources(qr-lib
	PRIVATE	${CPP_SOURCE_FILES}
	PUBLIC
	FILE_SET qrLibHeaders
	TYPE HEADERS
	BASE_DIRS include
	FILES include/qrlib.h
	)

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})
target_link_libraries(qr-lib ${X11_LIBRARIES})

catch_discover_tests(qr-lib-tests)
