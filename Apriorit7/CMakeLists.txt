cmake_minimum_required(VERSION 3.20)
project(Apriorit7 VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(Apriorit7
	src/main.cpp
)

target_include_directories(Apriorit7 PRIVATE include)