#include <cassert>
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

#include "day2.h"

using ranges::accumulate;
using ranges::subrange;
using ranges::views::chunk;
using ranges::views::transform;

enum class Action { Lose, Draw, Win };

struct NewRulesRound {
  Shape opponent_shape{};
  Action my_action{};
};

auto ToAction [[nodiscard]] (char character) {
  switch (character) {
    case 'X':
      return Action::Lose;
    case 'Y':
      return Action::Draw;
    case 'Z':
      return Action::Win;
    default:
      assert(false);
  }
}

auto ShapeWhichLosesTo [[nodiscard]] (Shape winner_shape) {
  return static_cast<Shape>((static_cast<int>(winner_shape) - 1 + 3) % 3);
}

auto ShapeWhichWinsAgainst [[nodiscard]] (Shape loser_shape) {
  return static_cast<Shape>((static_cast<int>(loser_shape) + 1 + 3) % 3);
}

auto ToOldRulesRound [[nodiscard]] (const NewRulesRound &new_rules_round) {
  const auto my_shape = [&new_rules_round]() {
    switch (new_rules_round.my_action) {
      case Action::Lose:
        return ShapeWhichLosesTo(new_rules_round.opponent_shape);
      case Action::Draw:
        return new_rules_round.opponent_shape;
      case Action::Win:
        return ShapeWhichWinsAgainst(new_rules_round.opponent_shape);
    }
  }();

  return GameRound{.opponent_shape = new_rules_round.opponent_shape,
                   .my_shape = my_shape};
}

auto main(int /*unused*/, const char *const *args) -> int {
  auto file_stream = std::ifstream{args[1]};

  const auto chars = subrange{std::istream_iterator<char>{file_stream},
                              std::istream_iterator<char>{}};
  auto char_pairs = chars | chunk(2);
  auto new_rules_rounds =
      char_pairs | transform([](const auto &char_pair) {
        auto character = char_pair.begin();
        return NewRulesRound{.opponent_shape = ToShape(*character),
                             .my_action = ToAction(*++character)};
      });
  auto old_rules_rounds = new_rules_rounds | transform(ToOldRulesRound);
  auto scores = old_rules_rounds | transform(CalculateGameScore);

  const auto total_score = accumulate(scores, 0);

  std::cout << total_score << "\n";
}