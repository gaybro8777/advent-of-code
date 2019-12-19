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
#include <queue>

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

class KeySet final {
  auto tie() const noexcept { return std::tie (bitfield); }

public:
  KeySet() = default;

  auto operator-=(const char c) noexcept -> auto&{
    bitfield &= ~(1 << (c - 'a'));
    return *this;
  }

  auto operator+=(const char c) noexcept -> auto&{
    bitfield |= (1 << (c - 'a'));
    return *this;
  }

  auto operator-(const char c) const noexcept {
    auto copy{*this};
    return copy -= c;
  }

  auto operator+(const char c) const noexcept {
    auto copy{*this};
    return copy += c;
  }

  auto contains(const char c) const noexcept {
    return bitfield & (1 << (c - 'a'));
  }

  template <typename Fn>
  auto forEach (Fn&& fn) const noexcept {
    constexpr auto bits = sizeof(bitfield) * 8;

    for (auto c = 'a'; c <= 'z'; ++c) {
      if (contains(c))
        fn(c);
    }
  }

  auto operator<(const KeySet& k) const noexcept { return tie() < k.tie(); }

  auto empty() const noexcept { return bitfield == 0; }

private:
  explicit KeySet(size_t bitfield) : bitfield(bitfield) {}

  size_t bitfield{};
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

  auto compute(const KeySet &remainingKeys, CharPair pair) const
      -> std::optional<size_t> {
    if (const auto it = memory.find(pair); it != memory.cend()) {
      const auto &requiredKeys = it->second.requiredKeys;
      if (std::all_of(requiredKeys.cbegin(), requiredKeys.cend(), [&](auto c) {
            return ! remainingKeys.contains(c);
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

auto part1v2(char currentKey,
    const MemoizedStepsToKey& memoized,
    const KeySet keys,
    std::map<std::tuple<char, KeySet>, size_t>& cache) {
  if (keys.empty())
    return size_t{};

  const auto cacheKey = std::tuple(currentKey, keys);

  if (const auto it = cache.find(cacheKey); it != cache.cend())
    return it->second;

  auto result = std::numeric_limits<size_t>::max();

  keys.forEach ([&] (auto key) {
    if (const auto dist = memoized.compute(keys, {currentKey, key}))
      result =
          std::min(result, *dist + part1v2(key, memoized, keys - key, cache));
    });

  cache[cacheKey] = result;
  return result;
}

} // namespace

// Calculate min distances between each pair of keys, and doors in between
// Then, the memoized steps thing can check whether the key requirement is met,
// and return the min distance if so, and nullopt if not.

TEST_CASE("day18") {
  constexpr char input[] = R"(#################################################################################
#.......#.......#..q..W...#..n......F...#.......#...#...#.....#.....#.#.......#.#
#.#######.###.###.#####.#.#.###########.#.#####.#.#.#.###.###.#.###.#.#.#.###.#.#
#..x........#.....#...#.#.#.#.........#.#...#...#.#...#...#.#.....#.#...#.#...#.#
###########.#######.#.#.#.###.#######.#.#.#.#.###.#####.###.#######.#.###.#.###.#
#...#...#.......#...#...#.#...#.....#.#.#.#.#.....#...#...#.#.......#.#..b#.#...#
#.#.#.#.#######.#.#######.#.###.###.#.#.###.#####.#.#.###.#.#.#######.#.###.#.#.#
#.#...#...#.....#.#.....#.#.#...#.#.#...#...#.....#.#...#...#.....#.#.#.#...#.#.#
#.#######.#######.#.#####.#.#.###.#.###.#.#########.###.###.#####.#.#.#.###.###.#
#.......#...#...#.#.........#...#...#...#.#...........#.#.......#.#...#...#.#...#
#.#####.###.#.#.#Z#############.#.#####.#.#.###########.#.#######.#.#####.#.#.#.#
#.#.....#.#.#.#...#...#.........#.#...#.#.#.....#.......#...#.....#.#...#.#...#.#
#.#.#####.#.#.#####.#.#.#########.#.#.#.#.#####.#.#######.###.#####.#.#.#.#####.#
#.#.#c..#...#...#...#.#.........#...#.#.#.#.....#.......#.#...#.#...#.#.#...#...#
###.#.#.#.#####.#.###.#####.###.#####.###.#.###########.#.#.###.#.###.#.###.#.###
#.G.#.#...#...#.#.#...#...#.#...#...#...#.#.....#.......#.#...#.#...#.#.#...#.#.#
#.###.#####.#.#.#.#.###.#.###.#####.###.#.#.#####.#####.#.###.#.###.###.#.###.#.#
#.#.........#.#.#.#.....#...#.#.....#...#...#...#...#...#.#.#.#...#.....#k..#.#.#
#.###########.#.#.#########.#.#.#####O#######.#.#.#.#.###.#.#.###.#####.###.#.#.#
#.#...#...#i....#...#.....#...#.....#.#.#.....#.#.#.#...#...#.#...#...#.#.D.#.#.#
#.#.#.#.#.#####.###.###.#.#####.###.#.#.#.#####.###.###.###.#.#.#.#.#.#.#.###.#.#
#.#.#...#.#...#.#.#...#.#...#...#...#...#...#.......#...#...#.#.#.#.#...#.#...#.#
#.#.#####.#.#.#.#.#####.###.#.###.#####.#.#.#############.#####.###.#####.#.###.#
#.#.....#...#.#...#...#.#...#...#.....#.#.#.......#.......#.....#...#.....#.#...#
#.#####.#####.###.#.#.#.#.#########.#.#.#.#######.#.#######.#####.###.#####.#.#.#
#.....#.#.....#.....#...#.........#.#.#.#.#.....#.#...#.........#...#...#.....#.#
#.###.#.#.###.###################.###.#.###.###.#.###.#.#####.#.###.###.#########
#.#.B.#.#.U.#.#.....#.......#...#.....#.#...#.....#...#.#...#.#...#...#...#.....#
###.###.###.#.#.###.#.#.#####.#.###.###.#.#######.#.#####.#.#####.###.###.#.###.#
#...#...#...#.#..a#.#.#.......#.....#...#...#...#.#.......#.....#......h#...#.#.#
#.#.#.###.#######.#.#################.###.#.#.#.#.#############.#########.###J#.#
#.#.#.#.#.#...#...#.#.......#.......#...#.#...#.#.#.....#.....#.....#...#.....#.#
#.###.#.#.#.#.#.###.#.#####.#.#####.###.#.#####T###.###.#.###.#####.#.#.#####.#.#
#...#...#...#...#.#.......#.#.....#..y..#.#...#...#.#.#.....#.....#...#.#.....#.#
#.#.###.#########.#########.#####.#########.#.###.#.#.#######.###.#####.#######.#
#.#...#.#.....#.....#.#.........#...#...#...#.....#.......#...#...#...#...#.....#
#.###.#.#.#.#.###.#.#.#.#.#########.###.#.###.###########.#.#######.#.###.#.###.#
#...#...#.#.#.#...#...#.#.#.......#...#.#...#.#.........#.#.#.......#u..#.R.#.#.#
###.#######.#.#.#####.#.###.###.#####.#.#.#.###.#######.#.#.#A#########.#####.#.#
#...........#...#.....#.......#...........#...........#...#...........#.........#
#######################################.@.#######################################
#.#...........#.......#...#.....#...#.......#...........#...............#.......#
#.#.#######.#.###.###.#.#.###.###.#.#.#.#.###.#####.#.###.###.#.#######.###.###.#
#...#r......#.....#.#...#...#.....#.#.#.#.#...#...#.#.#...#.#.#.#...#.......#...#
#####.#############.#######.###.###.#.#.#.#.#####.#.###.###.#.###.#.#############
#j..#.#.......#...........#...#...#...#.#.#.....#.#...#.....#.....#.#.......#...#
#.#.#.#####.#.#.#######.#.###.#########.#.#####.#.###.#####.#######.#.#####.#.#.#
#.#...#..d#.#...#.....#.#...#.#.....#...#.....#.#...#...#...#....m#.#.#e..#...#.#
#.#####.#.#.#####.###.#.###.#.#.###.#.###.#####.#.###.###.###.###.#.#.#.#.###.#.#
#.#.....#.#.....#.M.#.#...#.....#...#...#.#...#...#...#...#.#...#.#.#.#.#...#.#.#
#.#.#####.#####.###.#.###.#######.#####.#.#.#.###.#.###.###.#.###.#.#.#.###.###.#
#.....#...#...#...#.#...#.#...#.......#.#...#.....#.#...#...#.#...#...#.#.....V.#
#.#####.###.#.###.#.###.###.#.#######.#.#.#########.#.###.#.#.#.###.###.#######.#
#.#.#...#...#.#...#.#.S.....#.....#.#.#.#t#.........#...#.#.#.#.#.#.#g#...#.....#
#.#.#.###.###.#.###.#############.#.#.#.###.#######.###.#.#.#.#.#.#.#.###.#####.#
#...#.#...#...#...#...#...#...#...#...#.#...#.....#...#.#.#.#s#.#.#.....#.#...#.#
#####.#####.#####.###.###.#.#.#.###.###.#.###.###.#####.#.###.#.#.#####.#.#.#.#.#
#.....#...#.#...#...#.#.N.#.#.#.#.#.....#.....#.#.#.......#...#.#...#...#...#.#.#
#.###.#.#.#.#.#.#.###.#.###.#.#.#.#####.#.#####.#.#.#######.###.#.###.#######.###
#.#...#.#.#...#...#...#.....#p..#.....#.#.#.....#.#.#...P...#l..#.#...#.....#...#
#K#####.#.#.#######L###.#########.###.#.#.###.#.#.#.#.#######.###.#.###.#####.#.#
#.#.....#.#.#.....#...#...#.....Y.#.#.#.#.....#.#.#.#.#.#...#.#...#.....#...#.#.#
#.#.#####.#.#####.#######.#######.#.#.#.#.#######.#.#.#.#.#.###.#.#####.#.#.###.#
#.....#...#...#.......#.#...#...#o..#...#.#.....#...#.#.#.#.#...#.......#.#.....#
#######.#####.#.#####.#.###.#.#.#########.#.###.#####.#.#.#.#.###########.#####.#
#.......#.....#.#.........#...#.........#.#...#.......#...#.........#.....#...#.#
#.#######.#####.#.#####################.#.###.#######################.#####.###.#
#....v..#.......#.#...........#...#.....#...#.#.........#.............#.#.C.....#
#.#####.###########.#########.#.#.#.#########.###.#####.#.#############.#.#######
#.#...#.....#.......#.........#.#...#...#.....#...#.....#.......#.......#.....#.#
#.#.#.#####.#.#######.#######.#.#####.#.#.#####.###.#####.#####.#.#####.#####.#.#
#...#.#...#...#.....#.....#...#.#.....#.#....f..#.#.Q...#.#.....#.#...#.....#...#
#####.###.#######.#####.#.#####.#.###.###########.#####.###.#######.#.#.###.###.#
#.....#.....#...#.#...#.#...#...#...#...#.#...........#...#.#...#...#...#.....#.#
#.#####.###.#.#.#.#.#.#####.#.#########.#.#.#####.###.###.#.#.#.#.#######.#####.#
#...#.H.#.....#.#.#.#.....#.#.#...#...#.#...#...#.#.....#.#...#.#...#.....#...#.#
###.#.#.#######.#.#.#####.#.#.#.#.#.#.#.#.#####.#.#######.#####.###.#######.#.#.#
#.#.#.#.....#.#.#.#.....#...#...#...#.#.#.#.....#.......#.......#..w#......z#.I.#
#.#.#######.#.#.#.#####.#############.#.#.###.#X#######.#########.###.###########
#.......E...#.........#.................#.....#.......#.........................#
#################################################################################
)";

  Map map(input);
  MemoizedStepsToKey memoized{map, '@'};
  std::map<std::tuple<char, KeySet>, size_t> cache;

  KeySet keySet;
  for (const auto& key : map.remainingKeys())
    keySet += key;

  std::cout << part1v2('@', memoized, keySet, cache) << '\n';
}
