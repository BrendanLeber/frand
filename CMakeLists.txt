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


# application info
set(APPLICATION_NAME "frand")
set(APPLICATION_CODENAME "frand")
set(APPLICATION_COPYRIGHT_YEARS "2013-2017")
set(APPLICATION_VERSION_MAJOR 1)
set(APPLICATION_VERISON_MINOR 0)
set(APPLICATION_VERSION_PATCH 0)
set(APPLICATION_VERSION_STRING "1.0.0")
set(APPLICATION_VENDOR_ID "com.brendanleber")
set(APPLICATION_VENDOR_NAME "Brendan Leber")
set(APPLICATION_VENDOR_URL "brendanleber.com")
set(APPLICATION_ID "com.brendanleber.frand")
set(APPLICATION_SHORT_DESC "Create a random collection of folders.")
set(APPLICATION_BUGREPORT "frand@brendanleber.com")
set(APPLICATION_URL "http://frand.brendanleber.com/")
configure_file("${PROJECT_SOURCE_DIR}/src/appinfo.hpp.in" "${PROJECT_BINARY_DIR}/appinfo.hpp")
include_directories("${PROJECT_BINARY_DIR}")

#include_directories(${PROJECT_SOURCE_DIR}/external/cxxopts/include)

set(FRAND_SOURCES
  src/frand.cpp
  src/options.cpp)

add_executable(frand ${FRAND_SOURCES})
