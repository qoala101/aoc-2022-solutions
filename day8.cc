#include <bits/std_abs.h>

#include <cassert>
#include <concepts>
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

using ranges::subrange;
using ranges::to_vector;
using ranges::views::transform;

namespace aoc {
using Tree = int;
using TreeMatrix = std::vector<std::vector<Tree>>;

auto GetTreeHeight [[nodiscard]] (Tree tree) { return std::abs(tree); }

void MarkTreeAsVisible(Tree &tree) { tree = -GetTreeHeight(tree); }

auto IsTreeVisible [[nodiscard]] (Tree tree) { return tree < 0; }

auto IsTreeHigher [[nodiscard]] (Tree left, Tree right) {
  return GetTreeHeight(left) > GetTreeHeight(right);
}

template <typename T>
concept TreeMatrixTraits = requires(const TreeMatrix const_tree_matrix,
                                    TreeMatrix tree_matrix, int row, int col) {
                             {
                               T::GetNumRows(const_tree_matrix)
                               } -> std::same_as<int>;
                             {
                               T::GetNumCols(const_tree_matrix)
                               } -> std::same_as<int>;
                             {
                               T::GetTreeInCell(tree_matrix, row, col)
                               } -> std::same_as<Tree &>;
                             true;
                           };

template <TreeMatrixTraits Traits>
class MarkTreesVisibleFromSides {
 public:
  auto operator() [[nodiscard]] (TreeMatrix &tree_matrix) {
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

  void MarkTreeAsVisibleIfNotAlready(Tree &tree) {
    if (!IsTreeVisible(tree)) {
      MarkTreeAsVisible(tree);
      ++num_visible_trees_;
    }
  }

  void MarkFromBeginTillMax() {
    assert(tree_matrix_ != nullptr);

    max_tree_ = Tree{};
    max_tree_col_ = -1;

    for (auto col = 0; col < num_cols_; ++col) {
      auto &tree = Traits::GetTreeInCell(*tree_matrix_, row_, col);

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

    max_tree_ = Tree{};
    const auto forward_max_tree_col = max_tree_col_;

    for (auto col = num_cols_ - 1; col > forward_max_tree_col; --col) {
      auto &tree = Traits::GetTreeInCell(*tree_matrix_, row_, col);

      if (IsTreeHigher(tree, max_tree_)) {
        MarkTreeAsVisibleIfNotAlready(tree);

        max_tree_ = GetTreeHeight(tree);
        max_tree_col_ = col;
      }
    }
  }

 private:
  TreeMatrix *tree_matrix_{};
  int num_cols_{};
  int row_{};
  bool first_or_last_row_{};
  Tree max_tree_{};
  int max_tree_col_{};
  int num_visible_trees_{};
};

struct RowFirstTraversal {
  static auto GetNumRows [[nodiscard]] (const TreeMatrix &tree_matrix) {
    return static_cast<int>(tree_matrix.size());
  }

  static auto GetNumCols [[nodiscard]] (const TreeMatrix &tree_matrix) {
    assert(!tree_matrix.empty());
    return static_cast<int>(tree_matrix[0].size());
  }

  static auto GetTreeInCell
      [[nodiscard]] (TreeMatrix &tree_matrix, int row, int col) -> auto & {
    assert((row >= 0) && (row < GetNumRows(tree_matrix)));
    assert((col >= 0) && (col < GetNumRows(tree_matrix)));
    return tree_matrix[row][col];
  }
};

struct ColumnFirstTraversal {
  static auto GetNumRows [[nodiscard]] (const TreeMatrix &tree_matrix) {
    assert(!tree_matrix.empty());
    return static_cast<int>(tree_matrix[0].size());
  }

  static auto GetNumCols [[nodiscard]] (const TreeMatrix &tree_matrix) {
    return static_cast<int>(tree_matrix.size());
  }

  static auto GetTreeInCell
      [[nodiscard]] (TreeMatrix &tree_matrix, int row, int col) -> auto & {
    assert((row >= 0) && (row < GetNumRows(tree_matrix)));
    assert((col >= 0) && (col < GetNumRows(tree_matrix)));
    return tree_matrix[col][row];
  }
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