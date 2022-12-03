#include <fstream>
#include <iostream>
#include <iterator>
#include <range/v3/functional/arithmetic.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/set_algorithm.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <set>
#include <string>
#include <string_view>
#include <utility>

#include "line.h"

using ranges::accumulate;
using ranges::subrange;
using ranges::to;
using ranges::views::set_intersection;
using ranges::views::transform;

auto InRange [[nodiscard]] (char min, char character, char max) {
  return (character >= min) && (character <= max);
}

auto ToPriority [[nodiscard]] (char character) {
  if (InRange('a', character, 'z')) {
    return int{character - 'a' + 1};
  }

  if (InRange('A', character, 'Z')) {
    return int{character - 'A' + 'z' - 'a' + 2};
  }
}

auto main(int /*unused*/, const char *const *args) -> int {
  auto file_stream = std::ifstream{args[1]};

  const auto lines = subrange{std::istream_iterator<Line>{file_stream},
                              std::istream_iterator<Line>{}};
  const auto strings = lines | transform(Line::ToString);
  const auto char_sets =
      strings | transform([](const auto &string) {
        const auto middle = string.begin() + string.size() / 2;
        return std::pair{
            to<std::set<char>>(std::string_view{string.begin(), middle}),
            to<std::set<char>>(std::string_view{middle, string.end()})};
      });
  const auto shared_chars =
      char_sets | transform([](const auto &char_set) {
        return *set_intersection(char_set.first, char_set.second).begin();
      });
  const auto priorities = shared_chars | transform(ToPriority);

  const auto sum = accumulate(priorities, 0);

  std::cout << sum << "\n";
}