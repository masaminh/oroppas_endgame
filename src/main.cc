#include <chrono>
#include <iostream>
#include <limits>

#include "board.h"
#include "ffotest_file.h"
#include "logic.h"

using oroppas::endgame::FfoTestFile;
namespace chrono = std::chrono;
namespace board = oroppas::endgame::board;
namespace logic = oroppas::endgame::logic;

int main(int /*argc*/, char *argv[]) {
  FfoTestFile file(argv[1]);
  file.Read();
  auto black = file.GetBlack();
  auto white = file.GetWhite();
  const auto int_max = std::numeric_limits<int>::max();
  logic::Benchmark benchmark;
  auto begin = chrono::high_resolution_clock::now();
  auto result = logic::alphabeta(black, white, -int_max, int_max, &benchmark);
  auto end = chrono::high_resolution_clock::now();
  auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end - begin);
  auto elapsed = elapsed_time.count() * 1.0;
  auto total = benchmark.internal + benchmark.leaf;
  std::cout << result << "\t" << benchmark.internal << "\t" << benchmark.leaf
            << "\t" << total << "\t" << elapsed / 1000 << "\t"
            << total / elapsed << std::endl;
  return 0;
}
