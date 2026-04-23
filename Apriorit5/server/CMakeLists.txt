add_executable(Server
	src/main.cpp	
	src/network/file_transfer_server.cpp
)

target_include_directories(Server PRIVATE
	include
)

target_link_libraries(Server PRIVATE
	GrpcProtoLib
	Threads::Threads
)