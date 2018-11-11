cmake_minimum_required(VERSION 2.8)

add_subdirectory(lib)

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(endgame main.cc)
target_link_libraries(endgame libendgame)
