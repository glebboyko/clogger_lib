cmake_minimum_required(VERSION 3.20)
project(clogger)

set(CMAKE_CXX_STANDARD 20)

include_directories(include source)

get_filename_component(CLOGGER_DIR_INCLUDE include ABSOLUTE DIRECTORY)
set(CLOGGER_DIR_INCLUDE ${CLOGGER_DIR_INCLUDE} PARENT_SCOPE)

add_library(${PROJECT_NAME}
        STATIC
        source/logger.cpp source/logger-standard.cpp)
add_library(clogger_lib ALIAS ${PROJECT_NAME})