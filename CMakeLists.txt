cmake_minimum_required(VERSION 3.20)
project(clogger)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS -pthread)

include_directories(include source)

get_filename_component(CLOGGER_DIR_INCLUDE include ABSOLUTE DIRECTORY)
set(CLOGGER_DIR_INCLUDE ${CLOGGER_DIR_INCLUDE} PARENT_SCOPE)

add_library(${PROJECT_NAME}
        STATIC
        source/clogger.cpp source/clogger-standard.cpp)
add_library(clogger_lib ALIAS ${PROJECT_NAME})