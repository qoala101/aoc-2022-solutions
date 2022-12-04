#include <array>
#include <fstream>  // IWYU pragma: keep
#include <functional>
#include <iostream>
#include <iterator>
#include <range/v3/algorithm/partial_sort_copy.hpp>
#include <range/v3/functional/arithmetic.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>

using ranges::accumulate;
using ranges::partial_sort_copy;
using ranges::subrange;
using ranges::views::split;
using ranges::views::transform;

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

auto main(int /*unused*/, const char *const *args) -> int {
  auto file_stream = std::ifstream{args[1]};

  const auto lines = subrange{std::istream_iterator<aoc::Line>{file_stream},
                              std::istream_iterator<aoc::Line>{}};
  const auto strings = lines | transform(aoc::Line::ToString);
  auto string_lists = strings | split(std::string{});
  auto calorie_lists = string_lists | transform([](const auto &string_list) {
                         return string_list | transform([](const auto &string) {
                                  return std::stoi(string);
                                });
                       });
  auto calorie_sums = calorie_lists | transform([](const auto &calorie_list) {
                        return accumulate(calorie_list, 0);
                      });

  auto top_calories = std::array<int, 3>{};
  partial_sort_copy(calorie_sums, top_calories, std::greater<>{});

  const auto max_calories = top_calories[0];
  const auto top_calories_sum = accumulate(top_calories, 0);

  std::cout << max_calories << "\n" << top_calories_sum << "\n";
}