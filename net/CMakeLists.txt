#This file is only used if you want to build neural network library with cmake using add_subdirectory
cmake_minimum_required(VERSION 3.10)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


add_library(neural 
    "src/net.cpp"
)

target_include_directories(neural PUBLIC include)