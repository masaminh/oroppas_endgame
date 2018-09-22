#ifndef OROPPAS_ENDGAME_LOGIC_H_
#define OROPPAS_ENDGAME_LOGIC_H_

#include <cstdint>

namespace oroppas {
namespace endgame {
namespace logic {
struct Benchmark {
  inline Benchmark() : internal(0), leaf(0) {}
  int64_t internal;
  int64_t leaf;
};

int GetScore(uint64_t black, uint64_t white, int alpha, int beta,
             Benchmark *benchmark);
}  // namespace logic
}  // namespace endgame
}  // namespace oroppas
#endif
