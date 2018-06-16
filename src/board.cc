#include "board.h"

#include <functional>

namespace oroppas {
namespace endgame {
namespace board {
namespace {
class RightShift {
public:
  inline RightShift(int n) : n_(n) {}
  inline uint64_t operator()(uint64_t m) { return m >> n_; }

private:
  int n_;
};

class LeftShift {
public:
  inline LeftShift(int n) : n_(n) {}
  inline uint64_t operator()(uint64_t m) { return m << n_; }

private:
  int n_;
};

///
/// 反転用のビットパターンの取得
/// @remark 参照
/// https://ja.wikipedia.org/w/index.php?title=%E3%82%AA%E3%82%BB%E3%83%AD%E3%81%AB%E3%81%8A%E3%81%91%E3%82%8B%E3%83%93%E3%83%83%E3%83%88%E3%83%9C%E3%83%BC%E3%83%89&oldid=48206803
///
template <uint64_t mask, typename F>
uint64_t GetReversePattern(uint64_t m, uint64_t black, uint64_t white, F func) {
  uint64_t rev = 0;

  if (((black | white) & m) != 0) { // 着手箇所が空白で無い場合
    return rev;
  }

  uint64_t wh = white & mask; // 番人
  uint64_t m2, m3, m4, m5, m6;
  uint64_t m1 = func(m);
  if ((m1 & wh) != 0) {
    if (((m2 = func(m1)) & wh) == 0) {
      if ((m2 & black) != 0) { // 1個だけ返す場合
        rev |= m1;
      }
    } else if (((m3 = func(m2)) & wh) == 0) {
      if ((m3 & black) != 0) { // 2個返す場合
        rev |= m1 | m2;
      }
    } else if (((m4 = func(m3)) & wh) == 0) {
      if ((m4 & black) != 0) { // 3個返す場合
        rev |= m1 | m2 | m3;
      }
    } else if (((m5 = func(m4)) & wh) == 0) {
      if ((m5 & black) != 0) { // 4個返す場合
        rev |= m1 | m2 | m3 | m4;
      }
    } else if (((m6 = func(m5)) & wh) == 0) {
      if ((m6 & black) != 0) { // 5個返す場合
        rev |= m1 | m2 | m3 | m4 | m5;
      }
    } else {
      if ((func(m6) & black) != 0) { // 6個返す場合
        rev |= m1 | m2 | m3 | m4 | m5 | m6;
      }
    }
  }

  return rev;
}
} // namespace

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
  bool rc;
  uint64_t rev = 0;

  rev |= GetReversePattern<0x00ffffffffffff00>(position, *black, *white,
                                               LeftShift(8)); // 下方向
  rev |= GetReversePattern<0x007e7e7e7e7e7e00>(position, *black, *white,
                                               LeftShift(7)); // 左下方向
  rev |= GetReversePattern<0x7e7e7e7e7e7e7e7e>(position, *black, *white,
                                               RightShift(1)); // 左方向
  rev |= GetReversePattern<0x007e7e7e7e7e7e00>(position, *black, *white,
                                               RightShift(9)); // 左上方向
  rev |= GetReversePattern<0x00ffffffffffff00>(position, *black, *white,
                                               RightShift(8)); // 上方向
  rev |= GetReversePattern<0x007e7e7e7e7e7e00>(position, *black, *white,
                                               RightShift(7)); // 右上方向
  rev |= GetReversePattern<0x7e7e7e7e7e7e7e7e>(position, *black, *white,
                                               LeftShift(1)); // 右方向
  rev |= GetReversePattern<0x007e7e7e7e7e7e00>(position, *black, *white,
                                               LeftShift(9)); // 右下方向

  if (rev) {
    *black |= position | rev;
    *white ^= rev;
    rc = true;
  } else {
    rc = false;
  }

  return rc;
}

} // namespace board
} // namespace endgame
} // namespace oroppas
