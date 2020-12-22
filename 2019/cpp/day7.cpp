#include "aoc_intcode.hpp"

#include <catch2/catch.hpp>

#include <array>
#include <cstdint>
#include <iostream>
#include <optional>
#include <queue>
#include <stdexcept>
#include <variant>

namespace {

template <typename Program, size_t... Ind>
auto makeInterpreters(std::index_sequence<Ind...>, Program &&program,
                      const std::array<int, sizeof...(Ind)> &phases) {
  return std::array{aoc::Interpreter{program, std::array{phases[Ind]}}...};
}

template <typename Program, size_t Num>
auto makeInterpreters(Program &&program, const std::array<int, Num> &phases) {
  return makeInterpreters(std::make_index_sequence<Num>(),
                          std::forward<Program>(program), phases);
}

template <typename Program, size_t Num>
auto runAmplifiersWithFeedback(Program &&program,
                               const std::array<int, Num> &phases)
    -> std::optional<int64_t> {
  auto interpreters = makeInterpreters(program, phases);
  auto in = 0;

  for (auto it = interpreters.begin();; ++it) {
    if (it == interpreters.end())
      it = interpreters.begin();

    it->queueInputs(std::array{in});

    if (const auto result = it->runUntilOutput())
      in = *result;
    else
      break;
  }

  return in;
}

template <typename Program, size_t Num>
auto findAmplificationWithFeedback(Program &&program, std::array<int, Num> phases) {
  int64_t max = 0;

  do {
    if (const auto result = runAmplifiersWithFeedback(program, phases))
      max = std::max(max, *result);
  } while (std::next_permutation(phases.begin(), phases.end()));

  return max;
}

} // namespace

TEST_CASE("day7") {
  const auto prog = std::array{
      3,    8,    1001, 8,    10,   8,    105,  1,    0,     0,    21,   42,
      51,   60,   77,   94,   175,  256,  337,  418,  99999, 3,    9,    1001,
      9,    4,    9,    102,  5,    9,    9,    1001, 9,     3,    9,    102,
      5,    9,    9,    4,    9,    99,   3,    9,    102,   2,    9,    9,
      4,    9,    99,   3,    9,    1001, 9,    3,    9,     4,    9,    99,
      3,    9,    101,  4,    9,    9,    1002, 9,    4,     9,    101,  5,
      9,    9,    4,    9,    99,   3,    9,    1002, 9,     5,    9,    101,
      3,    9,    9,    102,  2,    9,    9,    4,    9,     99,   3,    9,
      1001, 9,    1,    9,    4,    9,    3,    9,    1002,  9,    2,    9,
      4,    9,    3,    9,    102,  2,    9,    9,    4,     9,    3,    9,
      102,  2,    9,    9,    4,    9,    3,    9,    102,   2,    9,    9,
      4,    9,    3,    9,    101,  2,    9,    9,    4,     9,    3,    9,
      101,  2,    9,    9,    4,    9,    3,    9,    101,   2,    9,    9,
      4,    9,    3,    9,    1001, 9,    1,    9,    4,     9,    3,    9,
      101,  1,    9,    9,    4,    9,    99,   3,    9,     1001, 9,    2,
      9,    4,    9,    3,    9,    101,  1,    9,    9,     4,    9,    3,
      9,    102,  2,    9,    9,    4,    9,    3,    9,     1001, 9,    1,
      9,    4,    9,    3,    9,    101,  2,    9,    9,     4,    9,    3,
      9,    1002, 9,    2,    9,    4,    9,    3,    9,     1001, 9,    1,
      9,    4,    9,    3,    9,    101,  1,    9,    9,     4,    9,    3,
      9,    1001, 9,    2,    9,    4,    9,    3,    9,     1002, 9,    2,
      9,    4,    9,    99,   3,    9,    101,  2,    9,     9,    4,    9,
      3,    9,    102,  2,    9,    9,    4,    9,    3,     9,    101,  2,
      9,    9,    4,    9,    3,    9,    102,  2,    9,     9,    4,    9,
      3,    9,    1002, 9,    2,    9,    4,    9,    3,     9,    102,  2,
      9,    9,    4,    9,    3,    9,    102,  2,    9,     9,    4,    9,
      3,    9,    101,  2,    9,    9,    4,    9,    3,     9,    1001, 9,
      2,    9,    4,    9,    3,    9,    101,  2,    9,     9,    4,    9,
      99,   3,    9,    101,  2,    9,    9,    4,    9,     3,    9,    1002,
      9,    2,    9,    4,    9,    3,    9,    1001, 9,     1,    9,    4,
      9,    3,    9,    101,  1,    9,    9,    4,    9,     3,    9,    1001,
      9,    1,    9,    4,    9,    3,    9,    1002, 9,     2,    9,    4,
      9,    3,    9,    1002, 9,    2,    9,    4,    9,     3,    9,    101,
      1,    9,    9,    4,    9,    3,    9,    102,  2,     9,    9,    4,
      9,    3,    9,    101,  2,    9,    9,    4,    9,     99,   3,    9,
      1001, 9,    2,    9,    4,    9,    3,    9,    101,   2,    9,    9,
      4,    9,    3,    9,    1002, 9,    2,    9,    4,     9,    3,    9,
      101,  2,    9,    9,    4,    9,    3,    9,    101,   2,    9,    9,
      4,    9,    3,    9,    102,  2,    9,    9,    4,     9,    3,    9,
      1002, 9,    2,    9,    4,    9,    3,    9,    102,   2,    9,    9,
      4,    9,    3,    9,    1002, 9,    2,    9,    4,     9,    3,    9,
      101,  1,    9,    9,    4,    9,    99};
  REQUIRE(findAmplificationWithFeedback(prog, std::array{0, 1, 2, 3, 4}) ==
          18812);
  REQUIRE(findAmplificationWithFeedback(prog, std::array{5, 6, 7, 8, 9}) ==
          25534964);
}

