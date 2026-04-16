cmake_minimum_required(VERSION 3.20)
project(Apriorit2 VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_custom_target(Apriorit2
    SOURCES
        "before.hpp"
        "after.hpp"
)
