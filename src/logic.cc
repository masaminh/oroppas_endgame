#include "logic.h"

#include <algorithm>

#include "board.h"

namespace oroppas {
namespace endgame {
namespace logic {

namespace board = oroppas::endgame::board;

int alphabeta(uint64_t black, uint64_t white, int alpha, int beta,
              Benchmark *benchmark) {
  if (board::GetMovableBitBoard(black, white) == 0) {
    if (board::GetMovableBitBoard(white, black) == 0) {
      // 双方が石を置けない
      ++benchmark->leaf;
      return board::GetScore(black, white);
    } else {
      // パス
      return -alphabeta(white, black, -beta, -alpha, benchmark);
    }
  }

  ++benchmark->internal;
  auto positions = board::GetMovableBitBoard(black, white);
  for (auto i = 0; i < 64; ++i) {
    auto position = uint64_t{1} << i;
    if (positions & position) {
      auto newblack = black;
      auto newwhite = white;
      board::Move(position, &newblack, &newwhite);
      alpha = std::max(
          alpha, -alphabeta(newwhite, newblack, -beta, -alpha, benchmark));

      if (alpha >= beta) {
        return alpha;
      }
    }
  }

  return alpha;
}
} // namespace logic
} // namespace endgame
} // namespace oroppas
