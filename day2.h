#ifndef DAY2_H_
#define DAY2_H_

#include <cassert>

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

inline auto CalculateGameScore [[nodiscard]] (GameRound game_round) {
  const auto my_shape_score = ShapeScore(game_round.my_shape);
  const auto shape_score_difference =
      (ShapeScore(game_round.opponent_shape) - my_shape_score + 3) % 3;

  static const auto kDraw = 0;
  static const auto kLost = 1;
  static const auto kWon = 2;

  switch (shape_score_difference) {
  case kDraw:
    return my_shape_score + 3;
  case kLost:
    return my_shape_score;
  case kWon:
    return my_shape_score + 6;
  default:
    assert(false);
  }
}

#endif // DAY2_H_
