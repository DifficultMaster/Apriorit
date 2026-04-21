cmake_minimum_required(VERSION 3.20)
project(Apriorit4 VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Threads REQUIRED)

set(SHARED_SOURCES 
	"shared/src/network/unix_socket_service.cpp"
)

add_executable(Server server/src/main.cpp ${SHARED_SOURCES})
target_link_libraries(Server PRIVATE Threads::Threads)
target_include_directories(Server PRIVATE shared/include)

add_executable(Client client/src/main.cpp ${SHARED_SOURCES})
target_link_libraries(Client PRIVATE Threads::Threads)
target_include_directories(Client PRIVATE shared/include)