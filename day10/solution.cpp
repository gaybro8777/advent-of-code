#include <catch2/catch.hpp>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

namespace {

const auto pi = std::acos(-1.0);

struct Coord final {
  int64_t x{}, y{};
};

std::ostream &operator<<(std::ostream &os, const Coord &c) {
  return os << '(' << c.x << ',' << c.y << ')';
}

auto tie(const Coord &c) { return std::tie(c.x, c.y); }
auto operator<(const Coord &a, const Coord &b) { return tie(a) < tie(b); }
auto operator==(const Coord &a, const Coord &b) { return tie(a) == tie(b); }
auto operator!=(const Coord &a, const Coord &b) { return tie(a) != tie(b); }

auto operator+=(Coord &a, const Coord &b) -> auto & {
  a.x += b.x;
  a.y += b.y;
  return a;
}

auto operator-=(Coord &a, const Coord &b) -> auto & {
  a.x -= b.x;
  a.y -= b.y;
  return a;
}

auto operator+(const Coord &a, const Coord &b) {
  auto copy{a};
  copy += b;
  return copy;
}

auto operator-(const Coord &a, const Coord &b) {
  auto copy{a};
  copy -= b;
  return copy;
}

auto magSq(const Coord &a) { return a.x * a.x + a.y * a.y; }

template <typename Range> auto parseCoords(Range &&range) {
  int64_t x{}, y{};
  std::set<Coord> result;

  for (auto c : range) {
    switch (c) {
    case '\n':
      x = 0;
      y += 1;
      break;
    case '#':
      result.insert({x++, y});
      break;
    default:
      x += 1;
      break;
    }
  }

  return result;
}

template <typename T> auto gcd(T a, T b) {
  while (b != 0)
    a = std::exchange(b, a % b);
  return a;
}

auto angle(Coord t) {
  return std::fmod(2 * pi + std::atan2(-t.x, t.y), 2 * pi);
}

auto relativeMap(const std::set<Coord> &coords, Coord target) {
  std::map<double, std::vector<Coord>> map;

  for (auto c : coords) {
    if (c != target) {
      const auto d = target - c;
      map[angle(d)].emplace_back(d);
    }
  }

  for (auto &[angle, vec] : map) {
    std::sort(vec.begin(), vec.end(),
              [](auto a, auto b) { return magSq(a) < magSq(b); });
  }

  return map;
}

auto find200th(std::map<double, std::vector<Coord>> map, Coord target) {
  auto it = map.begin();
  for (auto i = 0; i != 199; ++i) {
    it->second.erase(it->second.begin());

    if (it->second.empty())
      it = map.erase(it);
    else
      it = std::next(it);

    if (it == map.end())
      it = map.begin();
  }

  return target - it->second.front();
}

auto countVisible(const std::set<Coord> &coords, Coord target) {
  std::set<double> uniqueIncrements;
  for (auto other : coords)
    if (other != target)
      uniqueIncrements.emplace(angle(target - other));
  return uniqueIncrements.size();
}

constexpr char input[] = R"(#..#.#.#.######..#.#...##
##.#..#.#..##.#..######.#
.#.##.#..##..#.#.####.#..
.#..##.#.#..#.#...#...#.#
#...###.##.##..##...#..#.
##..#.#.#.###...#.##..#.#
###.###.#.##.##....#####.
.#####.#.#...#..#####..#.
.#.##...#.#...#####.##...
######.#..##.#..#.#.#....
###.##.#######....##.#..#
.####.##..#.##.#.#.##...#
##...##.######..##..#.###
...###...#..#...#.###..#.
.#####...##..#..#####.###
.#####..#.#######.###.##.
#...###.####.##.##.#.##.#
.#.#.#.#.#.##.#..#.#..###
##.#.####.###....###..##.
#..##.#....#..#..#.#..#.#
##..#..#...#..##..####..#
....#.....##..#.##.#...##
.##..#.#..##..##.#..##..#
.##..#####....#####.#.#.#
#..#..#..##...#..#.#.#.##)";

} // namespace

TEST_CASE("day10") {
  const auto coords = parseCoords(input);
  const auto [mostVisible, pos] =
      std::accumulate(coords.cbegin(), coords.cend(),
                      std::tuple{size_t{0}, Coord{}}, [&](auto acc, auto c) {
                        const auto visible = countVisible(coords, c);
                        if (visible < std::get<0>(acc))
                          return acc;
                        return std::tuple{visible, c};
                      });
  REQUIRE(mostVisible == 253);
  REQUIRE(find200th(relativeMap(coords, pos), pos) == Coord{8, 15});
}

