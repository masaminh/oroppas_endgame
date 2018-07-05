#include "logic.h"

#include <algorithm>

#include "board.h"
#include "utility.h"

namespace oroppas {
namespace endgame {
namespace logic {

namespace board = oroppas::endgame::board;
namespace utility = oroppas::endgame::utility;

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

  auto blank = ~(black | white);
  auto countBlank = utility::CountBits(blank);
  if (countBlank == 1) {
    // 最後の1手のときは別処理
    ++benchmark->leaf;
    auto position = uint64_t{1} << utility::CountNTZ(blank);
    board::Move(position, &black, &white);
    return std::max(alpha, board::GetScore(black, white));
  }

  // 相手の着手可能位置が少ない順にソートして走査対象の枝を減らす
  struct ScoreTable {
    uint64_t black;
    uint64_t white;
    int score;
  };

  ScoreTable score_table[60];
  int score_table_size = 0;

  for (auto i = utility::CountNTZ(positions); positions; ++i) {
    auto position = uint64_t{1} << i;
    if (positions & position) {
      ScoreTable *score = score_table + score_table_size;
      score->black = black;
      score->white = white;
      board::Move(position, &score->black, &score->white);
      ++score_table_size;
      positions ^= position;
    }
  }

  // 最終盤までソートするとそのコストの方が遅くなるので、一定のところでソートは止めておく
  if (score_table_size > 1 && countBlank > 4) {
    for (auto i = 0; i < score_table_size; ++i) {
      auto score = score_table + i;
      score->score = utility::CountBits(
          board::GetMovableBitBoard(score->white, score->black));
    }

    std::sort(score_table, score_table + score_table_size,
              [](const ScoreTable &a, const ScoreTable &b) {
                return a.score < b.score;
              });
  }

  for (auto i = 0; i < score_table_size; ++i) {
    const auto &e = score_table[i];
    alpha =
        std::max(alpha, -alphabeta(e.white, e.black, -beta, -alpha, benchmark));

    if (alpha >= beta) {
      return alpha;
    }
  }

  return alpha;
}
} // namespace logic
} // namespace endgame
} // namespace oroppas
