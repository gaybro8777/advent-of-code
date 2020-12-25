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
auto play(std::vector<int64_t> &input, int64_t min, int64_t max) {
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

auto play(std::vector<int64_t> copy, int64_t steps) {
  auto const min = *std::min_element(copy.cbegin(), copy.cend());
  auto const max = *std::max_element(copy.cbegin(), copy.cend());

  for (auto i = 0; i != steps; ++i)
    play(copy, min, max);

  auto const start = std::find(copy.cbegin(), copy.cend(), 1);
  std::vector const removed(copy.cbegin(), start);
  copy.erase(copy.cbegin(), start + 1);
  copy.insert(copy.cend(), removed.cbegin(), removed.cend());

  int64_t i = 0;

  for (auto const c : copy) {
    i *= 10;
    i += c;
  }

  return i;
}
} // namespace

TEST_CASE("day23") {
#if 1
  std::vector<int64_t> const input{1, 9, 3, 4, 6, 7, 2, 5, 8};
#else
  std::vector<int64_t> const input{3, 8, 9, 1, 2, 5, 4, 6, 7};
#endif

#if 0
  auto const a = play(input, int64_t{100});

  auto const b = [&] {
    auto const min = 1;
    auto const max = 1'000'000;

    auto copy = input;
    copy.push_back(copy.size() + 1);

    std::vector<int64_t> entries;
    entries.resize(max + 1, 0);

    for (auto i = 0; i != max; ++i)
      entries[i + 1] = i + 2;

    for (auto i = 0; i != copy.size() - 1; ++i)
      entries[copy[i]] = copy[i + 1];

    entries[max] = copy.front();

    auto current_cup = copy.front();

    std::vector<int64_t> removed;

    for (auto i = 0; i != 10'000'000; ++i, current_cup = entries[current_cup]) {
      removed.clear();
      auto next = current_cup;

      for (auto ind = 0; ind != 3; ++ind) {
        next = entries[next];
        removed.push_back(next);
      }

      entries[current_cup] = entries[removed.back()];

      for (auto dest = current_cup - 1;; --dest) {
        if (dest < min)
          dest = max;

        if (std::find(removed.cbegin(), removed.cend(), dest) ==
            removed.cend()) {
          auto const prev = entries[dest];
          entries[dest] = removed.front();
          entries[removed.back()] = prev;
          break;
        }
      }
    }

    auto const u = entries[1];
    auto const v = entries[u];

    return u * v;
  }();
#endif
}
