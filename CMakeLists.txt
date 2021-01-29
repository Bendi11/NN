cmake_minimum_required(VERSION 3.10)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}) #For FindSDL2.cmake
include("E:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake") #Find vcpkg script

project(NN)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_library("glad" "glad/src/glad.c")


include_directories(
    "src/include"
    "glad/include/"
    ${CMAKE_SOURCE_DIR}
    "${CMAKE_SOURCE_DIR}/src/include"
    "${CMAKE_SOURCE_DIR}/src/modules/"
    "imgui"
)

set(SRCS 
    "src/main.cpp"
    "src/net.cpp"
    "src/gui.cpp"

    imgui/imconfig.h
    imgui/imgui.cpp
    imgui/imgui.h
    imgui/imgui_draw.cpp
    imgui/imgui_internal.h
    imgui/imgui_widgets.cpp
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imgui_tables.cpp
    imgui/imstb_truetype.h
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_impl_opengl3.h
    imgui/imgui_impl_sdl.cpp
    imgui/imgui_impl_sdl.h
)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_link_libraries(${CMAKE_PROJECT_NAME}
    "glad"
)
target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY})

