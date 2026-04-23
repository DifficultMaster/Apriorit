cmake_minimum_required(VERSION 3.20)
project(Apriorit5 VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

add_subdirectory(shared)
add_subdirectory(server)
add_subdirectory(client)