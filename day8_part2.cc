#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <iterator>
#include <range/v3/algorithm/max.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/comparisons.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/utility/get.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>
#include <vector>

#include "day8.h"

using ranges::max;
using ranges::subrange;
using ranges::to_vector;
using ranges::views::iota;
using ranges::views::reverse;
using ranges::views::transform;

namespace aoc {
using TreeHeightsMatrix = std::vector<std::vector<int>>;
using VisibilityMatrix = std::vector<std::vector<int>>;

template <MatrixTraits Traits>
class CalculateVisibilitiesFromSides {
 private:
  void CalculateVisibilityInRange(int row, const auto &col_range) {
    num_visible_trees_ = 0;

    for (auto col : col_range) {
      const auto other_tree_height =
          Traits::GetCell(*tree_heights_matrix_, row, col);
      ++num_visible_trees_;

      if (other_tree_height >= tree_height_) {
        break;
      }
    }

    (*visibility_) *= num_visible_trees_;
  }

  void CalculateVisibilityForCurrentTree() {
    tree_height_ = Traits::GetCell(*tree_heights_matrix_, row_, col_);
    visibility_ = &Traits::GetCell(*visibility_matrix_, row_, col_);

    if (col_ > 0) {
      CalculateVisibilityInRange(row_, iota(0, col_) | reverse);
    }

    if (col_ < (num_cols_ - 1)) {
      CalculateVisibilityInRange(row_, iota(col_ + 1, num_cols_));
    }

    visibility_ = nullptr;
  }

 public:
  void operator()(TreeHeightsMatrix &tree_heights_matrix,
                  VisibilityMatrix &visibility_matrix) {
    tree_heights_matrix_ = &tree_heights_matrix;
    visibility_matrix_ = &visibility_matrix;

    const auto num_rows_ = Traits::GetNumRows(*tree_heights_matrix_);
    num_cols_ = Traits::GetNumCols(*tree_heights_matrix_);

    for (row_ = 0; row_ < num_rows_; ++row_) {
      for (col_ = 0; col_ < num_cols_; ++col_) {
        CalculateVisibilityForCurrentTree();
      }
    }

    tree_heights_matrix_ = nullptr;
    visibility_matrix_ = nullptr;
  }

 private:
  TreeHeightsMatrix *tree_heights_matrix_{};
  VisibilityMatrix *visibility_matrix_{};
  int num_cols_{};
  int row_{};
  int col_{};
  int num_visible_trees_{};
  int tree_height_{};
  int *visibility_{};
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
  auto visibility_matrix = tree_matrix | transform([](const auto &tree_row) {
                             return std::vector<int>(tree_row.size(), 1);
                           }) |
                           to_vector;

  aoc::CalculateVisibilitiesFromSides<aoc::RowFirstTraversal>{}(
      tree_matrix, visibility_matrix);
  aoc::CalculateVisibilitiesFromSides<aoc::ColumnFirstTraversal>{}(
      tree_matrix, visibility_matrix);

  const auto max_visibility =
      max(visibility_matrix | transform([](const auto &visibility_row) {
            return max(visibility_row);
          }));

  std::cout << max_visibility << "\n";
}