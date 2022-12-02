#include <fstream>
#include <iostream>
#include <iterator>
#include <range/v3/algorithm/max.hpp>
#include <range/v3/functional/comparisons.hpp>
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
  const auto max_calories = ranges::max(calorie_sums);

  std::cout << max_calories;
}