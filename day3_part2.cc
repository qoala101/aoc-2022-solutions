#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <iterator>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/functional/arithmetic.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/utility/get.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/set_algorithm.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <set>
#include <string>
#include <utility>

#include "day3.h"

using ranges::accumulate;
using ranges::subrange;
using ranges::to;
using ranges::views::chunk;
using ranges::views::drop;
using ranges::views::set_intersection;
using ranges::views::transform;

auto main(int /*unused*/, const char *const *args) -> int {
  auto file_stream = std::ifstream{args[1]};

  const auto strings = subrange{std::istream_iterator<std::string>{file_stream},
                                std::istream_iterator<std::string>{}};
  auto string_lists = strings | chunk(3);
  auto char_set_lists =
      string_lists | transform([](const auto &string_list) {
        return string_list | transform([](const auto &string) {
                 return to<std::set<char>>(string);
               });
      });
  auto shared_chars =
      char_set_lists | transform([](const auto &char_sets) {
        auto intersection = *char_sets.begin();

        ranges::for_each(char_sets | drop(1), [&](const auto &char_set) {
          intersection =
              to<std::set<char>>(set_intersection(intersection, char_set));
        });

        return *intersection.begin();
      });
  auto priorities = shared_chars | transform(aoc::ToPriority);

  const auto sum = accumulate(priorities, 0);

  std::cout << sum << "\n";
}