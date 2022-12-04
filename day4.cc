#include <array>
#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <iterator>
#include <range/v3/algorithm/copy.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>

using ranges::copy;
using ranges::count_if;
using ranges::subrange;
using ranges::views::transform;

namespace aoc {
using AssignmentPair = std::pair<std::pair<int, int>, std::pair<int, int>>;

auto IsValueInRange [[nodiscard]] (int value, std::pair<int, int> range) {
  return (value >= range.first) && (value <= range.second);
}

auto IsRangeInRange [[nodiscard]] (std::pair<int, int> inner_range,
                                   std::pair<int, int> outer_range) {
  return IsValueInRange(inner_range.first, outer_range) &&
         IsValueInRange(inner_range.second, outer_range);
}

auto HasFullOverlap [[nodiscard]] (const AssignmentPair &assignment_pair) {
  return IsRangeInRange(assignment_pair.first, assignment_pair.second) ||
         IsRangeInRange(assignment_pair.second, assignment_pair.first);
}
}  // namespace aoc

auto main(int /*unused*/, const char *const *args) -> int {
  auto file_stream = std::ifstream{args[1]};

  const auto strings = subrange{std::istream_iterator<std::string>{file_stream},
                                std::istream_iterator<std::string>{}};
  const auto string_arrays =
      strings | transform([](const auto &string) {
        const auto begin = string.begin();
        const auto first_dash = string.find_first_of('-');
        const auto comma = string.find_first_of(',', first_dash);
        const auto last_dash = string.find_first_of('-', comma);
        return std::array{std::string{begin, begin + first_dash},
                          std::string{begin + first_dash + 1, begin + comma},
                          std::string{begin + comma + 1, begin + last_dash},
                          std::string{begin + last_dash + 1, string.end()}};
      });
  const auto section_arrays =
      string_arrays | transform([](const auto &string_array) {
        auto sections_array = std::array<int, 4>{};
        copy(string_array | transform([](const auto &string) {
               return std::stoi(string);
             }),
             sections_array.begin());
        return sections_array;
      });
  const auto assignment_pair_lists =
      section_arrays | transform([](const auto &section_arrays) {
        return aoc::AssignmentPair{{section_arrays[0], section_arrays[1]},
                                   {section_arrays[2], section_arrays[3]}};
      });

  const auto num_full_overlaps =
      count_if(assignment_pair_lists, aoc::HasFullOverlap);

  std::cout << num_full_overlaps << "\n";
}