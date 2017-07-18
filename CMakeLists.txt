cmake_minimum_required(VERSION 3.8)

project(frand)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_XX_FLAGS} -Weverything -Wno-c++98-compat -Wno-padded -pedantic")
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None, Debug, Release, RelWithDebInfo and MinSizeRel."
    FORCE)
endif()


# microsoft gsl
set(GSL_URL https://github.com/Microsoft/GSL)
set(GSL_VERSION be43c79)
message(STATUS "Using and shipping ${GSL_URL} version ${GCL_VERSION}")
set(GSL_DIR ${CMAKE_CURRENT_BINARY_DIR}/external/GSL)
if(NOT EXISTS ${GSL_DIR})
  execute_process(COMMAND git clone ${GSL_URL} ${GSL_DIR})
endif()
execute_process(COMMAND git checkout ${GSL_VERSION} WORKING_DIRECTORY ${GSL_DIR})
#install(DIRECTORY ${GSL_DIR}/gsl DESTINATION include)
#install(FILES ${GSL_DIR}/LICENSE DESTINATION include/gsl/LICENSE)
set(GSL_INCLUDE_DIR NAMES ${GSL_DIR})

if(GSL_INCLUDE_DIR)
  set(GSL_INCLUDE_DIRS ${GSL_INCLUDE_DIR}/include)
  include_directories(${GSL_INCLUDE_DIRS})
  set(GSL_FOUND TRUE)
endif()


# herb sutter gcpp
set(GCPP_URL https://github.com/hsutter/gcpp)
set(GCPP_VERSION 3be430d)
message(STATUS "Using and shipping ${GCPP_URL} version ${GCPP_VERSION}")
set(GCPP_DIR ${CMAKE_CURRENT_BINARY_DIR}/external/gcpp)
if(NOT EXISTS ${GCPP_DIR})
  execute_process(COMMAND git clone ${GCPP_URL} ${GCPP_DIR})
endif()
execute_process(COMMAND git checkout ${GCPP_VERSION} WORKING_DIRECTORY ${GCPP_DIR})
#install(DIRECTORY ${GCPP_DIR} DESTINATION include)
#install(FILES ${GCPP_DIR}/LICENSE DESTINATION include/gcpp/LICENSE)
set(GCPP_INCLUDE_DIR NAMES ${GCPP_DIR})

if(GCPP_INCLUDE_DIR)
  set(GCPP_INCLUDE_DIRS ${GCPP_INCLUDE_DIR})
  include_directories(${GCPP_INCLUDE_DIRS})
  set(GCPP_FOUND TRUE)
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
