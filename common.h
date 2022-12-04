#ifndef COMMON_H_
#define COMMON_H_

#include <utility>

namespace aoc {
template <typename T>
inline auto IsInRange
    [[nodiscard]] (const T& value, const std::pair<T, T>& range) {
  return (value >= range.first) && (value <= range.second);
}
}  // namespace aoc

#endif  // COMMON_H_
