#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <iterator>
#include <range/v3/functional/arithmetic.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>

#include "day2.h"

using ranges::accumulate;
using ranges::subrange;
using ranges::views::chunk;
using ranges::views::transform;

auto main(int /*unused*/, const char *const *args) -> int {
  auto file_stream = std::ifstream{args[1]};

  const auto chars = subrange{std::istream_iterator<char>{file_stream},
                              std::istream_iterator<char>{}};
  auto shapes = chars | transform(aoc::ToShape);
  auto shape_pairs = shapes | chunk(2);
  auto game_rounds =
      shape_pairs | transform([](const auto &shape_pair) {
        auto shape = shape_pair.begin();
        return aoc::GameRound{.opponent_shape = *shape, .my_shape = *++shape};
      });
  auto scores = game_rounds | transform(aoc::CalculateGameScore);

  const auto total_score = accumulate(scores, 0);

  std::cout << total_score << "\n";
}