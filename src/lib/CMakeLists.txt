cmake_minimum_required(VERSION 2.8)

find_package(Boost REQUIRED)

include_directories(../include)

file(GLOB sourcefiles *.cc)

add_library(libendgame ${sourcefiles})
target_include_directories(libendgame PUBLIC ../include)
