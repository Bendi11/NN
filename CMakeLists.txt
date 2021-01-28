cmake_minimum_required(VERSION 3.10)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
project(NN)


include_directories(
    "src/include"
)

set(SRCS 
    "src/main.cpp"
    "src/net.cpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

