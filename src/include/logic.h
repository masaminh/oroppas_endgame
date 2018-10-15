#ifndef OROPPAS_ENDGAME_LOGIC_H_
#define OROPPAS_ENDGAME_LOGIC_H_

#include <cstdint>
#include "transposition_table.h"

namespace oroppas {
namespace endgame {
namespace logic {
struct Benchmark {
  inline Benchmark() : internal(0), leaf(0) {}
  int64_t internal;
  int64_t leaf;
};

/// @brief 盤面からスコアを推測する
/// @param [in] black 着手側ビットボード
/// @param [in] white 相手側ビットボード
/// @param [in] alpha 対象とする下限値
/// @param [in] beta 対象とする上限値
/// @param [in,out] table 置換表
/// @param [in,out] benchmark ベンチマーク用情報
/// @return 評価値
int GetScore(uint64_t black, uint64_t white, int alpha, int beta,
             TranspositionTable *table, Benchmark *benchmark);
}  // namespace logic
}  // namespace endgame
}  // namespace oroppas
#endif
