cmake_minimum_required(VERSION 3.10)
include(FetchContent) #For JSON fetching from git repo

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}) #For FindSDL2.cmake

if(WIN32)
include("E:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake") #Find vcpkg script
endif()

project(NN)

find_package(Curses REQUIRED)


include_directories(
    "src/include"
    "third-party/json/include"
    ${CMAKE_SOURCE_DIR}
    ${Curses_INCLUDE_DIR}
    "${CMAKE_SOURCE_DIR}/src/include"
)

set(SOURCES 
    "src/main.cpp"
    "src/net.cpp"
    "src/dataLoader.cpp"
)

if(WIN32)
    STRING(REPLACE "/Od" "/O2" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})    
    # add_link_options("/subsystem:windows")
    add_link_options("/nodefaultlib:msvcrt.lib")
endif()



add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

if(WIN32)
find_path(PDC_INCLUDES curses.h)
find_library(PDC_LIBS pdcurses REQUIRED)

target_include_directories(${CMAKE_PROJECT_NAME} ${PDC_INCLUDES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${PDC_LIBS})
else()
target_link_libraries(${CMAKE_PROJECT_NAME} ${Curses_LIBRARIES})
endif()

