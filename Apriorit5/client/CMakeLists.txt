add_executable(Client
	src/main.cpp
	src/network/file_transfer_client.cpp
)

target_include_directories(Client PRIVATE
	include
)

target_link_libraries(Client PRIVATE
	GrpcProtoLib
	Threads::Threads
)