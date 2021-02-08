cmake_minimum_required(VERSION 3.10)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)


set(libSOURCES
    "src/neural/net.cpp"
)

add_library(libNeural ${libSOURCES}) #Add Neural network library

target_include_directories( libNeural
    PUBLIC
    "src/neural/include"
)


project(NN)

include_directories(
    "src/manager/include"
    "third-party/json/include"
    ${CMAKE_SOURCE_DIR}
    ${CURSES_INCLUDE_DIR}
    "${CMAKE_SOURCE_DIR}/src/manager/include"
)

set(SOURCES 
    "src/manager/main.cpp"
    "src/manager/dataLoader.cpp"
    "src/manager/cli.cpp"
)

if(WIN32)
    STRING(REPLACE "/Od" "/O2" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})    
    add_link_options("/nodefaultlib:msvcrt.lib")
endif()



add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE libNeural)

if(WIN32) #Build PDCurses on windows

add_subdirectory(third-party/PDCurses-3.8) #Build and link PDcurses using the cmakelists.txt file
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE PDcurses)

else() #Use Ncurses on *nixes

find_library(Curses REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CURSES_LIBRARIES})

endif()
