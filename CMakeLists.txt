cmake_minimum_required(VERSION 3.8)

project(frand)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_XX_FLAGS} -Weverything -Wno-c++98-compat -Wno-padded -pedantic")
#set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None, Debug, Release, RelWithDebInfo and MinSizeRel."
    FORCE)
endif()

#include_directories(${PROJECT_SOURCE_DIR}/external/cxxopts/include)

set(FRAND_SOURCES
  src/frand.cpp
  src/options.cpp)

add_executable(frand ${FRAND_SOURCES})
