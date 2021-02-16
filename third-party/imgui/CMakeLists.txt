cmake_minimum_required(VERSION 3.10)
set(CMAKE_MODULE_PATH .) # For FindSDL2.cmake

find_package(SDL2 REQUIRED)

add_library(imgui 
    imconfig.h
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_internal.h
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui.cpp
    imgui.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h

    backends/imgui_impl_opengl3.cpp 
    backends/imgui_impl_sdl.cpp
    glad/src/glad.c
)

target_include_directories(imgui PUBLIC 
    "backends",
    "."
    "third-party/imgui"
    "glad/include"
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(imgui PRIVATE ${SDL2_LIBRARIES} )
