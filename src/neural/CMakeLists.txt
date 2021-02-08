#This file is only used if you want to build neural network library with cmake using add_subdirectory
cmake_minimum_required(VERSION 3.10)
project(libNeural)

set(SOURCES
    "net.cpp"
)

add_library(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include)