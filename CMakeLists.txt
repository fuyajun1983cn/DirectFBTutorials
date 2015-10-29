cmake_minimum_required(VERSION 2.8)
project(DirectFBTutorial)

#add_definitions("-fno-permissive")
set(CMAKE_CXX_FLAGS "-fpermissive")

set(EXECUTABLE_OUTPUT_PATH   ${PROJECT_SOURCE_DIR}/bin)

set(IMG_DIR "${PROJECT_SOURCE_DIR}/images")
set(FONT_DIR "${PROJECT_SOURCE_DIR}/fonts")
configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
   )
include_directories("${PROJECT_BINARY_DIR}")

find_package(DFB)
if (NOT DFB_FOUND)
   message(FATAL_ERROR "failed to find directfb")
   return()
endif()

include_directories(${DFB_INCLUDE_DIRS})

add_subdirectory(lesson01)
add_subdirectory(lesson02)
add_subdirectory(lesson03)
add_subdirectory(lesson04)
add_subdirectory(lesson05)
add_subdirectory(lesson06)
add_subdirectory(lesson07)
add_subdirectory(lesson08)
add_subdirectory(lesson09)
add_subdirectory(lesson10)
add_subdirectory(lesson11)
add_subdirectory(lesson12)
add_subdirectory(lesson13)
