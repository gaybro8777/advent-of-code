#include <catch2/catch.hpp>

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>

namespace {
auto constexpr test_input = R"(.#.
..#
###
)";

auto constexpr input = R"(######.#
#.###.#.
###.....
#.####..
##.#.###
.######.
###.####
######.#
)";

struct Coord {
  int x{}, y{}, z{}, w{};
};

auto tie(Coord const &c) { return std::tie(c.x, c.y, c.z, c.w); }
auto operator<(Coord const &a, Coord const &b) { return tie(a) < tie(b); }
auto operator+(Coord const &a, Coord const &b) {
  return Coord{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

auto count_adjacent(std::map<Coord, bool> const &arena, Coord loc) {
  auto count = 0;

  for (auto a = -1; a != 2; ++a) {
    for (auto b = -1; b != 2; ++b) {
      for (auto c = -1; c != 2; ++c) {
        for (auto d = -1; d != 2; ++d) {
          if (a == 0 && b == 0 && c == 0 && d == 0)
            continue;

          if (auto const it = arena.find(loc + Coord{a, b, c, d});
              it != arena.cend() && it->second)
            count += 1;
        }
      }
    }
  }

  return count;
}

void step(std::map<Coord, bool> const &arena, std::map<Coord, bool> &next,
          Coord loc) {
  auto const active = [&] {
    auto const it = arena.find(loc);
    return it != arena.cend() && it->second;
  }();

  auto const count = count_adjacent(arena, loc);
  next[loc] = active ? (count == 2 || count == 3) : count == 3;
}

auto simulate_3d(std::map<Coord, bool> arena) {
  auto next = arena;

  auto min_x = 0, max_x = std::prev(arena.end())->first.x + 1;
  auto min_y = 0, max_y = std::prev(arena.end())->first.y + 1;
  auto min_z = 0, max_z = 1;

  for (auto i = 0; i != 6; ++i) {
    min_x -= 1;
    min_y -= 1;
    min_z -= 1;

    max_x += 1;
    max_y += 1;
    max_z += 1;

    for (auto x = min_x; x != max_x; ++x)
      for (auto y = min_y; y != max_y; ++y)
        for (auto z = min_z; z != max_z; ++z)
          step(arena, next, {x, y, z});

    std::swap(arena, next);
  }

  return std::count_if(arena.cbegin(), arena.cend(),
                       [](auto const &pair) { return pair.second; });
}

auto simulate_4d(std::map<Coord, bool> arena) {
  auto next = arena;

  auto min_x = 0, max_x = std::prev(arena.end())->first.x + 1;
  auto min_y = 0, max_y = std::prev(arena.end())->first.y + 1;
  auto min_z = 0, max_z = 1;
  auto min_w = 0, max_w = 1;

  for (auto i = 0; i != 6; ++i) {
    min_x -= 1;
    min_y -= 1;
    min_z -= 1;
    min_w -= 1;

    max_x += 1;
    max_y += 1;
    max_z += 1;
    max_w += 1;

    for (auto x = min_x; x != max_x; ++x)
      for (auto y = min_y; y != max_y; ++y)
        for (auto z = min_z; z != max_z; ++z)
          for (auto w = min_w; w != max_w; ++w)
            step(arena, next, {x, y, z, w});

    std::swap(arena, next);
  }

  return std::count_if(arena.cbegin(), arena.cend(),
                       [](auto const &pair) { return pair.second; });
}
} // namespace

TEST_CASE("day17") {
  auto arena = [&] {
    std::map<Coord, bool> arena;
    std::istringstream is{input};
    std::string line;
    auto y = 0;

    while (std::getline(is, line)) {
      auto x = 0;
      for (auto c : line) {
        arena[{x, y, 0}] = c == '#';
        x += 1;
      }

      y += 1;
    }

    return arena;
  }();

  auto const a = simulate_3d(arena);
  auto const b = simulate_4d(arena);

  std::cout << a << ' ' << b << '\n';
}
