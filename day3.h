#ifndef DAY3_H_
#define DAY3_H_

#include <cassert>
#include <utility>

#include "common.h"

namespace aoc {
inline auto ToPriority [[nodiscard]] (char character) {
  if (IsInRange(character, {'a', 'z'})) {
    return int{character - 'a' + 1};
  }

  if (IsInRange(character, {'A', 'Z'})) {
    return int{character - 'A' + 'z' - 'a' + 2};
  }

  assert(false);
}
}  // namespace aoc

#endif  // DAY3_H_