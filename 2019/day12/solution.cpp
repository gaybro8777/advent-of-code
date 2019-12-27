#include <catch2/catch.hpp>

#include <array>
#include <iostream>
#include <numeric>
#include <tuple>

namespace {

struct Vec final {
  int x{}, y{}, z{};
};

constexpr auto tie(const Vec &a) { return std::tie(a.x, a.y, a.z); }
constexpr auto operator==(const Vec &a, const Vec &b) {
  return tie(a) == tie(b);
}
constexpr auto operator!=(const Vec &a, const Vec &b) {
  return tie(a) != tie(b);
}

std::ostream &operator<<(std::ostream &os, Vec v) {
  return os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

constexpr auto operator+=(Vec &a, const Vec &b) -> auto & {
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}

constexpr auto operator+(const Vec &a, const Vec &b) {
  auto copy{a};
  return copy += b;
}

constexpr auto gravity(int a, int b) {
  if (a < b) return 1;
  if (b < a) return -1;
  return 0;
}

template <typename Fn>
constexpr auto zipWith(const Vec &a, const Vec &b, Fn &&fn) {
  return Vec{fn(a.x, b.x), fn(a.y, b.y), fn(a.z, b.z)};
}

constexpr auto gravity(const Vec &a, const Vec &b) {
  return zipWith(a, b, [](auto i, auto j) { return gravity(i, j); });
}

struct Dim final {
  int pos{}, vel{};
};

std::ostream &operator<<(std::ostream &os, Dim d) {
  return os << '(' << d.pos << ", " << d.vel << ')';
}

constexpr auto tie(const Dim &d) { return std::tie(d.pos, d.vel); }
constexpr auto operator==(const Dim &a, const Dim &b) {
  return tie(a) == tie(b);
}
constexpr auto operator!=(const Dim &a, const Dim &b) {
  return tie(a) != tie(b);
}

struct Moon final {
  constexpr explicit Moon(Vec p) : x{p.x}, y{p.y}, z{p.z} {}
  constexpr Moon(Dim x, Dim y, Dim z) : x{x}, y{y}, z{z} {}

  constexpr auto pos() const { return Vec{x.pos, y.pos, z.pos}; }
  constexpr auto vel() const { return Vec{x.vel, y.vel, z.vel}; }

  Dim x, y, z;
};

constexpr auto tie(const Moon &m) { return std::tie(m.x, m.y, m.z); }
constexpr auto operator==(const Moon &a, const Moon &b) {
  return tie(a) == tie(b);
}
constexpr auto operator!=(const Moon &a, const Moon &b) {
  return tie(a) != tie(b);
}

template <size_t Num>
constexpr auto computeGravities(const std::array<int, Num> &m) {
  std::array<int, Num> result{{}};

  for (auto i = 0; i != Num; ++i) {
    for (auto j = 0; j != Num; ++j) {
      if (i != j) {
        result[i] += gravity(m[i], m[j]);
      }
    }
  }

  return result;
}

template <typename T, size_t... Ix, typename Fn>
constexpr auto transform(std::index_sequence<Ix...>,
                         const std::array<T, sizeof...(Ix)> &a, Fn &&fn) {
  return std::array{fn(a[Ix])...};
}

template <typename T, size_t Num, typename Fn>
constexpr auto transform(const std::array<T, Num> &a, Fn &&fn) {
  return transform(std::make_index_sequence<Num>{}, a, std::forward<Fn>(fn));
}

template <size_t Num>
constexpr auto step(std::array<Dim, Num> &m) {
  const auto gravities =
      computeGravities(transform(m, [](auto d) { return d.pos; }));

  for (auto i = 0; i != Num; ++i) {
    m[i].vel += gravities[i];
    m[i].pos += m[i].vel;
  }
}

template <size_t Num>
constexpr auto step(std::array<Dim, Num> &m, size_t steps) {
  for (size_t j = 0; j != steps; ++j) step(m);
}

auto abs(const Vec &v) {
  return Vec{std::abs(v.x), std::abs(v.y), std::abs(v.z)};
}

constexpr auto sumComponents(const Vec &v) { return v.x + v.y + v.z; }

auto computeEnergy(const Moon &m) {
  return sumComponents(abs(m.pos())) * sumComponents(abs(m.vel()));
}

template <size_t I, typename Fn, typename... Ts>
constexpr auto call(Fn &&fn, Ts &&... ts) {
  return fn(ts[I]...);
}

template <typename Fn, size_t... Ix, typename... Ts>
constexpr auto zipArrays(std::index_sequence<Ix...>, Fn &&fn, Ts &&... ts) {
  return std::array{call<Ix>(fn, ts...)...};
}

template <typename Fn, typename T, size_t Num, typename... Ts>
constexpr auto zipArrays(Fn &&fn, const std::array<T, Num> &a, Ts &&... ts) {
  return zipArrays(std::make_index_sequence<Num>{}, std::forward<Fn>(fn), a,
                   std::forward<Ts>(ts)...);
}

template <size_t Num>
auto energyAfter1000Steps(const std::array<Moon, Num> &i) {
  auto x = transform(i, [](auto m) { return m.x; });
  auto y = transform(i, [](auto m) { return m.y; });
  auto z = transform(i, [](auto m) { return m.z; });

  constexpr auto steps = 1000;

  step(x, steps);
  step(y, steps);
  step(z, steps);

  const auto moons = zipArrays(
      [](auto x, auto y, auto z) {
        return Moon{x, y, z};
      },
      x, y, z);

  return std::accumulate(
      moons.cbegin(), moons.cend(), 0,
      [](auto acc, const auto &m) { return acc + computeEnergy(m); });
}

constexpr auto test = std::array{Moon{Vec{-8, -10, 0}}, Moon{Vec{5, 5, 10}},
                                 Moon{Vec{2, -7, 3}}, Moon{Vec{9, -8, -3}}};

constexpr auto input = std::array{Moon{Vec{1, 2, -9}}, Moon{Vec{-1, -9, -4}},
                                  Moon{Vec{17, 6, 8}}, Moon{Vec{12, 4, 2}}};

template <size_t Num>
constexpr auto findLoopLength(const std::array<Dim, Num> &dim) {
  auto hare = dim;
  auto tortoise = dim;

  size_t out = 0;

  do {
    step(hare);
    step(hare);
    step(tortoise);
    out += 1;
  } while (hare != tortoise);

  return out;
}

} // namespace

TEST_CASE("day12") {
  REQUIRE(energyAfter1000Steps(input) == 7471);

  const auto xSteps =
      findLoopLength(transform(input, [](auto m) { return m.x; }));
  const auto ySteps =
      findLoopLength(transform(input, [](auto m) { return m.y; }));
  const auto zSteps =
      findLoopLength(transform(input, [](auto m) { return m.z; }));

  REQUIRE(std::lcm(std::lcm(xSteps, ySteps), zSteps) == 376243355967784);
}
