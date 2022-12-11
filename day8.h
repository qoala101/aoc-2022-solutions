#ifndef DAY8_H_
#define DAY8_H_

#include <cassert>
#include <vector>

namespace aoc {
template <typename T>
concept MatrixTraits =
    requires(const std::vector<std::vector<int>> const_matrix,
             std::vector<std::vector<int>> matrix, int row, int col) {
      { T::GetNumRows(const_matrix) } -> std::same_as<int>;
      { T::GetNumCols(const_matrix) } -> std::same_as<int>;
      { T::GetCell(matrix, row, col) } -> std::same_as<int &>;
      true;
    };

struct RowFirstTraversal {
  static auto GetNumRows
      [[nodiscard]] (const std::vector<std::vector<int>> &matrix) {
    return static_cast<int>(matrix.size());
  }

  static auto GetNumCols
      [[nodiscard]] (const std::vector<std::vector<int>> &matrix) {
    assert(!matrix.empty());
    return static_cast<int>(matrix[0].size());
  }

  static auto GetCell [[nodiscard]] (std::vector<std::vector<int>> &matrix,
                                     int row, int col) -> auto & {
    assert((row >= 0) && (row < GetNumRows(matrix)));
    assert((col >= 0) && (col < GetNumRows(matrix)));
    return matrix[row][col];
  }
};

struct ColumnFirstTraversal {
  static auto GetNumRows
      [[nodiscard]] (const std::vector<std::vector<int>> &matrix) {
    assert(!matrix.empty());
    return static_cast<int>(matrix[0].size());
  }

  static auto GetNumCols
      [[nodiscard]] (const std::vector<std::vector<int>> &matrix) {
    return static_cast<int>(matrix.size());
  }

  static auto GetCell [[nodiscard]] (std::vector<std::vector<int>> &matrix,
                                     int row, int col) -> auto & {
    assert((row >= 0) && (row < GetNumRows(matrix)));
    assert((col >= 0) && (col < GetNumRows(matrix)));
    return matrix[col][row];
  }
};
}  // namespace aoc

#endif  // DAY8_H_
