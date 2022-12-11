#include <bits/std_abs.h>

#include <cassert>
#include <cstdlib>
#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <iterator>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/utility/get.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <vector>

#include "day8.h"

using ranges::subrange;
using ranges::to_vector;
using ranges::views::transform;

namespace aoc {
using TreeHeight = int;
using TreeHeightsMatrix = std::vector<std::vector<TreeHeight>>;

auto GetTreeHeight [[nodiscard]] (TreeHeight tree) { return std::abs(tree); }

void MarkTreeAsVisible(TreeHeight &tree) { tree = -GetTreeHeight(tree); }

auto IsTreeVisible [[nodiscard]] (TreeHeight tree) { return tree < 0; }

auto IsTreeHigher [[nodiscard]] (TreeHeight left, TreeHeight right) {
  return GetTreeHeight(left) > GetTreeHeight(right);
}

template <MatrixTraits Traits>
class MarkTreesVisibleFromSides {
 public:
  auto operator() [[nodiscard]] (TreeHeightsMatrix &tree_matrix) {
    tree_matrix_ = &tree_matrix;

    const auto num_rows = Traits::GetNumRows(*tree_matrix_);
    num_cols_ = Traits::GetNumCols(*tree_matrix_);

    for (row_ = 0; row_ < num_rows; ++row_) {
      first_or_last_row_ = (row_ == 0) || (row_ == (num_rows - 1));

      MarkFromBeginTillMax();

      if (first_or_last_row_) {
        continue;
      }

      MarkFromEndTillMax();
    }

    tree_matrix_ = nullptr;

    return num_visible_trees_;
  }

  void MarkTreeAsVisibleIfNotAlready(TreeHeight &tree) {
    if (!IsTreeVisible(tree)) {
      MarkTreeAsVisible(tree);
      ++num_visible_trees_;
    }
  }

  void MarkFromBeginTillMax() {
    assert(tree_matrix_ != nullptr);

    max_tree_ = TreeHeight{};
    max_tree_col_ = -1;

    for (auto col = 0; col < num_cols_; ++col) {
      auto &tree = Traits::GetCell(*tree_matrix_, row_, col);

      if (first_or_last_row_) {
        MarkTreeAsVisibleIfNotAlready(tree);
        continue;
      }

      if (IsTreeHigher(tree, max_tree_)) {
        MarkTreeAsVisibleIfNotAlready(tree);

        max_tree_ = GetTreeHeight(tree);
        max_tree_col_ = col;
      }
    }
  }

  void MarkFromEndTillMax() {
    assert(tree_matrix_ != nullptr);

    max_tree_ = TreeHeight{};
    const auto forward_max_tree_col = max_tree_col_;

    for (auto col = num_cols_ - 1; col > forward_max_tree_col; --col) {
      auto &tree = Traits::GetCell(*tree_matrix_, row_, col);

      if (IsTreeHigher(tree, max_tree_)) {
        MarkTreeAsVisibleIfNotAlready(tree);

        max_tree_ = GetTreeHeight(tree);
        max_tree_col_ = col;
      }
    }
  }

 private:
  TreeHeightsMatrix *tree_matrix_{};
  int num_cols_{};
  int row_{};
  bool first_or_last_row_{};
  TreeHeight max_tree_{};
  int max_tree_col_{};
  int num_visible_trees_{};
};
}  // namespace aoc

auto main(int /*unused*/, const char *const *args) -> int {
  auto file_stream = std::ifstream{args[1]};

  const auto strings = subrange{std::istream_iterator<std::string>{file_stream},
                                std::istream_iterator<std::string>{}};
  auto tree_matrix = strings | transform([](const auto &string) {
                       return string | transform([](auto character) {
                                return std::stoi(std::string{character}) + 1;
                              }) |
                              to_vector;
                     }) |
                     to_vector;

  const auto num_visible_trees =
      aoc::MarkTreesVisibleFromSides<aoc::RowFirstTraversal>{}(tree_matrix) +
      aoc::MarkTreesVisibleFromSides<aoc::ColumnFirstTraversal>{}(tree_matrix);

  std::cout << num_visible_trees << "\n";
}