#include "aoc_coord.hpp"

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
int64_t transform(int64_t in, int64_t subject) {
  return (in * subject) % 20201227;
}

int64_t transform(int64_t in, int64_t subject, size_t times) {
  for (size_t i = 0; i < times; ++i)
    in = transform(in, subject);

  return in;
}

int64_t get_loop_size(size_t num) {
  static std::vector<int64_t> lookup{1};

  if (auto const it = std::find(lookup.cbegin(), lookup.cend(), num);
      it != lookup.cend())
    return std::distance(lookup.cbegin(), it);

  while (lookup.back() != num)
    lookup.push_back(transform(lookup.back(), 7));

  return lookup.size() - 1;
}

} // namespace

TEST_CASE("day25") {
#if 0
  auto const a = 5764801;
  auto const b = 17807724;
#else
  auto const a = 10705932;
  auto const b = 12301431;
#endif

  auto const u = get_loop_size(a);
  auto const v = get_loop_size(b);

  auto const key1 = transform(1, b, u);
  auto const key2 = transform(1, a, v);
}
