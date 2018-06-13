#include "ffotest_file.h"
#include <fstream>

namespace oroppas {
namespace endgame {
bool FfoTestFile::Read() {
  std::ifstream ifs(file_path_);
  std::string board_string;
  std::string color_string;
  std::getline(ifs, board_string);
  board_string = board_string.substr(0, 64);
  std::getline(ifs, color_string);

  char black_mark = 'X';
  char white_mark = 'O';
  if (color_string == "White") {
    std::swap(black_mark, white_mark);
  }

  black_ = white_ = 0;
  uint64_t posbit = 1;

  for (auto ch : board_string) {
    assert(ch == 'O' || ch == 'X' || ch == '-');
    if (ch == black_mark) {
      black_ |= posbit;
    } else if (ch == white_mark) {
      white_ |= posbit;
    }
    posbit <<= 1;
  }

  return true;
}
} // namespace endgame
} // namespace oroppas
