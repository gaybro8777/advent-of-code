#include <catch2/catch.hpp>

#include <algorithm>
#include <charconv>
#include <iostream>
#include <map>
#include <optional>
#include <span>
#include <sstream>
#include <vector>

namespace {
auto constexpr timestamp = 1014511;
std::optional<uint64_t> constexpr input[]{
    17,  {}, {}, {}, {}, {}, {}, 41, {}, {}, {}, {}, {}, {}, {},  {}, {},
    643, {}, {}, {}, {}, {}, {}, {}, 23, {}, {}, {}, {}, 13, {},  {}, {},
    {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, 29, {}, 433, {}, {},
    {},  {}, {}, 37, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, 19};

auto constexpr time_to_wait(uint64_t timestamp, uint64_t id) {
  return id - (timestamp % id);
}

auto calculate_timestamp(std::span<std::optional<uint64_t> const> in) {
  uint64_t timestamp = 0;
  uint64_t increment = 1;

  for (auto offset = 0; offset < std::size(in); ++offset) {
    if (!in[offset].has_value())
      continue;

    auto const this_id = *in[offset];

    for (auto t = timestamp;; t += increment) {
      if (time_to_wait(t, this_id) % this_id == offset % this_id) {
        timestamp = t;
        increment *= this_id;
        break;
      }
    }
  }

  return timestamp;
}

} // namespace

TEST_CASE("day13") {
  auto const a = [&] {
    auto const minutes = std::accumulate(
        std::begin(input), std::end(input), std::optional<uint64_t>{},
        [](auto acc, auto item) {
          if (!acc.has_value())
            return item;

          if (item.has_value())
            return std::optional(std::min(*acc, *item, [](auto a, auto b) {
              return time_to_wait(timestamp, a) < time_to_wait(timestamp, b);
            }));

          return acc;
        });
    return *minutes * time_to_wait(timestamp, *minutes);
  }();

  auto const b = calculate_timestamp(std::span(input, std::size(input)));
}
