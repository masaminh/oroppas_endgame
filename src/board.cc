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
/// @remark 参照 https://ja.wikipedia.org/wiki/オセロにおけるビットボード
///
uint64_t GetReversePattern(uint64_t mask, uint64_t m, uint64_t black,
                           uint64_t white,
                           std::function<uint64_t(uint64_t)> func) {
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

uint64_t GetMovableBitBoard(uint64_t black, uint64_t white) {
  uint64_t result = 0;
  uint64_t m = 1;
  uint64_t space_board = ~(black | white);

  for (auto i = 0; i < 64; ++i) {
    if ((m & space_board) &&
        (GetReversePattern(0x00ffffffffffff00, m, black, white,
                           LeftShift(8)) || // 下方向
         GetReversePattern(0x007e7e7e7e7e7e00, m, black, white,
                           LeftShift(7)) || // 左下方向
         GetReversePattern(0x7e7e7e7e7e7e7e7e, m, black, white,
                           RightShift(1)) || // 左方向
         GetReversePattern(0x007e7e7e7e7e7e00, m, black, white,
                           RightShift(9)) || // 左上方向
         GetReversePattern(0x00ffffffffffff00, m, black, white,
                           RightShift(8)) || // 上方向
         GetReversePattern(0x007e7e7e7e7e7e00, m, black, white,
                           RightShift(7)) || // 右上方向
         GetReversePattern(0x7e7e7e7e7e7e7e7e, m, black, white,
                           LeftShift(1)) || // 右方向
         GetReversePattern(0x007e7e7e7e7e7e00, m, black, white,
                           LeftShift(9)))) // 右下方向
    {
      result |= m;
    }

    m <<= 1;
  }

  return result;
}

bool Move(uint64_t position, uint64_t *black, uint64_t *white) {
  bool rc;
  uint64_t rev = 0;

  rev |= GetReversePattern(0x00ffffffffffff00, position, *black, *white,
                           LeftShift(8)); // 下方向
  rev |= GetReversePattern(0x007e7e7e7e7e7e00, position, *black, *white,
                           LeftShift(7)); // 左下方向
  rev |= GetReversePattern(0x7e7e7e7e7e7e7e7e, position, *black, *white,
                           RightShift(1)); // 左方向
  rev |= GetReversePattern(0x007e7e7e7e7e7e00, position, *black, *white,
                           RightShift(9)); // 左上方向
  rev |= GetReversePattern(0x00ffffffffffff00, position, *black, *white,
                           RightShift(8)); // 上方向
  rev |= GetReversePattern(0x007e7e7e7e7e7e00, position, *black, *white,
                           RightShift(7)); // 右上方向
  rev |= GetReversePattern(0x7e7e7e7e7e7e7e7e, position, *black, *white,
                           LeftShift(1)); // 右方向
  rev |= GetReversePattern(0x007e7e7e7e7e7e00, position, *black, *white,
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
