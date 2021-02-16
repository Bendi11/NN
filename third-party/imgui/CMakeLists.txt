cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE) #If vcpkg.cmake is used, don't use findSDL2.cmake
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}) # For FindSDL2.cmake
endif()
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(imgui 
    imconfig.h
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
    "backends"
    "."
    "third-party/imgui"
    "glad/include"
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(imgui PUBLIC SDL2::SDL2main SDL2::SDL2 ${OPENGL_gl_LIBRARY})
