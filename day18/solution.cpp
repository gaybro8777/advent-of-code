#include "coord.hpp"

#include <catch2/catch.hpp>

#include <iostream>
#include <optional>
#include <set>
#include <string>

namespace {

class Map final {
  auto toInd(reuk::Coord c) const noexcept -> std::optional<size_t> {
    const auto result = c.x + width * c.y;

    if (c.x < 0 || width <= c.x || c.y < 0 || storage.size() <= result)
      return {};

    return result;
  }

  auto toCoord(int64_t ind) const noexcept {
    return reuk::Coord{ind % width, ind / width};
  }

public:
  explicit Map(std::string_view m)
      : storage(m), width(storage.find('\n') + 1) {}

  auto set(reuk::Coord c, char t) noexcept {
    if (const auto ind = toInd(c))
      storage[*ind] = t;
  }

  auto get(reuk::Coord c) const noexcept -> std::optional<char> {
    if (const auto ind = toInd(c))
      return storage[*ind];
    return {};
  }

  auto get(char c) const noexcept { return toCoord(storage.find(c)); }

  auto unlock(char c) { std::replace(storage.begin(), storage.end(), c, '.'); }

  auto remainingKeys() const noexcept {
    std::set<char> result;

    for (const auto &c : storage)
      if ('a' <= c && c <= 'z')
        result.insert(c);

    return result;
  }

private:
  std::string storage;
  int64_t width{};
};

auto stepsToKey(const Map &map, reuk::Coord pos, char key) -> std::optional<size_t> {
  const auto tile = map.get(pos);

  if (tile && *tile == key)
    return 0;

  if (tile != '.')
    return {};

  auto copy{map};
  copy.set(pos, '#');
}

} // namespace

// Move to starting point
// Remove the current key, if any
// Find reachable keys, and distances to them
// If there are no reachable keys, and no unreachable keys, we're done
//    return the total steps travelled
// If there are unreachable keys
//    return failure
// For each reachable key
//    run the procedure again
// Return the min steps travelled, if any

TEST_CASE("day18") {
  constexpr char input[] = R"(#########
#b.A.@.a#
#########
)";

  Map map(input);
}
