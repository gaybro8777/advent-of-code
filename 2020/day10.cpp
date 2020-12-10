#include <catch2/catch.hpp>

#include <algorithm>
#include <iostream>
#include <map>
#include <optional>
#include <span>
#include <vector>

namespace {
template <size_t N> auto sorted(std::array<int, N> const &a) {
  auto copy = a;
  std::sort(copy.begin(), copy.end());
  return copy;
}

auto const input = sorted(std::array{
    56,  139, 42, 28, 3,   87,  142, 57,  147, 6,   117, 95,  2,  112, 107, 54,
    146, 104, 40, 26, 136, 127, 111, 47,  8,   24,  13,  92,  18, 130, 141, 37,
    81,  148, 31, 62, 50,  80,  91,  33,  77,  1,   96,  100, 9,  120, 27,  97,
    60,  102, 25, 83, 55,  118, 19,  113, 49,  133, 14,  119, 88, 124, 110, 145,
    65,  21,  7,  74, 72,  61,  103, 20,  41,  53,  32,  44,  10, 34,  121, 114,
    67,  69,  66, 82, 101, 68,  84,  48,  73,  17,  43,  140});

auto const test_input = sorted(
    std::array{28, 33, 18, 42, 31, 14, 46, 20, 48, 47, 24, 23, 49, 45, 19, 38,
               39, 11, 1,  32, 25, 35, 8,  17, 7,  9,  4,  2,  34, 10, 3});

int64_t count_combinations_memoized(std::map<size_t, int64_t> &results,
                                    std::span<int const> input, size_t pos) {
  if (auto const it = results.find(pos); it != results.cend())
    return it->second;

  if (pos >= input.size() - 1) {
    results.emplace(pos, 1);
    return 1;
  }

  int64_t result = 0;

  for (auto i = pos + 1; i < input.size() && input[i] <= input[pos] + 3; ++i)
    result += count_combinations_memoized(results, input, i);

  results.emplace(pos, result);
  return result;
}

int64_t count_combinations(std::span<int const> input, size_t pos) {
  std::map<size_t, int64_t> results;
  return count_combinations_memoized(results, input, pos);
}

} // namespace

TEST_CASE("day10") {
  auto const a = [&] {
    std::vector sorted(input.begin(), input.end());
    sorted.insert(sorted.begin(), 0);
    sorted.push_back(sorted.back() + 3);

    std::vector<int> differences;
    differences.reserve(sorted.size());

    std::transform(sorted.begin(), sorted.end() - 1, sorted.begin() + 1,
                   std::back_inserter(differences),
                   [](auto u, auto v) { return v - u; });

    return std::count(differences.begin(), differences.end(), 1) *
           std::count(differences.begin(), differences.end(), 3);
  }();

  auto const b = [&] {
    auto const &in = input;
    std::vector sorted(in.begin(), in.end());
    sorted.insert(sorted.begin(), 0);
    return count_combinations(std::span(sorted.data(), sorted.size()), 0);
  }();
}
