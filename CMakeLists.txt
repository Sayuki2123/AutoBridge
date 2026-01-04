cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

project(autobridge)

add_library(${PROJECT_NAME} SHARED src/main.cpp src/module_helper.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} log)
