#pragma once

#include "coord.hpp"

#include <array>

namespace aoc {

enum class Direction2d { up, right, down, left };

constexpr auto directions2d = std::array{Direction2d::up, Direction2d::right,
                                         Direction2d::down, Direction2d::left};

constexpr auto toCoord(Direction2d d) -> aoc::Coord {
  switch (d) {
  case Direction2d::up:
    return {0, -1};
  case Direction2d::right:
    return {1, 0};
  case Direction2d::down:
    return {0, 1};
  case Direction2d::left:
    return {-1, 0};
  }

  return {};
}

constexpr auto toChar(Direction2d d) {
  switch (d) {
  case Direction2d::up:
    return 'u';
  case Direction2d::right:
    return 'r';
  case Direction2d::down:
    return 'd';
  case Direction2d::left:
    return 'l';
  }

  return ' ';
}

inline auto operator<<(std::ostream &os, Direction2d d) -> auto & {
  return os << toChar(d);
}

} // namespace aoc
