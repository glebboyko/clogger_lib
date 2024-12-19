cmake_minimum_required(VERSION 3.20)
project(clogger)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS -pthread)

include_directories(include source)

add_library(${PROJECT_NAME}
        STATIC
        source/clogger.cpp source/clogger-standard.cpp source/var_string.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "lib_")