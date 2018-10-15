#ifndef OROPPAS_ENDGAME_LOGIC_H_
#define OROPPAS_ENDGAME_LOGIC_H_

#include <cstdint>

#include "endgame.h"
#include "transposition_table.h"

namespace oroppas {
namespace endgame {
namespace logic {

/// @brief ベンチマーク用構造体
struct Benchmark {
  inline Benchmark() : internal(0), leaf(0) {}

  /// @brief 中間ノード数
  uint64_t internal;

  /// @brief 末端ノード数
  uint64_t leaf;
};

/// @brief 盤面からスコアを推測する
/// @param [in] black 着手側ビットボード
/// @param [in] white 相手側ビットボード
/// @param [in] alpha 対象とする下限値
/// @param [in] beta 対象とする上限値
/// @param [in,out] table 置換表
/// @param [in,out] benchmark ベンチマーク用情報
/// @return 評価値
int GetScore(uint64_t black, uint64_t white, ScoreType alpha, ScoreType beta,
             TranspositionTable *table, Benchmark *benchmark);
}  // namespace logic
}  // namespace endgame
}  // namespace oroppas
#endif
