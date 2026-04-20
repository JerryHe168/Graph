cmake_minimum_required(VERSION 3.10)

project(GraphLibrary VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(graph_test main.cpp)

target_include_directories(graph_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    target_compile_options(graph_test PRIVATE /W4 /utf-8)
else()
    target_compile_options(graph_test PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME GraphTest COMMAND graph_test)
