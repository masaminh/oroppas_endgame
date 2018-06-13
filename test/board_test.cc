#include <boost/test/unit_test.hpp>

#include <board.h>

BOOST_AUTO_TEST_SUITE(board)

BOOST_AUTO_TEST_CASE(GetMovableBitBoard_init) {
  BOOST_CHECK_EQUAL(0x0000102004080000,
                    oroppas::endgame::board::GetMovableBitBoard(
                        0x0000000810000000, 0x0000001008000000));
}

BOOST_AUTO_TEST_SUITE_END()
