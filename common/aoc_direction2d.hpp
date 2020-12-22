#pragma once

#include "aoc_coord.hpp"

#include <array>

namespace aoc {

enum class Direction2d { up, right, down, left };

constexpr std::array directions2d{Direction2d::up, Direction2d::right,
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

constexpr auto opposite(Direction2d e) -> Direction2d {
  switch (e) {
  case Direction2d::up:
    return Direction2d::down;
  case Direction2d::down:
    return Direction2d::up;
  case Direction2d::left:
    return Direction2d::right;
  case Direction2d::right:
    return Direction2d::left;
  }

  return {};
}

inline auto operator<<(std::ostream &os, Direction2d d) -> auto & {
  return os << toChar(d);
}

} // namespace aoc
