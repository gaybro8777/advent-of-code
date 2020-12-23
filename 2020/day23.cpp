#include <catch2/catch.hpp>

#include <charconv>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace {
auto play(std::vector<int> &input, int min, int max) {
  std::vector const removed(input.cbegin() + 1, input.cbegin() + 4);
  input.erase(input.cbegin() + 1, input.cbegin() + 4);

  for (auto dest = input.front() - 1;; --dest) {
    if (dest < min)
      dest = max;

    if (std::find(removed.cbegin(), removed.cend(), dest) == removed.cend()) {
      auto const jt = std::find(input.cbegin() + 1, input.cend(), dest);
      input.insert(jt + 1, removed.cbegin(), removed.cend());
      break;
    }
  }

  input.push_back(input.front());
  input.erase(input.cbegin());
}

auto play(std::vector<int> copy, int steps) {
  auto const min = *std::min_element(copy.cbegin(), copy.cend());
  auto const max = *std::max_element(copy.cbegin(), copy.cend());

  for (auto i = 0; i != steps; ++i)
    play(copy, min, max);

  auto const start = std::find(copy.cbegin(), copy.cend(), 1);
  std::vector const removed(copy.cbegin(), start);
  copy.erase(copy.cbegin(), start + 1);
  copy.insert(copy.cend(), removed.cbegin(), removed.cend());

  int i = 0;

  for (auto const c : copy) {
    i *= 10;
    i += c;
  }

  return i;
}
} // namespace

TEST_CASE("day23") {
  std::vector const input{1, 9, 3, 4, 6, 7, 2, 5, 8};

  auto const a = play(input, 100);

  auto const b = [&] {
    auto copy = input;
    copy.resize(1'000'000);
    std::iota(copy.begin() + input.size(), copy.end(), input.size() + 1);
    return play(copy, 10'000'000);
  }();

  std::cout << a << ' ' << b << '\n';
}
