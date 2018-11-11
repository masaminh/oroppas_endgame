find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_definitions(-DBOOST_TEST_DYN_LINK)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(unit_test main.cc board_test.cc)

target_link_libraries(unit_test libendgame ${Boost_LIBRARIES})

add_test(
        NAME unit_test
        COMMAND $<TARGET_FILE:unit_test>
        )
