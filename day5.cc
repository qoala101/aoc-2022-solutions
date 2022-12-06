#include <array>
#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <iterator>
#include <list>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/max.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/comparisons.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/utility/get.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/stride.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <sstream>  // IWYU pragma: keep
#include <string>
#include <utility>
#include <vector>

#include "common.h"
#include "line.h"

using ranges::accumulate;
using ranges::for_each;
using ranges::max;
using ranges::subrange;
using ranges::to_vector;
using ranges::views::drop;
using ranges::views::stride;
using ranges::views::take_while;
using ranges::views::transform;

namespace aoc {
struct Move {
  int num_crates_to_move{};
  int from_container_index{};
  int to_container_index{};

 private:
  friend auto operator>>(std::istream &stream, Move &move) -> auto & {
    auto discard = std::string{};

    stream >> discard >> move.num_crates_to_move >> discard >>
        move.from_container_index >> discard >> move.to_container_index;

    return stream;
  }
};

void Move9000(std::list<char> &from_container, int num_crates_to_move,
              std::list<char> &to_container) {
  for (auto i = 0; i < num_crates_to_move; ++i) {
    to_container.push_front(from_container.front());
    from_container.pop_front();
  }
}

void Move9001(std::list<char> &from_container, int num_crates_to_move,
              std::list<char> &to_container) {
  auto temp_container = std::list<char>{};
  Move9000(from_container, num_crates_to_move, temp_container);
  Move9000(temp_container, num_crates_to_move, to_container);
}

auto ApplyMoves
    [[nodiscard]] (std::vector<std::list<char>> containers,
                   const std::vector<Move> &moves, const auto &crane) {
  for_each(moves, [&containers, &crane](const auto &move) {
    auto &from_container = containers[move.from_container_index - 1];
    auto &to_container = containers[move.to_container_index - 1];
    crane(from_container, move.num_crates_to_move, to_container);
  });

  return containers;
}
}  // namespace aoc

auto main(int /*unused*/, const char *const *args) -> int {
  auto file_stream = std::ifstream{args[1]};

  const auto lines = subrange{std::istream_iterator<aoc::Line>{file_stream},
                              std::istream_iterator<aoc::Line>{}};
  const auto strings = lines | transform(aoc::Line::ToString);

  const auto crate_strings =
      strings | take_while([](const auto &string) { return !string.empty(); });
  const auto horizontal_char_lists =
      crate_strings | transform([](const auto &crate_string) {
        return crate_string | drop(1) | stride(4) | to_vector;
      }) |
      to_vector;

  const auto num_containers =
      max(horizontal_char_lists | transform([](const auto &horizontal_chars) {
            return horizontal_chars.size();
          }));
  auto containers = std::vector<std::list<char>>(num_containers);

  for_each(horizontal_char_lists, [&containers](
                                      const auto &horizontal_chars) mutable {
    auto container = containers.begin();

    for_each(horizontal_chars, [&container](const auto &character) mutable {
      if (aoc::IsInRange(character, {'A', 'Z'})) {
        container->push_back(character);
      }

      ++container;
    });
  });

  auto move_strings = strings | drop(1);
  const auto moves = move_strings | transform([](const auto &move_string) {
                       auto stream = std::istringstream{move_string};
                       auto move = aoc::Move{};
                       stream >> move;
                       return move;
                     }) |
                     to_vector;

  const auto cranes = std::array{&aoc::Move9000, &aoc::Move9001};

  for_each(cranes, [&containers, &moves](const auto &crane) {
    const auto moved_containers = aoc::ApplyMoves(containers, moves, crane);
    const auto top_containers_list =
        moved_containers |
        transform([](const auto &container) { return container.front(); });

    const auto top_containers = accumulate(top_containers_list, std::string{});

    std::cout << top_containers << "\n";
  });
}