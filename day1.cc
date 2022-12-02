#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <range/v3/algorithm/partial_sort_copy.hpp>
#include <range/v3/functional/arithmetic.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>

#include "line.h"

using ranges::accumulate;
using ranges::partial_sort_copy;
using ranges::views::split;
using ranges::views::transform;

auto main(int, const char *const *args) -> int {
  auto file_stream = std::ifstream{args[1]};

  const auto lines = ranges::subrange{std::istream_iterator<Line>{file_stream},
                                      std::istream_iterator<Line>{}};
  const auto strings = lines | transform(Line::ToString);
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