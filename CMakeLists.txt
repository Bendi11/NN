cmake_minimum_required(VERSION 3.10)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

project(NN-Builder)

include_directories(
    "builder/include"
    "third-party/json/include"
    ${CMAKE_SOURCE_DIR}
    "${CMAKE_SOURCE_DIR}/builder/include"
)

set(SOURCES 
    "builder/main.cpp"
    "builder/dataLoader.cpp"
)

if(WIN32)
    STRING(REPLACE "/Od" "/O2" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})    
    add_link_options("/nodefaultlib:msvcrt.lib")
endif()



add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

add_subdirectory(net)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE neural)

add_subdirectory(third-party/imgui)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui)

if(UNIX)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE dl)
endif()

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME "NN-Builder")
