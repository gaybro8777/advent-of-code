#include "direction2d.hpp"

#include <catch2/catch.hpp>

#include <array>
#include <iostream>
#include <map>
#include <optional>
#include <set>

namespace {

class Map final {
  auto toInd(reuk::Coord c) const noexcept -> std::optional<size_t> {
    const auto result = c.x + (width_ + 1) * c.y;

    if (c.x < 0 || width_ <= c.x || c.y < 0 || storage_.size() <= result)
      return {};

    return result;
  }

  auto toCoord(int64_t ind) const noexcept {
    return reuk::Coord{ind % width_, ind / width_};
  }

  auto tie() const noexcept { return std::tie(storage_, width_); }

public:
  explicit Map(std::string_view m) : storage_(m), width_(storage_.find('\n')) {}

  auto get(reuk::Coord c) const noexcept -> std::optional<char> {
    if (const auto ind = toInd(c))
      return storage_[*ind];
    return {};
  }

  auto set(reuk::Coord c, char d) {
    if (const auto ind = toInd(c))
      storage_[*ind] = d;
  }

  auto width() const { return width_; }
  auto height() const { return storage_.size() / (width_ + 1); }

  auto show(std::ostream &os) const -> auto & { return os << storage_; }

  auto swap(Map &other) noexcept {
    using std::swap;
    swap(other.storage_, storage_);
    swap(other.width_, width_);
  }

  auto operator<(const Map &other) const noexcept {
    return tie() < other.tie();
  }

  auto operator==(const Map &other) const noexcept {
    return tie() == other.tie();
  }

  auto operator!=(const Map &other) const noexcept {
    return tie() != other.tie();
  }

private:
  std::string storage_;
  int64_t width_{};
};

void swap(Map &a, Map &b) { a.swap(b); }

auto operator<<(std::ostream &os, const Map &map) -> auto & {
  return map.show(os);
}

class Sim final {
public:
  template <typename Range>
  explicit Sim(Range &&input) : current{input}, next{input} {}

  auto step() {
    const auto width = current.width();
    const auto height = current.height();

    for (auto x = 0; x != width; ++x) {
      for (auto y = 0; y != height; ++y) {
        const auto alive = current.get(reuk::Coord{x, y}) == '#';
        const auto adjacent = std::count_if(
            reuk::directions2d.cbegin(), reuk::directions2d.cend(),
            [&](auto d) {
              return current.get(reuk::Coord{x, y} + toCoord(d)) == '#';
            });

        next.set(reuk::Coord{x, y}, [&] {
          if (alive && adjacent != 1)
            return '.';
          if (!alive && 1 <= adjacent && adjacent < 3)
            return '#';
          return alive ? '#' : '.';
        }());
      }
    }

    swap(current, next);
  }

  auto get() const -> const auto & { return current; }

private:
  Map current, next;
};

auto operator==(const Sim &a, const Sim &b) { return a.get() == b.get(); }
auto operator!=(const Sim &a, const Sim &b) { return a.get() != b.get(); }

template <typename Range> auto firstRepetition(Range &&input) {
  Sim tortoise{input};
  Sim hare{input};

  do {
    tortoise.step();
    hare.step();
    hare.step();
  } while (tortoise != hare);

  auto mu = 0;
  tortoise = Sim{input};

  while (tortoise != hare) {
    tortoise.step();
    hare.step();
    mu += 1;
  }

  return hare.get();
}

auto biodiversity(const Map &m) {
  const auto width = m.width();
  const auto height = m.height();

  int64_t result{};

  for (auto x = 0; x != width; ++x) {
    for (auto y = 0; y != height; ++y) {
      if (m.get(reuk::Coord{x, y}) == '#')
        result += 1 << (x + (y * width));
    }
  }

  return result;
}

constexpr auto start = R"(#####
.#.#.
.#..#
....#
..###
)";

constexpr auto test = R"(....#
#..#.
#..##
..#..
#....
)";

//==============================================================================

struct MapIndex final {
  reuk::Coord pos;
  int64_t level{};
};

auto tie(const MapIndex &mi) { return std::tie(mi.pos, mi.level); }
auto operator<(const MapIndex &a, const MapIndex &b) { return tie(a) < tie(b); }
auto operator==(const MapIndex &a, const MapIndex &b) {
  return tie(a) == tie(b);
}
auto operator!=(const MapIndex &a, const MapIndex &b) {
  return tie(a) != tie(b);
}

auto operator<<(std::ostream &os, const MapIndex &mi) -> auto & {
  return os << mi.pos << ' ' << mi.level;
}

auto computeAdjacentCells(const MapIndex &ind) {
  constexpr auto minCoord = -2;
  constexpr auto maxCoord = 2;

  std::vector<MapIndex> result;

  for (const auto d : reuk::directions2d) {
    const auto newPos = ind.pos + toCoord(d);

    if (newPos.x == 0 && newPos.y == 0) {
      const auto increment = [&]() -> reuk::Coord {
        if (ind.pos.y == 0)
          return {0, 1};
        return {1, 0};
      }();

      const auto innerCoord = [&]() -> reuk::Coord {
        switch (d) {
        case reuk::Direction2d::up:
          return {0, maxCoord};
        case reuk::Direction2d::down:
          return {0, minCoord};
        case reuk::Direction2d::left:
          return {maxCoord, 0};
        case reuk::Direction2d::right:
          return {minCoord, 0};
        }

        return {};
      }();

      for (auto p = minCoord; p <= maxCoord; ++p)
        result.push_back(
            {innerCoord + increment * reuk::Coord{p, p}, ind.level + 1});
    } else if (newPos.x < minCoord) {
      result.push_back({{-1, 0}, ind.level - 1});
    } else if (maxCoord < newPos.x) {
      result.push_back({{1, 0}, ind.level - 1});
    } else if (newPos.y < minCoord) {
      result.push_back({{0, -1}, ind.level - 1});
    } else if (maxCoord < newPos.y) {
      result.push_back({{0, 1}, ind.level - 1});
    } else {
      result.push_back({ind.pos + toCoord(d), ind.level});
    }
  }

  return result;
}

class RecursiveSim final {
  auto getAlive(const MapIndex &d) {
    if (const auto it = current.find(d); it != current.cend())
      return it->second;
    return false;
  }

  auto getTilesToUpdate() const {
    std::set<MapIndex> result;

    for (const auto &[pos, _] : current) {
      const auto adjacentCells = computeAdjacentCells(pos);
      result.insert(adjacentCells.cbegin(), adjacentCells.cend());
    }

    return result;
  }

  auto buildMap() const {
    std::map<MapIndex, bool> levels;

    for (auto x = 0; x != initial.width(); ++x) {
      for (auto y = 0; y != initial.height(); ++y) {
        const reuk::Coord offset{x - 2, y - 2};

        if (offset.x != 0 || offset.y != 0)
          levels.emplace(MapIndex{offset, 0}, initial.get({x, y}) == '#');
      }
    }

    return levels;
  }

public:
  template <typename Range>
  explicit RecursiveSim(Range &&input) : initial{input}, current{buildMap()} {}

  auto step() {
    const auto toUpdate = getTilesToUpdate();
    for (const auto &pos : toUpdate) {
      const auto adjacentCells = computeAdjacentCells(pos);
      const auto adjacent =
          std::count_if(adjacentCells.cbegin(), adjacentCells.cend(),
                        [&](const auto d) { return getAlive(d); });
      const auto alive = getAlive(pos);

      next[pos] = [&] {
        if (alive && adjacent != 1)
          return false;
        if (!alive && 1 <= adjacent && adjacent < 3)
          return true;
        return alive;
      }();
    }

    using std::swap;
    swap(current, next);
  }

  auto count() const {
    return std::count_if(current.cbegin(), current.cend(),
                         [](const auto &p) { return p.second; });
  }

private:
  const Map initial;
  std::map<MapIndex, bool> current, next;
};

} // namespace

TEST_CASE("day24") {
  REQUIRE(biodiversity(firstRepetition(start)) == 30442557);

  RecursiveSim sim{start};

  for (auto i = 0; i != 200; ++i)
    sim.step();

  REQUIRE(sim.count() == 1987);
}
