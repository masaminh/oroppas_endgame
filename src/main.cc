#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

#include "board.h"
#include "ffotest_file.h"
#include "logic.h"

using oroppas::endgame::FfoTestFile;
namespace chrono = std::chrono;
namespace board = oroppas::endgame::board;
namespace logic = oroppas::endgame::logic;

std::string get_comma_int(uint64_t value) {
  std::vector<std::string> v;

  do {
    v.push_back(
        (boost::format(value > 999 ? "%03d" : "%d") % (value % 1000)).str());
    value /= 1000;
  } while (value);

  return boost::algorithm::join(boost::adaptors::reverse(v), ",");
}

std::string get_comma_double(double value) {
  auto int_value = (uint64_t)value;
  auto double_value = value - int_value;
  auto double_str = (boost::format("%.3f") % double_value).str().substr(1);
  return get_comma_int(int_value) + double_str;
}

int main(int argc, char *argv[]) {
  for (auto i = 1; i < argc; ++i) {
    FfoTestFile file(argv[i]);
    file.Read();
    auto black = file.GetBlack();
    auto white = file.GetWhite();
    const auto int_max = std::numeric_limits<int>::max();
    logic::Benchmark benchmark;
    auto begin = chrono::high_resolution_clock::now();
    auto result = logic::alphabeta(black, white, -int_max, int_max, &benchmark);
    auto end = chrono::high_resolution_clock::now();
    auto elapsed_time =
        chrono::duration_cast<chrono::milliseconds>(end - begin);
    auto elapsed = elapsed_time.count() * 1.0;
    auto total = benchmark.internal + benchmark.leaf;
    std::cout << "|" << argv[i] << "|" << result << "|"
              << get_comma_int(benchmark.internal) << "|"
              << get_comma_int(benchmark.leaf) << "|" << get_comma_int(total)
              << "|" << get_comma_double(elapsed / 1000) << "|"
              << get_comma_double(total / elapsed) << "|" << std::endl;
  }

  return 0;
}
