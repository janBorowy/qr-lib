add_executable(qr-cli)
target_sources(qr-cli
	PRIVATE main.cpp)

target_link_libraries(qr-cli PRIVATE qr-lib)
target_link_libraries(qr-cli PRIVATE CLI11::CLI11)
