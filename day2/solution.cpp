#include <array>
#include <iostream>
#include <optional>
#include <stdexcept>

enum class Result { failure, success };

template <typename Range> inline Result run(Range &&range) noexcept {
  using std::begin;
  const auto mem = begin(range);
  auto ip = mem;

  const auto op = [&mem, &ip](auto &&fn) {
    mem[ip[3]] = fn(mem[ip[1]], mem[ip[2]]);
    std::advance(ip, 4);
  };

  for (;;) {
    switch (*ip) {
    case 1:
      op(std::plus<>{});
      break;
    case 2:
      op(std::multiplies<>{});
      break;
    case 99:
      return Result::success;
    default:
      return Result::failure;
    }
  }
}

auto runTest(int noun, int verb) noexcept -> std::optional<int> {
  auto input = std::array{
      1,  noun, verb, 3,   1, 1,   2,  3,   1, 3,   4,   3,   1, 5,   0,   3,
      2,  1,    6,    19,  2, 19,  6,  23,  1, 23,  5,   27,  1, 9,   27,  31,
      1,  31,   10,   35,  2, 35,  9,  39,  1, 5,   39,  43,  2, 43,  9,   47,
      1,  5,    47,   51,  2, 51,  13, 55,  1, 55,  10,  59,  1, 59,  10,  63,
      2,  9,    63,   67,  1, 67,  5,  71,  2, 13,  71,  75,  1, 75,  10,  79,
      1,  79,   6,    83,  2, 13,  83, 87,  1, 87,  6,   91,  1, 6,   91,  95,
      1,  10,   95,   99,  2, 99,  6,  103, 1, 103, 5,   107, 2, 6,   107, 111,
      1,  10,   111,  115, 1, 115, 5,  119, 2, 6,   119, 123, 1, 123, 5,   127,
      2,  127,  6,    131, 1, 131, 5,  135, 1, 2,   135, 139, 1, 139, 13,  0,
      99, 2,    0,    14,  0};

  if (run(input) == Result::success)
    return input.front();

  return {};
}

int main() {
  const auto lim = 100;

  for (auto noun = 0; noun != lim; ++noun) {
    for (auto verb = 0; verb != lim; ++verb) {
      const auto result = runTest(noun, verb);

      if (result && *result == 19690720) {
        std::cout << noun << ' ' << verb << '\n';
        break;
      }
    }
  }
}
