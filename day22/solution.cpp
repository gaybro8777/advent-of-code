#include <catch2/catch.hpp>

#include <charconv>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <variant>

namespace {
constexpr auto dealIntoNewStack(int64_t oldPos, int64_t deckSize) {
  return deckSize - 1 - oldPos;
}

constexpr auto cutNCards(int64_t oldPos, int64_t deckSize, int64_t num) {
  if (num > 0) {
    oldPos -= num;

    if (oldPos < 0)
      oldPos += deckSize;
  } else {
    oldPos -= num;

    if (deckSize <= oldPos)
      oldPos -= deckSize;
  }

  return oldPos;
}

constexpr auto mulmod(int64_t a, int64_t b, int64_t mod) {
  int64_t res = 0;
  a = a % mod;

  while (b > 0) {
    if (b % 2 == 1)
      res = (res + a) % mod;

    a = (a * 2) % mod;
    b /= 2;
  }

  return res % mod;
}

constexpr auto dealWithIncrementN(int64_t oldPos, int64_t deckSize,
                                  int64_t num) {
  return mulmod(oldPos, num, deckSize);
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

constexpr auto crackUnknownIncrement(std::array<int64_t, 2> states,
                                     int64_t modulus, int64_t multiplier) {
  const int64_t increment = (states[1] - states[0] * multiplier) % modulus;
  return std::tuple{modulus, multiplier, increment};
}

constexpr auto egcd(int64_t a, int64_t b) -> std::tuple<int64_t, int64_t, int64_t>{
  if (a == 0)
    return {b, 0, 1};

  const auto [g, x, y] = egcd(b % a, a);
  return {g, y - (b / a) * x, x};
}

constexpr auto modinv(int64_t b, int64_t n) {
  const auto [g, x, _] = egcd(b, n);
  if (g == 1)
    return x % n;
  return int64_t{};
}

constexpr auto crackUnknownMultiplier(std::array<int64_t, 3> states,
                                      int64_t modulus) {
  const auto multiplier = __int128(states[2] - states[1]) *
                          __int128(modinv(states[1] - states[0], modulus)) %
                          modulus;
  return crackUnknownIncrement(std::array{states[0], states[1]}, modulus,
                               multiplier);
}

} // namespace

TEST_CASE("day22") {
  REQUIRE(pt1() == 1867);

  const auto ops = getOps(input);
  const int64_t deckSize = 119315717514047;
  const int64_t s0 = 0;
  const auto s1 = applyAllOps(s0, deckSize, ops);
  const auto s2 = applyAllOps(s1, deckSize, ops);

  const auto [modulus, multiplier, increment] =
      crackUnknownMultiplier(std::array{s0, s1, s2}, deckSize);

  const auto s3 = applyAllOps(s2, deckSize, ops);
  const auto v2 = (mulmod(s2, multiplier, modulus) + increment) % modulus;

//  REQUIRE(s3 == v2);
}
