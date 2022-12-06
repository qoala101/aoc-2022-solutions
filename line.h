#ifndef LINE_H_
#define LINE_H_

#include <istream>
#include <string>

namespace aoc {
class Line {
 public:
  static auto ToString [[nodiscard]] (const Line &line) { return line.value_; }

 private:
  friend auto operator>>(std::istream &stream, Line &line) -> auto & {
    std::getline(stream, line.value_);
    return stream;
  }

  std::string value_{};
};
}  // namespace aoc

#endif  // LINE_H_
