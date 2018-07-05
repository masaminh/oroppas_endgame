cmake_minimum_required(VERSION 2.8)

add_subdirectory(lib)

add_executable(endgame main.cc)
target_link_libraries(endgame libendgame)
