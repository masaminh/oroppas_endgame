#include "board.h"

#include <functional>

namespace oroppas {
namespace endgame {
namespace board {

///
/// 着手可能位置のビットボードを取得する
/// @param[in] black 着手側ビットボード
/// @param[in] white 相手側ビットボード
/// @return 着手可能位置
/// @remark 参考 https://qiita.com/sensuikan1973/items/459b3e11d91f3cb37e43
///
uint64_t GetMovableBitBoard(uint64_t black, uint64_t white) {
  //左右端の番人
  uint64_t horizontalWatchBoard = white & 0x7e7e7e7e7e7e7e7e;
  //上下端の番人
  uint64_t verticalWatchBoard = white & 0x00ffffffffffff00;
  //全辺の番人
  uint64_t allSideWatchBoard = white & 0x007e7e7e7e7e7e00;
  //空きマスのみにビットが立っているボード
  uint64_t blankBoard = ~(black | white);

  // 8方向チェック (・一度に返せる石は最大6つ ・高速化のためにforを展開)
  //右
  uint64_t tmp = horizontalWatchBoard & (black << 1);
  tmp |= horizontalWatchBoard & (tmp << 1);
  tmp |= horizontalWatchBoard & (tmp << 1);
  tmp |= horizontalWatchBoard & (tmp << 1);
  tmp |= horizontalWatchBoard & (tmp << 1);
  tmp |= horizontalWatchBoard & (tmp << 1);
  uint64_t legalBoard = blankBoard & (tmp << 1);

  //左
  tmp = horizontalWatchBoard & (black >> 1);
  tmp |= horizontalWatchBoard & (tmp >> 1);
  tmp |= horizontalWatchBoard & (tmp >> 1);
  tmp |= horizontalWatchBoard & (tmp >> 1);
  tmp |= horizontalWatchBoard & (tmp >> 1);
  tmp |= horizontalWatchBoard & (tmp >> 1);
  legalBoard |= blankBoard & (tmp >> 1);

  //下
  tmp = verticalWatchBoard & (black << 8);
  tmp |= verticalWatchBoard & (tmp << 8);
  tmp |= verticalWatchBoard & (tmp << 8);
  tmp |= verticalWatchBoard & (tmp << 8);
  tmp |= verticalWatchBoard & (tmp << 8);
  tmp |= verticalWatchBoard & (tmp << 8);
  legalBoard |= blankBoard & (tmp << 8);

  //上
  tmp = verticalWatchBoard & (black >> 8);
  tmp |= verticalWatchBoard & (tmp >> 8);
  tmp |= verticalWatchBoard & (tmp >> 8);
  tmp |= verticalWatchBoard & (tmp >> 8);
  tmp |= verticalWatchBoard & (tmp >> 8);
  tmp |= verticalWatchBoard & (tmp >> 8);
  legalBoard |= blankBoard & (tmp >> 8);

  //左斜め下
  tmp = allSideWatchBoard & (black << 7);
  tmp |= allSideWatchBoard & (tmp << 7);
  tmp |= allSideWatchBoard & (tmp << 7);
  tmp |= allSideWatchBoard & (tmp << 7);
  tmp |= allSideWatchBoard & (tmp << 7);
  tmp |= allSideWatchBoard & (tmp << 7);
  legalBoard |= blankBoard & (tmp << 7);

  //右斜め下
  tmp = allSideWatchBoard & (black << 9);
  tmp |= allSideWatchBoard & (tmp << 9);
  tmp |= allSideWatchBoard & (tmp << 9);
  tmp |= allSideWatchBoard & (tmp << 9);
  tmp |= allSideWatchBoard & (tmp << 9);
  tmp |= allSideWatchBoard & (tmp << 9);
  legalBoard |= blankBoard & (tmp << 9);

  //左斜め上
  tmp = allSideWatchBoard & (black >> 9);
  tmp |= allSideWatchBoard & (tmp >> 9);
  tmp |= allSideWatchBoard & (tmp >> 9);
  tmp |= allSideWatchBoard & (tmp >> 9);
  tmp |= allSideWatchBoard & (tmp >> 9);
  tmp |= allSideWatchBoard & (tmp >> 9);
  legalBoard |= blankBoard & (tmp >> 9);

  //右斜め上
  tmp = allSideWatchBoard & (black >> 7);
  tmp |= allSideWatchBoard & (tmp >> 7);
  tmp |= allSideWatchBoard & (tmp >> 7);
  tmp |= allSideWatchBoard & (tmp >> 7);
  tmp |= allSideWatchBoard & (tmp >> 7);
  tmp |= allSideWatchBoard & (tmp >> 7);
  legalBoard |= blankBoard & (tmp >> 7);

  return legalBoard;
}

bool Move(uint64_t position, uint64_t *black, uint64_t *white) {
  bool rc = false;
  auto pos = utility::CountNTZ(position);
  auto OM_x = *white;
  auto OM_y = *white & 0x7e7e7e7e7e7e7e7e;
  auto OM_z = OM_y;
  auto OM_w = OM_y;

  auto mask_x = 0x0080808080808080 >> (63 - pos);
  auto mask_y = 0x7f00000000000000 >> (63 - pos);
  auto mask_z = 0x0102040810204000 >> (63 - pos);
  auto mask_w = 0x0040201008040201 >> (63 - pos);

  auto outflank_x =
      (0x8000000000000000 >> utility::CountNLZ(~OM_x & mask_x)) & *black;
  auto outflank_y =
      (0x8000000000000000 >> utility::CountNLZ(~OM_y & mask_y)) & *black;
  auto outflank_z =
      (0x8000000000000000 >> utility::CountNLZ(~OM_z & mask_z)) & *black;
  auto outflank_w =
      (0x8000000000000000 >> utility::CountNLZ(~OM_w & mask_w)) & *black;

  auto flipped_x = (-outflank_x * 2) & mask_x;
  auto flipped_y = (-outflank_y * 2) & mask_y;
  auto flipped_z = (-outflank_z * 2) & mask_z;
  auto flipped_w = (-outflank_w * 2) & mask_w;

  mask_x = 0x0101010101010100 << pos;
  mask_y = uint64_t{0x00000000000000fe} << pos;
  mask_z = 0x0002040810204080 << pos;
  mask_w = 0x8040201008040200 << pos;

  outflank_x = mask_x & ((OM_x | ~mask_x) + 1) & *black;
  outflank_y = mask_y & ((OM_y | ~mask_y) + 1) & *black;
  outflank_z = mask_z & ((OM_z | ~mask_z) + 1) & *black;
  outflank_w = mask_w & ((OM_w | ~mask_w) + 1) & *black;

  flipped_x |= (outflank_x - (outflank_x != 0)) & mask_x;
  flipped_y |= (outflank_y - (outflank_y != 0)) & mask_y;
  flipped_z |= (outflank_z - (outflank_z != 0)) & mask_z;
  flipped_w |= (outflank_w - (outflank_w != 0)) & mask_w;

  auto rev = flipped_x | flipped_y | flipped_z | flipped_w;

  rc = rev != 0;
  if (rc) {
    *black ^= position | rev;
    *white ^= rev;
  }

  return rc;
}

} // namespace board
} // namespace endgame
} // namespace oroppas
