cmake_minimum_required(VERSION 3.20)
project(Apriorit1 VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(Apriorit1
	src/main.cpp
    src/controllers/process_controller.cpp
    src/providers/windows_process_provider.cpp
    src/exporters/csv_exporter.cpp
    src/exporters/json_exporter.cpp
    src/exporters/factories/csv_exporter_factory.cpp
    src/exporters/factories/json_exporter_factory.cpp
)

target_include_directories(Apriorit1 PRIVATE include)