cmake_minimum_required(VERSION 3.20)
project(clogger)

set(CMAKE_CXX_STANDARD 20)

include_directories(include, source)

add_library(${PROJECT_NAME}
        STATIC
        source/logger.cpp source/logger-standard.cpp)
add_library(clogger_lib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_SOURCE_DIR})