#include "coord.hpp"
#include "direction2d.hpp"
#include "scopeguard.hpp"

#include <catch2/catch.hpp>

#include <array>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>

namespace {

constexpr auto isKey(char c) { return 'a' <= c && c <= 'z'; }

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

  auto unlock(char c) {
    for (const auto t : std::array{c, char(std::toupper(c))})
      std::replace(storage.begin(), storage.end(), t, '.');
  }

  auto remainingKeys() const noexcept {
    std::set<char> result;

    for (const auto &c : storage)
      if (isKey(c))
        result.insert(c);

    return result;
  }

private:
  std::string storage;
  int64_t width{};
};

struct RequiredKeysAndDistance final {
  std::set<char> requiredKeys;
  size_t distance{};
};

auto operator<(const RequiredKeysAndDistance &a,
               const RequiredKeysAndDistance &b) {
  return a.distance < b.distance;
}

auto computeRequiredKeysAndDistance(const Map &map, reuk::Coord start,
                                    size_t steps,
                                    std::set<reuk::Coord> &visited,
                                    std::set<char> &heldKeys)
    -> std::map<char, RequiredKeysAndDistance> {
  if (visited.find(start) != visited.cend())
    return {};

  const auto tile = map.get(start);

  if (!tile || *tile == '#')
    return {};

  std::map<char, RequiredKeysAndDistance> result;

  if (tile && isKey(*tile))
    result.emplace(*tile, RequiredKeysAndDistance{heldKeys, steps});

  const auto withLock = [&](auto &&fn) {
    if (std::isupper(*tile))
      fn(std::tolower(*tile));
  };

  withLock([&](auto key) { heldKeys.insert(key); });
  visited.insert(start);

  const reuk::ScopeGuard scope{[&] {
    withLock([&](auto key) { heldKeys.erase(key); });
    visited.erase(start);
  }};

  for (const auto d : reuk::directions2d) {
    const auto out = computeRequiredKeysAndDistance(
        map, start + toCoord(d), steps + 1, visited, heldKeys);

    for (const auto &[key, value] : out) {
      if (const auto it = result.find(key); it != result.cend())
        it->second = std::min(it->second, value);
      else
        result.emplace(key, value);
    }
  }

  return result;
}

struct CharPair final {
  char from{}, to{};
};

auto tie(const CharPair &c) { return std::tie(c.from, c.to); }
auto operator<(const CharPair &a, const CharPair &b) { return tie(a) < tie(b); }

class MemoizedStepsToKey final {
public:
  explicit MemoizedStepsToKey(const Map &map, char start) {
    addToMemory(map, start);

    for (const auto &from : map.remainingKeys())
      addToMemory(map, from);
  }

  auto compute(const std::set<char> &ownedKeys, CharPair pair) const
      -> std::optional<size_t> {
    if (const auto it = memory.find(pair); it != memory.cend()) {
      const auto &requiredKeys = it->second.requiredKeys;
      if (std::all_of(requiredKeys.cbegin(), requiredKeys.cend(), [&](auto c) {
            return ownedKeys.find(c) != ownedKeys.cend();
          })) {
        return it->second.distance;
      }

      return {};
    }

    return {};
  }

private:
  void addToMemory(const Map &map, char start) {
    std::set<reuk::Coord> visitedPos;
    std::set<char> visitedKeys;
    for (const auto &[dest, info] : computeRequiredKeysAndDistance(
             map, map.get(start), 0, visitedPos, visitedKeys)) {
      if (start != dest) {
        memory.emplace(CharPair{start, dest}, info);
        memory.emplace(CharPair{dest, start}, info);
      }
    }
  }

  std::map<CharPair, RequiredKeysAndDistance> memory;
};

auto part1(const std::set<char> &allKeys, char start, size_t stepsSoFar,
           const MemoizedStepsToKey &memoized, std::set<char> &ownedKeys)
    -> std::optional<size_t> {
  if (ownedKeys == allKeys)
    return stepsSoFar;

  const auto remainingKeys = [&] {
    std::set<char> ret;
    std::set_difference(allKeys.cbegin(), allKeys.cend(), ownedKeys.cbegin(),
                        ownedKeys.cend(), std::inserter(ret, ret.begin()));
    return ret;
  }();

  std::optional<size_t> result;

  for (const auto key : remainingKeys) {
    const auto dist = memoized.compute(ownedKeys, {start, key});

    if (!dist)
      continue;

    ownedKeys.insert(key);
    const reuk::ScopeGuard scope{[&] { ownedKeys.erase(key); }};

    if (const auto totalSteps =
            part1(allKeys, key, stepsSoFar + *dist, memoized, ownedKeys)) {
      result = [&] {
        if (result)
          return std::min(*result, *totalSteps);
        return *totalSteps;
      }();
    }
  }

  return result;
}

} // namespace

// Calculate min distances between each pair of keys, and doors in between
// Then, the memoized steps thing can check whether the key requirement is met,
// and return the min distance if so, and nullopt if not.

TEST_CASE("day18") {
  constexpr char input[] = R"(#################
#i.G..c...e..H.p#
########.########
#j.A..b...f..D.o#
########@########
#k.E..a...g..B.n#
########.########
#l.F..d...h..C.m#
#################
)";

  Map map(input);
  MemoizedStepsToKey memoized{map, '@'};
  std::set<char> ownedKeys;
  std::cout
      << part1(map.remainingKeys(), '@', 0, memoized, ownedKeys).value_or(0)
      << '\n';
}
