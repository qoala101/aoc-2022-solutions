cmake_minimum_required(VERSION 3.18)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  "aoc-2022-solutions"
  VERSION 1.0.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

find_program(iwyu NAMES iwyu include-what-you-use REQUIRED)
set(iwyu ${iwyu}
  -Xiwyu --no_comments
  -Xiwyu --no_fwd_decls
  -Xiwyu --cxx17ns
)
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu})

add_compile_options(
  -Wall
  -Wextra
  -pedantic
  -Wno-missing-braces
)

find_package(range-v3)

add_executable(day1 day1.cc)
target_link_libraries(day1 PRIVATE range-v3::range-v3)

add_executable(day2_part1 day2_part1.cc)
target_link_libraries(day2_part1 PRIVATE range-v3::range-v3)

add_executable(day2_part2 day2_part2.cc)
target_link_libraries(day2_part2 PRIVATE range-v3::range-v3)