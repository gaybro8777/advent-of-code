#include <array>
#include <iostream>
#include <numeric>
#include <tuple>

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
  if (a < b)
    return 1;
  if (b < a)
    return -1;
  return 0;
}

template <typename Fn>
constexpr auto zipWith(const Vec &a, const Vec &b, Fn &&fn) {
  return Vec{fn(a.x, b.x), fn(a.y, b.y), fn(a.z, b.z)};
}

constexpr auto gravity(const Vec &a, const Vec &b) {
  return zipWith(a, b, [](auto i, auto j) { return gravity(i, j); });
}

struct Moon final {
  Vec pos{}, vel{};
};

constexpr auto tie(const Moon &m) { return std::tie(m.pos, m.vel); }

constexpr auto operator==(const Moon &a, const Moon &b) {
  return tie(a) == tie(b);
}
constexpr auto operator!=(const Moon &a, const Moon &b) {
  return tie(a) != tie(b);
}

template <size_t Num>
constexpr auto computeGravities(const std::array<Moon, Num> &m) {
  std::array<Vec, Num> result;

  for (auto i = 0; i != Num; ++i) {
    for (auto j = 0; j != Num; ++j) {
      if (i != j) {
        result[i] += gravity(m[i].pos, m[j].pos);
      }
    }
  }

  return result;
}

template <size_t Num> constexpr auto step(std::array<Moon, Num> &m) {
  const auto gravities = computeGravities(m);

  for (auto i = 0; i != Num; ++i) {
    m[i].vel += gravities[i];
    m[i].pos += m[i].vel;
  }
}

auto abs(const Vec &v) {
  return Vec{std::abs(v.x), std::abs(v.y), std::abs(v.z)};
}

constexpr auto sumComponents(const Vec &v) { return v.x + v.y + v.z; }

auto computeEnergy(const Moon &m) {
  return sumComponents(abs(m.pos)) * sumComponents(abs(m.vel));
}

template <size_t Num>
constexpr auto step(std::array<Moon, Num> &m, size_t steps) {
  for (size_t j = 0; j != steps; ++j)
    step(m);
}

template <size_t Num> auto energyAfter1000Steps(std::array<Moon, Num> i) {
  step(i, 1000);

  return std::accumulate(i.cbegin(), i.cend(), 0, [](auto acc, const auto &m) {
    return acc + computeEnergy(m);
  });
}

constexpr auto test = std::array{Moon{Vec{-8, -10, 0}}, Moon{Vec{5, 5, 10}},
                                 Moon{Vec{2, -7, 3}}, Moon{Vec{9, -8, -3}}};

constexpr auto input = std::array{Moon{Vec{2, 2, -9}}, Moon{Vec{-1, -9, -4}},
                                  Moon{Vec{17, 6, 8}}, Moon{Vec{12, 4, 2}}};

auto main() -> int {
  std::cout << energyAfter1000Steps(input) << '\n';

  auto hare = input;
  auto tortoise = input;
  size_t out = 0;

  do {
    step(hare);
    step(hare);
    step(tortoise);
    out += 1;

    if (out % 1000000 == 0) std::cout << out << '\n';
  } while (hare != tortoise);

  std::cout << out << '\n';
}
