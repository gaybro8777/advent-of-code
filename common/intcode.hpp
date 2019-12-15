#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <optional>

namespace reuk {

class Interpreter final {
  enum class Mode { position, immediate, relative };

  auto updateProgramSize(size_t size) -> auto & {
    program.resize(std::max(program.size(), size + 1), 0);
    return program[size];
  }

  auto read(Mode mode, size_t offset) -> auto & {
    switch (mode) {
    case Mode::position:
      return updateProgramSize(program[ip + offset + 1]);
    case Mode::immediate:
      return program[ip + offset + 1];
    case Mode::relative:
      return updateProgramSize(relativeBase + program[ip + offset + 1]);
    }
  }

  template <size_t... Ind> auto readParams(std::index_sequence<Ind...>) {
    const auto modes = parseModes<sizeof...(Ind)>(program[ip]);
    return std::array{std::ref(read(modes[Ind], Ind))...};
  }

  template <size_t Params> auto readParams() {
    auto result = readParams(std::make_index_sequence<Params>{});
    ip += Params + 1;
    return result;
  }

  template <size_t Params> static constexpr auto parseModes(int64_t value) {
    value /= 100;
    std::array<Mode, Params> result{{}};

    for (auto i = 0; i != Params; ++i) {
      result[i] = Mode(value % 10);
      value /= 10;
    }

    return result;
  }

  std::deque<int64_t> program;
  std::deque<int64_t> inputs;
  int64_t ip{};
  int64_t relativeBase{};

public:
  template <typename Prog, typename Inputs = std::array<int64_t, 0>>
  explicit Interpreter(Prog &&prog, Inputs &&ins = {})
      : program(std::begin(prog), std::end(prog)),
        inputs(std::begin(ins), std::end(ins)) {}

  template <typename Inputs = std::array<int64_t, 0>>
  auto runUntilOutput(Inputs &&newInputs = {}) -> std::optional<int64_t> {
    using std::begin, std::end;
    inputs.insert(inputs.end(), std::begin(newInputs), std::end(newInputs));

    for (;;) {
      switch (program[ip] % 100) {
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
        if (inputs.empty())
          return {};

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
        updateProgramSize(params[1]);
        ip = params[0] ? int64_t{params[1]} : ip;
        break;
      }
      case 6: {
        auto params = readParams<2>();
        updateProgramSize(params[1]);
        ip = !params[0] ? int64_t{params[1]} : ip;
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
      case 9: {
        auto params = readParams<1>();
        relativeBase += params[0];
        break;
      }
      case 99:
        [[fallthrough]];
      default:
        return {};
      }
    }
  }

  auto &operator[](size_t addr) const noexcept { return program[addr]; }
};

} // namespace reuk
