cmake_minimum_required(VERSION 2.8.2)
project (ENDGAME)

set(CMAKE_CXX_STANDARD 14)

SET(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(src)
if(NOT without-test)
    enable_testing()
    add_subdirectory(test)
endif()
