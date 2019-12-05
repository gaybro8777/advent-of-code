#include <array>
#include <iostream>
#include <optional>
#include <stdexcept>

enum class Result { failure, success };

enum class Mode { position, immediate };

template <typename It> struct Instruction final {
  Instruction(const It &mem, It &ip) : mem{mem}, ip{ip} {}

  template <size_t Params, typename Fn> void run(Fn &&fn) {
    call(std::make_index_sequence<Params>{}, std::forward<Fn>(fn));
  }

private:
  template <size_t Params> auto parseModes(int value) {
    value /= 100;
    std::array<Mode, Params> result{{}};

    for (auto i = 0; i != Params; ++i) {
      result[i] = Mode(value % 10);
      value /= 10;
    }

    return result;
  }

  auto read(Mode mode, size_t offset) -> auto & {
    switch (mode) {
    case Mode::position:
      return mem[ip[offset + 1]];
    case Mode::immediate:
      return ip[offset + 1];
    }
  }

  template <size_t... Ind, typename Fn>
  void call(std::index_sequence<Ind...>, Fn &&fn) {
    constexpr auto Params = sizeof...(Ind);
    const auto modes = parseModes<Params>(*ip);

    if (fn(ip, read(modes[Ind], Ind)...))
      std::advance(ip, Params + 1);
  }

  const It &mem;
  It &ip;
};

template <size_t Params, typename It, typename Fn>
void run(const It &mem, It &ip, Fn &&fn) {
  Instruction<It>{mem, ip}.template run<Params>(std::forward<Fn>(fn));
}

template <typename Range> inline Result run(Range &&range) noexcept {
  using std::begin;
  const auto mem = begin(range);
  auto ip = mem;

  for (;;) {
    switch (*ip % 100) {
    case 1:
      run<3>(mem, ip, [](auto &, auto &a, auto &b, auto &res) {
        res = a + b;
        return true;
      });
      break;
    case 2:
      run<3>(mem, ip, [](auto &, auto &a, auto &b, auto &res) {
        res = a * b;
        return true;
      });
      break;
    case 3:
      run<1>(mem, ip, [](auto &, auto &res) {
        std::cin >> res;
        return true;
      });
      break;
    case 4:
      run<1>(mem, ip, [](auto &, auto &res) {
        std::cout << res << '\n';
        return true;
      });
      break;
    case 5:
      run<2>(mem, ip, [&](auto &ip, auto &a, auto &b) {
        ip = a ? (mem + b) : ip;
        return !a;
      });
      break;
    case 6:
      run<2>(mem, ip, [&](auto &ip, auto &a, auto &b) {
        ip = (!a) ? (mem + b) : ip;
        return a;
      });
      break;
    case 7:
      run<3>(mem, ip, [&](auto &ip, auto &a, auto &b, auto &res) {
        res = a < b ? 1 : 0;
        return true;
      });
      break;
    case 8:
      run<3>(mem, ip, [&](auto &ip, auto &a, auto &b, auto &res) {
        res = a == b ? 1 : 0;
        return true;
      });
      break;
    case 99:
      return Result::success;
    default:
      return Result::failure;
    }
  }
}

int main() {
  auto input = std::array{
      3,     225,  1,     225,   6,     6,     1100, 1,     238,   225,   104,
      0,     1102, 27,    28,    225,   1,     113,  14,    224,   1001,  224,
      -34,   224,  4,     224,   102,   8,     223,  223,   101,   7,     224,
      224,   1,    224,   223,   223,   1102,  52,   34,    224,   101,   -1768,
      224,   224,  4,     224,   1002,  223,   8,    223,   101,   6,     224,
      224,   1,    223,   224,   223,   1002,  187,  14,    224,   1001,  224,
      -126,  224,  4,     224,   102,   8,     223,  223,   101,   2,     224,
      224,   1,    224,   223,   223,   1102,  54,   74,    225,   1101,  75,
      66,    225,  101,   20,    161,   224,   101,  -54,   224,   224,   4,
      224,   1002, 223,   8,     223,   1001,  224,  7,     224,   1,     224,
      223,   223,  1101,  6,     30,    225,   2,    88,    84,    224,   101,
      -4884, 224,  224,   4,     224,   1002,  223,  8,     223,   101,   2,
      224,   224,  1,     224,   223,   223,   1001, 214,   55,    224,   1001,
      224,   -89,  224,   4,     224,   102,   8,    223,   223,   1001,  224,
      4,     224,  1,     224,   223,   223,   1101, 34,    69,    225,   1101,
      45,    67,   224,   101,   -112,  224,   224,  4,     224,   102,   8,
      223,   223,  1001,  224,   2,     224,   1,    223,   224,   223,   1102,
      9,     81,   225,   102,   81,    218,   224,  101,   -7290, 224,   224,
      4,     224,  1002,  223,   8,     223,   101,  5,     224,   224,   1,
      223,   224,  223,   1101,  84,    34,    225,  1102,  94,    90,    225,
      4,     223,  99,    0,     0,     0,     677,  0,     0,     0,     0,
      0,     0,    0,     0,     0,     0,     0,    1105,  0,     99999, 1105,
      227,   247,  1105,  1,     99999, 1005,  227,  99999, 1005,  0,     256,
      1105,  1,    99999, 1106,  227,   99999, 1106, 0,     265,   1105,  1,
      99999, 1006, 0,     99999, 1006,  227,   274,  1105,  1,     99999, 1105,
      1,     280,  1105,  1,     99999, 1,     225,  225,   225,   1101,  294,
      0,     0,    105,   1,     0,     1105,  1,    99999, 1106,  0,     300,
      1105,  1,    99999, 1,     225,   225,   225,  1101,  314,   0,     0,
      106,   0,    0,     1105,  1,     99999, 1007, 677,   677,   224,   102,
      2,     223,  223,   1005,  224,   329,   101,  1,     223,   223,   1108,
      226,   677,  224,   1002,  223,   2,     223,  1005,  224,   344,   101,
      1,     223,  223,   1008,  677,   677,   224,  102,   2,     223,   223,
      1005,  224,  359,   101,   1,     223,   223,  8,     226,   677,   224,
      1002,  223,  2,     223,   1006,  224,   374,  101,   1,     223,   223,
      108,   226,  677,   224,   1002,  223,   2,    223,   1006,  224,   389,
      1001,  223,  1,     223,   1107,  226,   677,  224,   102,   2,     223,
      223,   1005, 224,   404,   1001,  223,   1,    223,   7,     226,   677,
      224,   1002, 223,   2,     223,   1005,  224,  419,   101,   1,     223,
      223,   1107, 677,   226,   224,   102,   2,    223,   223,   1006,  224,
      434,   1001, 223,   1,     223,   1107,  226,  226,   224,   1002,  223,
      2,     223,  1006,  224,   449,   101,   1,    223,   223,   1108,  226,
      226,   224,  1002,  223,   2,     223,   1005, 224,   464,   101,   1,
      223,   223,  8,     677,   226,   224,   102,  2,     223,   223,   1005,
      224,   479,  101,   1,     223,   223,   8,    226,   226,   224,   1002,
      223,   2,    223,   1006,  224,   494,   1001, 223,   1,     223,   1007,
      226,   677,  224,   1002,  223,   2,     223,  1006,  224,   509,   1001,
      223,   1,    223,   108,   226,   226,   224,  1002,  223,   2,     223,
      1006,  224,  524,   1001,  223,   1,     223,  1108,  677,   226,   224,
      102,   2,    223,   223,   1006,  224,   539,  101,   1,     223,   223,
      1008,  677,  226,   224,   102,   2,     223,  223,   1006,  224,   554,
      101,   1,    223,   223,   107,   226,   677,  224,   1002,  223,   2,
      223,   1006, 224,   569,   101,   1,     223,  223,   107,   677,   677,
      224,   102,  2,     223,   223,   1006,  224,  584,   101,   1,     223,
      223,   7,    677,   226,   224,   102,   2,    223,   223,   1005,  224,
      599,   101,  1,     223,   223,   1008,  226,  226,   224,   1002,  223,
      2,     223,  1005,  224,   614,   1001,  223,  1,     223,   107,   226,
      226,   224,  1002,  223,   2,     223,   1005, 224,   629,   101,   1,
      223,   223,  7,     226,   226,   224,   102,  2,     223,   223,   1006,
      224,   644,  1001,  223,   1,     223,   1007, 226,   226,   224,   102,
      2,     223,  223,   1006,  224,   659,   101,  1,     223,   223,   108,
      677,   677,  224,   102,   2,     223,   223,  1005,  224,   674,   1001,
      223,   1,    223,   4,     223,   99,    226};

  run(input);

  for (auto i : input)
    std::cout << i << ' ';

  std::cout << '\n';

  return {};
}

