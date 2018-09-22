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
/// 盤面からスコアを推測する (最終盤用)
/// @param[in] black 着手側ビットボード
/// @param[in] white 相手側ビットボード
/// @param[in] alpha 対象とする下限値
/// @param[in] beta 対象とする上限値
/// @param[in,out] benchmark ベンチマーク用情報
/// @return 評価値
///
int GetScoreLeafside(uint64_t black, uint64_t white, int alpha, int beta,
                     Benchmark *benchmark) {
  auto blank = ~(black | white);
  auto countBlank = utility::CountBits(blank);
  if (countBlank == 1) {
    // 最後の1手のときは別処理
    ++benchmark->leaf;
    if (board::Move(blank, &black, &white)) {
      ++benchmark->internal;
      return board::GetScore(black, white);
    }
    if (board::Move(blank, &white, &black)) {
      ++benchmark->internal;
      return -board::GetScore(white, black);
    }

    return board::GetScore(black, white);
  }

  auto positions = board::GetMovableBitBoard(black, white);
  if (positions == 0) {
    if (board::GetMovableBitBoard(white, black) == 0) {
      // 双方が石を置けない
      ++benchmark->leaf;
      return board::GetScore(black, white);
    } else {
      // パス
      return -GetScoreLeafside(white, black, -beta, -alpha, benchmark);
    }
  }

  ++benchmark->internal;

  while (positions) {
    auto position = uint64_t{1} << utility::CountNTZ(positions);

    auto newb = black;
    auto neww = white;
    board::Move(position, &newb, &neww);

    alpha = std::max(alpha,
                     -GetScoreLeafside(neww, newb, -beta, -alpha, benchmark));

    if (alpha >= beta) {
      return alpha;
    }

    positions ^= position;
  }

  return alpha;
}

///
/// 盤面からスコアを推測する
/// @param[in] black 着手側ビットボード
/// @param[in] white 相手側ビットボード
/// @param[in] alpha 対象とする下限値
/// @param[in] beta 対象とする上限値
/// @param[in,out] benchmark ベンチマーク用情報
/// @return 評価値
///
int GetScore(uint64_t black, uint64_t white, int alpha, int beta,
             Benchmark *benchmark) {
  auto blank = ~(black | white);
  auto countBlank = utility::CountBits(blank);
  if (countBlank <= 4) {
    return GetScoreLeafside(black, white, alpha, beta, benchmark);
  }

  auto positions = board::GetMovableBitBoard(black, white);
  if (positions == 0) {
    if (board::GetMovableBitBoard(white, black) == 0) {
      // 双方が石を置けない
      ++benchmark->leaf;
      return board::GetScore(black, white);
    } else {
      // パス
      return -GetScore(white, black, -beta, -alpha, benchmark);
    }
  }

  ++benchmark->internal;

  // 相手の着手可能位置が少ない順にソートして走査対象の枝を減らす
  struct ScoreTable {
    uint64_t black;
    uint64_t white;
    int score;
  };

  ScoreTable score_table[60];
  int score_table_size = 0;

  while (positions) {
    auto position = uint64_t{1} << utility::CountNTZ(positions);

    ScoreTable *score = score_table + score_table_size;
    score->black = black;
    score->white = white;
    board::Move(position, &score->black, &score->white);
    ++score_table_size;

    positions ^= position;
  }

  if (score_table_size > 1) {
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

  const auto &e = score_table[0];
  auto v = -GetScore(e.white, e.black, -beta, -alpha, benchmark);
  auto max = v;

  if (beta <= v) {
    return v;
  }

  if (score_table_size > 1) {
    if (alpha < v) {
      alpha = v;
    }

    for (auto i = 1; i < score_table_size; ++i) {
      const auto &e = score_table[i];
      v = -GetScore(e.white, e.black, -alpha - 1, -alpha,
                    benchmark);  // Null Window Search

      if (beta <= v) {
        return v;  // カット
      }

      if (alpha < v) {
        alpha = v;
        v = -GetScore(e.white, e.black, -beta, -alpha,
                      benchmark);  // 通常の窓で再探索

        if (beta <= v) {
          return v;  // カット
        }

        if (alpha < v) {
          alpha = v;
        }
      }

      if (max < v) {
        max = v;
      }
    }
  }

  return max;
}
}  // namespace logic
}  // namespace endgame
}  // namespace oroppas
