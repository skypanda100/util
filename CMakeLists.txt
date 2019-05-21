cmake_minimum_required(VERSION 2.8)
project(util)

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
    endif ()
else ()
    set (CMAKE_C_STANDARD 99)
endif ()

set(SOURCE_FILES main.c util.h util.c)
add_executable(util ${SOURCE_FILES})