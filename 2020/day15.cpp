#include <catch2/catch.hpp>

#include <algorithm>
#include <iostream>
#include <map>
#include <span>

namespace {
auto play(std::span<size_t const> input, size_t steps) {
  std::map<size_t, size_t> previous;

  size_t index = 0;
  std::for_each(input.begin(), std::prev(input.end()),
                [&](auto &item) { previous[item] = index++; });

  auto last = input.back();

  for (; index < steps - 1; ++index) {
    if (auto const it = previous.find(last); it != previous.cend()) {
      last = index - it->second;
      it->second = index;
    } else {
      previous[last] = index;
      last = 0;
    }
  }

  return last;
}
} // namespace

TEST_CASE("day15") {
  size_t constexpr input[]{2, 15, 0, 9, 1, 20};

  auto const a = play(input, 2020);
  auto const b = play(input, 30000000);

  std::cout << a << ' ' << b << '\n';
}
