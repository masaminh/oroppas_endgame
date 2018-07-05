include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(unit_test main.cc board_test.cc)

target_link_libraries(unit_test libendgame ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(
        NAME unit_test
        COMMAND $<TARGET_FILE:unit_test>
        )
