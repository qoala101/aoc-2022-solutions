#ifndef DAY2_H_
#define DAY2_H_

#include <cassert>

namespace aoc {
enum class Shape { Rock, Paper, Scissors };

struct GameRound {
  Shape opponent_shape{};
  Shape my_shape{};
};

inline auto ToShape [[nodiscard]] (char character) {
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

inline auto ShapeScore [[nodiscard]] (Shape shape) {
  switch (shape) {
    case Shape::Rock:
      return 1;
    case Shape::Paper:
      return 2;
    case Shape::Scissors:
      return 3;
  }
}

inline auto CalculateGameScore [[nodiscard]] (const GameRound &game_round) {
  const auto my_shape_score = ShapeScore(game_round.my_shape);
  const auto shape_score_difference =
      (ShapeScore(game_round.opponent_shape) - my_shape_score + 3) % 3;

  static constexpr auto kDrawDifference = 0;
  static constexpr auto kLostDifference = 1;
  static constexpr auto kWonDifference = 2;

  static constexpr auto kLostPoints = 3;
  static constexpr auto kWonPoints = 6;

  switch (shape_score_difference) {
    case kDrawDifference:
      return my_shape_score + kLostPoints;
    case kLostDifference:
      return my_shape_score;
    case kWonDifference:
      return my_shape_score + kWonPoints;
    default:
      assert(false);
  }
}
}  // namespace aoc

#endif  // DAY2_H_
