#include "intcode.hpp"

#include <catch2/catch.hpp>

#include <charconv>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <variant>

namespace {
constexpr auto dealIntoNewStack(int64_t oldPos, int64_t deckSize) {
  return (deckSize - 1 - oldPos) % deckSize;
}

constexpr auto cutNCards(int64_t oldPos, int64_t deckSize, int64_t num) {
  return (oldPos + deckSize - num) % deckSize;
}

constexpr auto dealWithIncrementN(int64_t oldPos, int64_t deckSize,
                                  int64_t num) {
  return (oldPos * num) % deckSize;
}

static_assert(dealWithIncrementN(0, 10, 7) == 0);
static_assert(dealWithIncrementN(1, 10, 7) == 7);
static_assert(dealWithIncrementN(7, 10, 7) == 9);

static_assert(dealIntoNewStack(dealWithIncrementN(cutNCards(0, 10, 6), 10, 7),
                               10) == 1);
static_assert(dealIntoNewStack(dealWithIncrementN(cutNCards(6, 10, 6), 10, 7),
                               10) == 9);

struct DealIntoNewStack final {
  constexpr auto apply(int64_t oldPos, int64_t deckSize) const {
    return dealIntoNewStack(oldPos, deckSize);
  }
};

struct Cut final {
  constexpr auto apply(int64_t oldPos, int64_t deckSize) const {
    return cutNCards(oldPos, deckSize, num);
  }

  int64_t num{};
};

struct DealWithIncrement final {
  constexpr auto apply(int64_t oldPos, int64_t deckSize) const {
    return dealWithIncrementN(oldPos, deckSize, num);
  }

  int64_t num{};
};

using Op = std::variant<DealIntoNewStack, Cut, DealWithIncrement>;

constexpr char input[] = R"(deal with increment 73
cut -6744
deal into new stack
cut 9675
deal with increment 63
cut -8047
deal with increment 21
cut -4726
deal with increment 39
cut -537
deal with increment 39
cut -6921
deal with increment 15
cut 2673
deal into new stack
cut 2483
deal with increment 66
deal into new stack
cut 1028
deal with increment 48
deal into new stack
cut -418
deal with increment 15
cut 9192
deal with increment 62
deal into new stack
deal with increment 23
cut 2840
deal with increment 50
cut 6222
deal with increment 20
deal into new stack
cut -6855
deal with increment 50
cut -1745
deal with increment 27
cut 4488
deal with increment 71
deal into new stack
deal with increment 28
cut -2707
deal with increment 40
deal into new stack
deal with increment 32
cut 8171
deal with increment 74
deal into new stack
cut -2070
deal with increment 61
deal into new stack
deal with increment 46
cut 4698
deal with increment 23
cut -3480
deal with increment 30
cut -6662
deal with increment 53
cut -5283
deal with increment 43
deal into new stack
cut -1319
deal with increment 9
cut -8990
deal into new stack
deal with increment 9
deal into new stack
cut -5058
deal with increment 28
cut -7975
deal with increment 57
cut 2766
deal with increment 19
cut 8579
deal into new stack
deal with increment 22
deal into new stack
cut 9835
deal with increment 36
cut -2485
deal with increment 52
cut -5818
deal with increment 9
cut 5946
deal with increment 51
deal into new stack
cut -5600
deal with increment 75
cut -9885
deal with increment 27
cut -2942
deal with increment 68
cut 3874
deal with increment 36
deal into new stack
deal with increment 20
cut -2565
deal with increment 17
cut -9109
deal with increment 62
cut 2175
)";

template <typename Range> auto getOps(Range &&input) {
  std::istringstream ss{input};
  std::string line;

  std::vector<Op> result;

  while (std::getline(ss, line)) {
    result.push_back([&]() -> Op {
      if (const auto pos = line.find("cut"); pos != std::string::npos) {
        int64_t num{};
        std::from_chars(line.data() + 4, line.data() + line.size(), num);
        return Cut{num};
      }

      if (const auto pos = line.find("deal with increment");
          pos != std::string::npos) {
        int64_t num{};
        std::from_chars(line.data() + 20, line.data() + line.size(), num);
        return DealWithIncrement{num};
      }

      return DealIntoNewStack{};
    }());
  }

  return result;
}

auto applyAllOps(int64_t card, int64_t deckSize, const std::vector<Op> &ops) {
  for (const auto &op : ops)
    card = std::visit([&](const auto &op) { return op.apply(card, deckSize); },
                      op);
  return card;
}

auto pt1() { return applyAllOps(2019, 10007, getOps(input)); }

// I cheated with part 2 :(
// This is from https://gist.github.com/romkatv/8ef7ea27ddce1de7b1b6f9b5a41838c4#file-day-22-2-cc
// Everything else is mine though!

template <typename Fn>
constexpr auto combine(Fn &&f, int64_t unit, int64_t a, int64_t b) {
  for (int64_t r = unit;; b >>= 1, a = f(a, a)) {
    if (!b)
      return r;
    if (b & 1)
      r = f(r, a);
  }
}

constexpr int64_t m = 119315717514047;
constexpr auto add(int64_t a, int64_t b) { return (m + a + b) % m; }
constexpr auto mul(int64_t a, int64_t b) { return combine(add, 0, a, b); }
constexpr auto pow(int64_t a, int64_t b) { return combine(mul, 1, a, b); }

} // namespace

TEST_CASE("day22") {
  REQUIRE(pt1() == 1867);

  int64_t k = 1, b = 0;

  for (const auto &op : getOps(input))
    std::visit(aoc::Overloaded{
                   [&](DealWithIncrement i) {
                     k = mul(k, i.num);
                     b = mul(b, i.num);
                   },
                   [&](Cut i) { b = add(b, -i.num); },
                   [&](DealIntoNewStack i) {
                     k = add(0, -k);
                     b = add(-1, -b);
                   },
               },
               op);
  const auto x = mul(b, pow(k - 1, m - 2));
  constexpr int64_t numShuffles = 101741582076661;

  REQUIRE(add(mul(add(x, 2020), pow(pow(k, m - 2), numShuffles)), -x) ==
          71047285772808);
}
