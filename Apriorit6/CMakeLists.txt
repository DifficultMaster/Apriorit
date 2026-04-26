cmake_minimum_required(VERSION 3.20)
project(Apriorit6 VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Apriorit6
    src/main.cpp
    task1/src/guards/file_handle_guard.cpp
    task2/include/deleters/socket_deleters.hpp
    task3/src/snippet1.cpp
    task3/src/snippet2.cpp 
    task3/src/snippet3.cpp 
    
)

target_include_directories(Apriorit6
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/task1/include
        ${CMAKE_CURRENT_SOURCE_DIR}/task2/include
        ${CMAKE_CURRENT_SOURCE_DIR}/task3/include
        ${CMAKE_CURRENT_SOURCE_DIR}/task4/include
)

target_compile_features(Apriorit6 PRIVATE cxx_std_23)

if (WIN32)
    target_link_libraries(Apriorit6 PRIVATE Ws2_32)
endif()
