#ifndef DAY3_H_
#define DAY3_H_

#include <cassert>
#include <utility>

inline auto InRange
    [[nodiscard]] (char character, std::pair<char, char> range) {
  return (character >= range.first) && (character <= range.second);
}

inline auto ToPriority [[nodiscard]] (char character) {
  if (InRange(character, {'a', 'z'})) {
    return int{character - 'a' + 1};
  }

  if (InRange(character, {'A', 'Z'})) {
    return int{character - 'A' + 'z' - 'a' + 2};
  }

  assert(false);
}

#endif  // DAY3_H_