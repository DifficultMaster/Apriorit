cmake_minimum_required(VERSION 3.20)
project(Apriorit3 VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(Apriorit3
	src/main.cpp
)

target_include_directories(Apriorit3 PRIVATE include)

find_package(Threads REQUIRED)
target_link_libraries(Apriorit3 PRIVATE Threads::Threads)