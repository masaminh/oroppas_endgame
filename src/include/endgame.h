#ifndef OROPPAS_ENDGAME_H_
#define OROPPAS_ENDGAME_H_

#include <cstdint>
#include <limits>

namespace oroppas {
namespace endgame {
/// @brief 評価値の型
typedef int32_t ScoreType;

/// @breif 評価値の最大
constexpr static ScoreType kScoreTypeMax =
    std::numeric_limits<ScoreType>::max();

/// @breif 評価値の最小
constexpr static ScoreType kScoreTypeMin =
    -std::numeric_limits<ScoreType>::max();

}  // namespace endgame
}  // namespace oroppas

#endif
