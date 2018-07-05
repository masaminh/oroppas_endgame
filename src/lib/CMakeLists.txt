cmake_minimum_required(VERSION 2.8)

file(GLOB sourcefiles *.cc)

add_library(libendgame ${sourcefiles})
