#ifndef OROPPS_ENDGAME_BOARD_H_
#define OROPPS_ENDGAME_BOARD_H_

#include <cstdint>

namespace oroppas {
namespace endgame {
namespace board {

int CountBits(uint64_t bits);

inline int GetScore(uint64_t black, uint64_t white) {
  return CountBits(black) - CountBits(white);
}

uint64_t GetMovableBitBoard(uint64_t black, uint64_t white);

inline bool IsFinished(uint64_t black, uint64_t white) {
  return GetMovableBitBoard(black, white) == 0 &&
         GetMovableBitBoard(white, black) == 0;
}

bool Move(uint64_t position, uint64_t *black, uint64_t *white);

} // namespace board
} // namespace endgame
} // namespace oroppas

#endif // OROPPS_ENDGAME_BOARD_H_
