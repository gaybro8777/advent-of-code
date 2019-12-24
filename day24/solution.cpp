#include "direction2d.hpp"

#include <catch2/catch.hpp>

#include <array>
#include <iostream>
#include <optional>

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
  explicit Map(std::string_view m)
      : storage_(m), width_(storage_.find('\n')) {}

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

constexpr auto start = R"(#####
.#.#.
.#..#
....#
..###
)";

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
  tortoise = Sim{start};

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

} // namespace

TEST_CASE("day24") {
  const auto repeated = firstRepetition(start);
  std::cout << repeated << '\n';
  std::cout << biodiversity(repeated) << '\n';
}
