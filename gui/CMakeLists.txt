add_executable(qr-gui)
target_sources(qr-gui
	PRIVATE main.cpp)

target_link_libraries(qr-gui PRIVATE qr-lib)
