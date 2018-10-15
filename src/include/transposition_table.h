#ifndef OROPPS_ENDGAME_TRANSPOSITION_TABLE_H_
#define OROPPS_ENDGAME_TRANSPOSITION_TABLE_H_

#include <array>
#include <cstdint>
#include <tuple>
#include <unordered_map>

#include "endgame.h"

namespace oroppas {
namespace endgame {
/// 置換表
class TranspositionTable {
 public:
  /// @brief コンストラクタ
  TranspositionTable();

  TranspositionTable(const TranspositionTable &) = delete;

  TranspositionTable &operator=(const TranspositionTable &) = delete;

  /// @brief 検索
  /// @param [in] black 着手側ビットボード
  /// @param [in] white 相手側ビットボード
  /// @retval (-∞,∞) 見つからなかった
  /// @retval それ以外 キャッシュされている値(最小値,最大値)
  const std::tuple<int, int> &find(uint64_t black, uint64_t white) const;

  /// @brief 更新
  /// @param [in] black 着手側ビットボード
  /// @param [in] white 相手側ビットボード
  /// @param [in] min 更新する最小値
  /// @param [in] max 更新する最大値
  inline void update(uint64_t black, uint64_t white, ScoreType min,
                     ScoreType max) {
    table[std::make_tuple(black, white)] = std::make_tuple(min, max);
  }

  /// @brief 要素数の取得
  /// @retval - 要素数
  inline std::size_t size() const { return table.size(); }

 private:
  /// @brief 置換表のキー型
  typedef std::tuple<uint64_t, uint64_t> key_t;

  /// @brief 置換表のハッシュ生成関数オブジェクト
  struct KeyHash {
    /// @brief コンストラクタ
    KeyHash();

    /// @brief ()演算子
    /// @param [in] k キー
    /// @retval ハッシュ値
    std::size_t operator()(const key_t &k) const;

   private:
    /// @brief ハッシュ計算用テーブルサイズ
    constexpr static std::size_t kHashArraySize = 2 * 8 * 256;

    /// @brief ハッシュ計算用テーブル
    std::array<uint64_t, kHashArraySize> hash_array_;
  };

  /// @brief 置換表本体
  std::unordered_map<key_t, std::tuple<ScoreType, ScoreType>, KeyHash> table;
};  // namespace endgame
}  // namespace endgame
}  // namespace oroppas

#endif  // OROPPS_ENDGAME_TRANSPOSITION_TABLE_H_
