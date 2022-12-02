#ifndef LINE_H_
#define LINE_H_

#include <string>

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

#endif // LINE_H_
