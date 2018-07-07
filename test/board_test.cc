#include <cstdint>

#include <boost/test/unit_test.hpp>

#include "board.h"

BOOST_AUTO_TEST_SUITE(board)

BOOST_AUTO_TEST_CASE(GetMovableBitBoard_init) {
  BOOST_CHECK_EQUAL(0x0000102004080000,
                    oroppas::endgame::board::GetMovableBitBoard(
                        0x0000000810000000, 0x0000001008000000));
}

BOOST_AUTO_TEST_CASE(GetMovableBitBoard_end40) {
  BOOST_CHECK_EQUAL(0x086c050000000106,
                    oroppas::endgame::board::GetMovableBitBoard(
                        0x008080c0c48c8080, 0x0010783f3b737e79));
}

BOOST_AUTO_TEST_CASE(Move_end40_a6) {
  uint64_t black = 0x008080c0c48c8080;
  uint64_t white = 0x0010783f3b737e79;
  oroppas::endgame::board::Move(0x0000010000000000, &black, &white);
  BOOST_CHECK_EQUAL(0x008081c2c48c8080, black);
  BOOST_CHECK_EQUAL(0x0010783d3b737e79, white);
}

BOOST_AUTO_TEST_SUITE_END()
