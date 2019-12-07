#include <array>
#include <cstdint>
#include <iostream>
#include <optional>
#include <queue>
#include <stdexcept>
#include <variant>

class Interpreter final {
  enum class Mode { position, immediate };

  auto read(Mode mode, size_t offset) -> auto & {
    switch (mode) {
    case Mode::position:
      return program[ip[offset + 1]];
    case Mode::immediate:
      return ip[offset + 1];
    }
  }

  template <size_t... Ind> auto readParams(std::index_sequence<Ind...>) {
    const auto modes = parseModes<sizeof...(Ind)>(*ip);
    return std::array{std::ref(read(modes[Ind], Ind))...};
  }

  template <size_t Params> auto readParams() {
    auto result = readParams(std::make_index_sequence<Params>{});
    ip += Params + 1;
    return result;
  }

  template <size_t Params> static constexpr auto parseModes(int value) {
    value /= 100;
    std::array<Mode, Params> result{{}};

    for (auto i = 0; i != Params; ++i) {
      result[i] = Mode(value % 10);
      value /= 10;
    }

    return result;
  }

  std::vector<int> program;
  std::vector<int>::iterator ip;
  std::deque<int> inputs;

public:
  template <typename Prog, typename Inputs>
  Interpreter(Prog &&prog, Inputs &&ins)
      : program(std::begin(prog), std::end(prog)), ip(std::begin(program)),
        inputs(std::begin(ins), std::end(ins)) {}

  template <typename Inputs>
  auto runUntilOutput(Inputs &&newInputs) -> std::optional<int> {
    using std::begin, std::end;
    inputs.insert(inputs.end(), begin(newInputs), end(newInputs));

    for (;;) {
      switch (*ip % 100) {
      case 1: {
        auto params = readParams<3>();
        params[2].get() = params[0] + params[1];
        break;
      }
      case 2: {
        auto params = readParams<3>();
        params[2].get() = params[0] * params[1];
        break;
      }
      case 3: {
        auto params = readParams<1>();
        params[0].get() = inputs.front();
        inputs.pop_front();
        break;
      }
      case 4: {
        auto params = readParams<1>();
        return params[0].get();
      }
      case 5: {
        auto params = readParams<2>();
        ip = params[0] ? std::next(program.begin(), params[1]) : ip;
        break;
      }
      case 6: {
        auto params = readParams<2>();
        ip = ! params[0] ? std::next(program.begin(), params[1]) : ip;
        break;
      }
      case 7: {
        auto params = readParams<3>();
        params[2].get() = params[0] < params[1] ? 1 : 0;
        break;
      }
      case 8: {
        auto params = readParams<3>();
        params[2].get() = params[0] == params[1] ? 1 : 0;
        break;
      }
      case 99: [[fallthrough]];
      default:
        return {};
      }
    }
  }
};

template <typename Program, size_t... Ind>
auto makeInterpreters(std::index_sequence<Ind...>, Program &&program,
                      const std::array<int, sizeof...(Ind)> &phases) {
  return std::array{Interpreter{program, std::array{phases[Ind]}}...};
}

template <typename Program, size_t Num>
auto makeInterpreters(Program &&program, const std::array<int, Num> &phases) {
  return makeInterpreters(std::make_index_sequence<Num>(),
                          std::forward<Program>(program), phases);
}

template <typename Program, size_t Num>
auto runAmplifiersWithFeedback(Program &&program,
                               const std::array<int, Num> &phases)
    -> std::optional<int64_t> {
  auto interpreters = makeInterpreters(program, phases);
  auto in = 0;

  for (auto it = interpreters.begin();; ++it) {
    if (it == interpreters.end())
      it = interpreters.begin();

    if (const auto result = it->runUntilOutput(std::array{in}))
      in = *result;
    else
      break;
  }

  return in;
}

template <typename Program, size_t Num>
auto findAmplificationWithFeedback(Program &&program, std::array<int, Num> phases) {
  int64_t max = 0;

  do {
    if (const auto result = runAmplifiersWithFeedback(program, phases))
      max = std::max(max, *result);
  } while (std::next_permutation(phases.begin(), phases.end()));

  return max;
}

auto main() -> int {
  const auto prog = std::array{
      3,    8,    1001, 8,    10,   8,    105,  1,    0,     0,    21,   42,
      51,   60,   77,   94,   175,  256,  337,  418,  99999, 3,    9,    1001,
      9,    4,    9,    102,  5,    9,    9,    1001, 9,     3,    9,    102,
      5,    9,    9,    4,    9,    99,   3,    9,    102,   2,    9,    9,
      4,    9,    99,   3,    9,    1001, 9,    3,    9,     4,    9,    99,
      3,    9,    101,  4,    9,    9,    1002, 9,    4,     9,    101,  5,
      9,    9,    4,    9,    99,   3,    9,    1002, 9,     5,    9,    101,
      3,    9,    9,    102,  2,    9,    9,    4,    9,     99,   3,    9,
      1001, 9,    1,    9,    4,    9,    3,    9,    1002,  9,    2,    9,
      4,    9,    3,    9,    102,  2,    9,    9,    4,     9,    3,    9,
      102,  2,    9,    9,    4,    9,    3,    9,    102,   2,    9,    9,
      4,    9,    3,    9,    101,  2,    9,    9,    4,     9,    3,    9,
      101,  2,    9,    9,    4,    9,    3,    9,    101,   2,    9,    9,
      4,    9,    3,    9,    1001, 9,    1,    9,    4,     9,    3,    9,
      101,  1,    9,    9,    4,    9,    99,   3,    9,     1001, 9,    2,
      9,    4,    9,    3,    9,    101,  1,    9,    9,     4,    9,    3,
      9,    102,  2,    9,    9,    4,    9,    3,    9,     1001, 9,    1,
      9,    4,    9,    3,    9,    101,  2,    9,    9,     4,    9,    3,
      9,    1002, 9,    2,    9,    4,    9,    3,    9,     1001, 9,    1,
      9,    4,    9,    3,    9,    101,  1,    9,    9,     4,    9,    3,
      9,    1001, 9,    2,    9,    4,    9,    3,    9,     1002, 9,    2,
      9,    4,    9,    99,   3,    9,    101,  2,    9,     9,    4,    9,
      3,    9,    102,  2,    9,    9,    4,    9,    3,     9,    101,  2,
      9,    9,    4,    9,    3,    9,    102,  2,    9,     9,    4,    9,
      3,    9,    1002, 9,    2,    9,    4,    9,    3,     9,    102,  2,
      9,    9,    4,    9,    3,    9,    102,  2,    9,     9,    4,    9,
      3,    9,    101,  2,    9,    9,    4,    9,    3,     9,    1001, 9,
      2,    9,    4,    9,    3,    9,    101,  2,    9,     9,    4,    9,
      99,   3,    9,    101,  2,    9,    9,    4,    9,     3,    9,    1002,
      9,    2,    9,    4,    9,    3,    9,    1001, 9,     1,    9,    4,
      9,    3,    9,    101,  1,    9,    9,    4,    9,     3,    9,    1001,
      9,    1,    9,    4,    9,    3,    9,    1002, 9,     2,    9,    4,
      9,    3,    9,    1002, 9,    2,    9,    4,    9,     3,    9,    101,
      1,    9,    9,    4,    9,    3,    9,    102,  2,     9,    9,    4,
      9,    3,    9,    101,  2,    9,    9,    4,    9,     99,   3,    9,
      1001, 9,    2,    9,    4,    9,    3,    9,    101,   2,    9,    9,
      4,    9,    3,    9,    1002, 9,    2,    9,    4,     9,    3,    9,
      101,  2,    9,    9,    4,    9,    3,    9,    101,   2,    9,    9,
      4,    9,    3,    9,    102,  2,    9,    9,    4,     9,    3,    9,
      1002, 9,    2,    9,    4,    9,    3,    9,    102,   2,    9,    9,
      4,    9,    3,    9,    1002, 9,    2,    9,    4,     9,    3,    9,
      101,  1,    9,    9,    4,    9,    99};
  std::cout << findAmplificationWithFeedback(prog, std::array{0, 1, 2, 3, 4})
            << '\n';
  std::cout << findAmplificationWithFeedback(prog, std::array{5, 6, 7, 8, 9})
            << '\n';

  return {};
}

