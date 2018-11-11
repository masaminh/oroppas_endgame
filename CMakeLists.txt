cmake_minimum_required(VERSION 2.8.2)
project (ENDGAME)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-march=native)

add_subdirectory(src)
if(NOT without-test)
    enable_testing()
    add_subdirectory(test)
endif()
