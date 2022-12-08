#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <iterator>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/iterator/operations.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <set>
#include <vector>

using ranges::distance;
using ranges::subrange;
using ranges::to_vector;
using ranges::views::sliding;
using ranges::views::take_while;
using ranges::views::transform;

namespace aoc {
auto FindMessageStartPos(const std::vector<char> &chars, int marker_length) {
  const auto char_lists = chars | sliding(marker_length);
  const auto char_sets =
      char_lists | transform([](const auto &char_list) {
        return std::set<char>(char_list.begin(), char_list.end());
      });
  const auto not_unique_char_sets =
      char_sets | take_while([marker_length](const auto &char_set) {
        return static_cast<int>(char_set.size()) < marker_length;
      });

  return distance(not_unique_char_sets) + marker_length;
}
}  // namespace aoc

auto main(int /*unused*/, const char *const *args) -> int {
  auto file_stream = std::ifstream{args[1]};

  const auto chars = subrange{std::istream_iterator<char>{file_stream},
                              std::istream_iterator<char>{}} |
                     to_vector;

  const auto message_start_pos_4 = aoc::FindMessageStartPos(chars, 4);
  const auto message_start_pos_14 = aoc::FindMessageStartPos(chars, 14);

  std::cout << message_start_pos_4 << "\n" << message_start_pos_14 << "\n";
}