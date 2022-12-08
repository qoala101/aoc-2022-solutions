#include <cassert>
#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <iterator>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/functional/arithmetic.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/dangling.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>
#include <variant>
#include <vector>

using ranges::accumulate;
using ranges::find_if;
using ranges::for_each;
using ranges::subrange;
using ranges::views::transform;

namespace aoc {
struct Dir;

struct File {
  auto GetSize [[nodiscard]] () const { return size; }

  void Accept(auto &visitor) const { visitor(*this); }

  std::string name{};
  int size{};

  Dir *parent{};
};

struct Dir {
  auto GetSize [[nodiscard]] () const -> int {
    const auto child_sizes =
        children | transform([](const auto &child) {
          return std::visit([](const auto &child) { return child.GetSize(); },
                            child);
        });

    return accumulate(child_sizes, 0);
  }

  template <typename T>
  void AddChild(auto &&child) {
    assert(child.parent == nullptr);
    assert(FindDirectChild(child.name) == nullptr);

    child.parent = this;
    children.emplace_back(std::forward<T>(child));
  }

  auto FindDirectChild [[nodiscard]] (const std::string &name)
  -> std::variant<Dir, File> * {
    const auto child = find_if(children, [&name](const auto &child) {
      return std::visit(
          [&name](const auto &child) { return child.name == name; }, child);
    });

    if (child == children.end()) {
      return nullptr;
    }

    return &*child;
  }

  void Accept(auto &visitor) const {
    visitor(*this);

    for (const auto &child : children) {
      std::visit([&visitor](const auto &child) { child.Accept(visitor); },
                 child);
    }
  }

  std::string name{};
  std::vector<std::variant<Dir, File>> children{};

  Dir *parent{};
};

enum class ParserState {
  ExpectsCommand,
  CdExpectsDir,
  LsExpectsFileListOrDollar,
  DirExpectsDirName,
  FileSizeExpectsFileName
};

struct Parser {
  // NOLINTNEXTLINE(*-function-cognitive-complexity)
  void ParseNextWord(const std::string &word) {
    switch (state) {
      case ParserState::ExpectsCommand: {
        if (word == "cd") {
          state = ParserState::CdExpectsDir;
          return;
        }

        if (word == "ls") {
          state = ParserState::LsExpectsFileListOrDollar;
          return;
        }

        assert(false);
      }
      case ParserState::CdExpectsDir: {
        state = ParserState::LsExpectsFileListOrDollar;

        if (word == "/") {
          current_dir = &root;
          return;
        }

        if (word == "..") {
          current_dir = current_dir->parent;
          return;
        }

        assert(current_dir != nullptr);
        auto *child_dir = current_dir->FindDirectChild(word);

        if (child_dir == nullptr) {
          current_dir->AddChild<Dir>(Dir{.name = word});
        }

        child_dir = current_dir->FindDirectChild(word);
        assert(child_dir != nullptr);
        assert(std::holds_alternative<Dir>(*child_dir));

        current_dir = &std::get<Dir>(*child_dir);

        return;
      }
      case ParserState::LsExpectsFileListOrDollar: {
        if (word == "$") {
          state = ParserState::ExpectsCommand;
          return;
        }

        if (word == "dir") {
          state = ParserState::DirExpectsDirName;
          return;
        }

        current_file_size = std::stoi(word);
        state = ParserState::FileSizeExpectsFileName;
        return;
      }
      case ParserState::DirExpectsDirName: {
        assert(current_dir != nullptr);
        current_dir->AddChild<Dir>(Dir{.name = word});
        state = ParserState::LsExpectsFileListOrDollar;
        return;
      }
      case ParserState::FileSizeExpectsFileName: {
        assert(current_dir != nullptr);
        current_dir->AddChild<File>(
            File{.name = word, .size = current_file_size});
        state = ParserState::LsExpectsFileListOrDollar;
        return;
      }
    }
  }

  Dir root{.name = "/"};

  ParserState state{ParserState::LsExpectsFileListOrDollar};
  Dir *current_dir{};
  int current_file_size = 0;
};

struct DirSizesSumVisitor {
  void operator()(const File & /*unused*/) {}

  void operator()(const Dir &dir) {
    const auto dir_size = dir.GetSize();

    // NOLINTNEXTLINE(*-magic-numbers)
    if (dir_size <= 100000) {
      dir_sizes_sum += dir_size;
    }
  }

  int dir_sizes_sum{};
};
}  // namespace aoc

auto main(int /*unused*/, const char *const *args) -> int {
  auto file_stream = std::ifstream{args[1]};

  const auto strings = subrange{std::istream_iterator<std::string>{file_stream},
                                std::istream_iterator<std::string>{}};

  auto parser = aoc::Parser{};

  for_each(strings,
           [&parser](const auto &string) { parser.ParseNextWord(string); });

  auto visitor = aoc::DirSizesSumVisitor{};
  parser.root.Accept(visitor);

  std::cout << visitor.dir_sizes_sum << "\n";
}