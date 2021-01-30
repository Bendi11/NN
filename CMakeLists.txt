cmake_minimum_required(VERSION 3.10)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}) #For FindSDL2.cmake
if(WIN32)
include("E:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake") #Find vcpkg script
endif()

project(NN)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_library("glad" "third-party/glad/src/glad.c")


include_directories(
    "src/include"
    "third-party/glad/include/"
    ${CMAKE_SOURCE_DIR}
    "${CMAKE_SOURCE_DIR}/src/include"
    "third-party/imgui"
)

set(SOURCES 
    "src/main.cpp"
    "src/net.cpp"
    "src/gui.cpp"

    third-party/imgui/imconfig.h
    third-party/imgui/imgui.cpp
    third-party/imgui/imgui.h
    third-party/imgui/imgui_draw.cpp
    third-party/imgui/imgui_internal.h
    third-party/imgui/imgui_widgets.cpp
    third-party/imgui/imstb_rectpack.h
    third-party/imgui/imstb_textedit.h
    third-party/imgui/imgui_tables.cpp
    third-party/imgui/imstb_truetype.h
    third-party/imgui/imgui_impl_opengl3.cpp
    third-party/imgui/imgui_impl_opengl3.h
    third-party/imgui/imgui_impl_sdl.cpp
    third-party/imgui/imgui_impl_sdl.h
    third-party/imgui/imgui_stdlib.cpp
)

if(WIN32)
    set(CMAKE_CXX_FLAGS "/O2")
    add_link_options("/subsystem:windows")
    add_link_options("/nodefaultlib:msvcrt.lib")
endif()



add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} OpenGL::GL)
target_link_libraries(${CMAKE_PROJECT_NAME}
    "glad"
)
target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY})
if(UNIX)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_DL_LIBS})
endif()
