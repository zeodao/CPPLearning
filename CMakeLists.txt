cmake_minimum_required(VERSION 3.15)

project(ZTool LANGUAGES C CXX)

message("PROJECT_NAME: ${PROJECT_NAME}")
message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("PROJECT_SOURCE_DIR_SRC: ${PROJECT_SOURCE_DIR}/src")

message("PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

if (PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
  message(WARNING "The binary derectory of CMake cannot be the same as source directory!")
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if (WIN32)
  add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_EXTENSIONS True)

include_directories(${PROJECT_SOURCE_DIR}/src/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRCS)

add_executable(${PROJECT_NAME} ${SRCS})
