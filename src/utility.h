#ifndef OROPPAS_ENDGAME_UTILITY_H_
#define OROPPAS_ENDGAME_UTILITY_H_

#include <cstdint>

#include <x86intrin.h>

namespace oroppas {
namespace endgame {
namespace utility {
///
/// 1が立っているビットを数える
///
inline int CountBits(uint64_t bits) { return _popcnt64(bits); }

///
/// 下位ビットから続く0の数を数える
///
inline int CountNTZ(uint64_t bits) { return _tzcnt_u64(bits); }

///
/// 上位ビットから続く0の数を数える
///
inline int CountNLZ(uint64_t bits) {
  if (bits == 0) {
    return 64;
  } else {
    int64_t res;
    __asm__("bsrq %1, %0" : "=r"(res) : "r"(bits));
    return (63 - res);
  }
}
} // namespace utility
} // namespace endgame
} // namespace oroppas

#endif // OROPPAS_ENDGAME_UTILITY_H_
