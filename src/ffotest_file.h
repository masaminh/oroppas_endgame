#ifndef OROPPS_ENDGAME_FFOTEST_FILE_H_
#define OROPPS_ENDGAME_FFOTEST_FILE_H_

#include <cstdint>
#include <string>

namespace oroppas {
namespace endgame {
class FfoTestFile {
public:
  inline explicit FfoTestFile(const std::string &file_path)
      : file_path_(file_path), black_(0), white_(0) {}
  bool Read();

  /// 着手側のビットボードを取得する。
  /// @return ビットボード。最下位ビットから順にA1,B1…
  inline uint64_t GetBlack() const { return black_; }

  /// 相手側のビットボードを取得する。
  /// @return ビットボード。最下位ビットから順にA1,B1…
  inline uint64_t GetWhite() const { return white_; }

private:
  std::string file_path_;
  uint64_t black_;
  uint64_t white_;
};
} // namespace endgame
} // namespace oroppas

#endif // OROPPS_ENDGAME_FFOTEST_FILE_H_
