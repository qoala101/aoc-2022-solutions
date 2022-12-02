#include <assert.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <range/v3/functional/arithmetic.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>

using ranges::accumulate;
using ranges::subrange;
using ranges::views::chunk;
using ranges::views::transform;

enum class Shape { Rock, Paper, Scissors };

auto ToShape [[nodiscard]] (char character) {
  switch (character) {
  case 'A':
  case 'X':
    return Shape::Rock;
  case 'B':
  case 'Y':
    return Shape::Paper;
  case 'C':
  case 'Z':
    return Shape::Scissors;
  default:
    assert(false);
  }
}

auto ShapeScore [[nodiscard]] (Shape shape) {
  switch (shape) {
  case Shape::Rock:
    return 1;
  case Shape::Paper:
    return 2;
  case Shape::Scissors:
    return 3;
  }
}

auto CalculateGameScore [[nodiscard]] (Shape opponent_shape, Shape my_shape) {
  const auto my_shape_score = ShapeScore(my_shape);
  const auto shape_score_difference =
      (ShapeScore(opponent_shape) - my_shape_score + 3) % 3;

  static const auto kDraw = 0;
  static const auto kWon = 1;
  static const auto kLost = 2;

  switch (shape_score_difference) {
  case kDraw:
    return my_shape_score + 3;
  case kWon:
    return my_shape_score + 6;
  case kLost:
    return my_shape_score;
  }
}

auto main(int, const char *const *args) -> int {
  auto file_stream = std::ifstream{args[1]};

  const auto chars = subrange{std::istream_iterator<char>{file_stream},
                              std::istream_iterator<char>{}};
  auto shapes = chars | transform(ToShape);
  auto shape_pairs = shapes | chunk(2);
  auto scores = shape_pairs | transform([](const auto &shape_pair) {
                  auto opponent_shape = shape_pair.begin();
                  const auto my_shape = ++opponent_shape;
                  return CalculateGameScore(*opponent_shape, *my_shape);
                });

  const auto total_score = accumulate(scores, 0);

  std::cout << total_score << "\n";
}