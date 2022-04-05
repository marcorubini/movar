cmake_minimum_required(VERSION 3.20.0)

## =====================================================================================================
## Setup project
## =====================================================================================================
project(ml-movar VERSION 0.1.0
  DESCRIPTION "C++ variant with monadic interface" HOMEPAGE_URL "https://www.example.com" LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_CURRENT_BINARY_DIR})
  
## =====================================================================================================
## No in-source build
## =====================================================================================================
if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
  message(FATAL_ERROR "[ml::movar] Building in-source not recommended! Build in a separate directory.")
endif()

## =====================================================================================================
## Configuration
## =====================================================================================================
include(third_party/CPM)
include(config/options)
include(config/dependencies)

## =====================================================================================================
## Library target
## =====================================================================================================
include(targets/movar)

## =====================================================================================================
## Documentation target
## =====================================================================================================
if(ML_MOVAR_BUILD_DOCUMENTATION)
  include(targets/docs)
endif()

## =====================================================================================================
## Test target
## =====================================================================================================
if (ML_MOVAR_BUILD_TEST)
  include(targets/tests)
endif()

