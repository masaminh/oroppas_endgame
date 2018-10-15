#include "transposition_table.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <random>
#include <tuple>

#include "utility.h"

namespace utility = oroppas::endgame::utility;

namespace oroppas {
namespace endgame {
namespace {
static std::size_t get_hash(uint64_t board, const uint64_t *a, int color) {
  std::size_t result = 0;
  auto board_ptr = reinterpret_cast<const uint8_t *>(&board);

  for (auto i = 0; i < 8; ++i) {
    result ^= a[color * 256 * 8 + i * 256 + board_ptr[i]];
  }

  return result;
}
}  // namespace

TranspositionTable::TranspositionTable() : table(12'000'000) {}

const std::tuple<int, int> &TranspositionTable::find(uint64_t black,
                                                     uint64_t white) const {
  constexpr static auto kScoreMax = std::numeric_limits<int>::max();
  constexpr static auto kNotFound = std::make_tuple(-kScoreMax, kScoreMax);

  auto it = table.find(std::make_tuple(black, white));
  return it == table.end() ? kNotFound : std::get<1>(*it);
}

TranspositionTable::KeyHash::KeyHash() {
  std::mt19937 mt{std::random_device{}()};
  std::uniform_int_distribution<uint64_t> dist(
      0, std::numeric_limits<uint64_t>::max());

  std::generate(hash_array_.begin(), hash_array_.end(),
                [&mt, &dist]() { return dist(mt); });
}

std::size_t TranspositionTable::KeyHash::operator()(const key_t &k) const {
  uint64_t black, white;
  std::tie(black, white) = k;
  return get_hash(black, hash_array_.data(), 0) +
         get_hash(white, hash_array_.data(), 1);
}

}  // namespace endgame
}  // namespace oroppas
