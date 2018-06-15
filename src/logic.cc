#include "logic.h"

#include <algorithm>

#include "board.h"
#include "utility.h"

namespace oroppas {
namespace endgame {
namespace logic {

namespace board = oroppas::endgame::board;
namespace utility = oroppas::endgame::utility;

namespace {
///
/// Number of Training Zero (NTZ)
/// @remark 参考 http://www.nminoru.jp/~nminoru/programming/bitcount.html
///
int ntz(uint64_t x) { return utility::CountBits((~x) & (x - 1)); }
} // namespace

///
/// αβ探索
/// @param[in] black 着手側ビットボード
/// @param[in] white 相手側ビットボード
/// @param[in] alpha 対象とする下限値
/// @param[in] beta 対象とする上限値
/// @param[in,out] benchmark ベンチマーク用情報
/// @return 評価値
///
int alphabeta(uint64_t black, uint64_t white, int alpha, int beta,
              Benchmark *benchmark) {
  auto positions = board::GetMovableBitBoard(black, white);
  if (positions == 0) {
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
  for (auto i = ntz(positions); positions; ++i) {
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

      positions ^= position;
    }
  }

  return alpha;
}
} // namespace logic
} // namespace endgame
} // namespace oroppas
