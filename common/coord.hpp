#pragma once

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <tuple>

namespace reuk {

struct Coord final {
  int64_t x{}, y{};
};

inline auto operator<<(std::ostream &os, const Coord &c) -> auto & {
  return os << '(' << c.x << ", " << c.y << ')';
}

constexpr auto tie(const Coord &c) { return std::tie(c.x, c.y); }
constexpr auto operator<(const Coord &a, const Coord &b) {
  return tie(a) < tie(b);
}
constexpr auto operator==(const Coord &a, const Coord &b) {
  return tie(a) == tie(b);
}
constexpr auto operator!=(const Coord &a, const Coord &b) {
  return tie(a) != tie(b);
}

constexpr auto operator+=(Coord &a, const Coord &b) -> auto & {
  a.x += b.x;
  a.y += b.y;
  return a;
}

constexpr auto operator+(const Coord &a, const Coord &b) {
  auto copy{a};
  return copy += b;
}

constexpr auto operator-=(Coord &a, const Coord &b) -> auto & {
  a.x -= b.x;
  a.y -= b.y;
  return a;
}

constexpr auto operator-(const Coord &a, const Coord &b) {
  auto copy{a};
  return copy -= b;
}

constexpr auto operator*=(Coord &a, const Coord &b) -> auto & {
  a.x *= b.x;
  a.y *= b.y;
  return a;
}

constexpr auto operator*(const Coord &a, const Coord &b) {
  auto copy{a};
  return copy *= b;
}

inline auto min(const Coord &a, const Coord &b) {
  return Coord{std::min(a.x, b.x), std::min(a.y, b.y)};
}

inline auto max(const Coord &a, const Coord &b) {
  return Coord{std::max(a.x, b.x), std::max(a.y, b.y)};
}

constexpr auto magSq(const Coord &a) { return a.x * a.x + a.y * a.y; }

} // namespace reuk
