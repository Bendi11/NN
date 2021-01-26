cmake_minimum_required(VERSION 3.10)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(wxWidgets_ROOT_DIR $ENV{WXWIN})
project(NN)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

include_directories(
    "src/include"
)

set(SRCS 
    "src/main.cpp"
    "src/net.cpp"
    "src/gui.cpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

