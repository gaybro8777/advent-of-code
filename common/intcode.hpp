#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <optional>

namespace reuk {

class Interpreter final {
  enum class Mode { position, immediate, relative };

  auto updateProgramSize(size_t size) -> auto &;
  auto read(Mode mode, size_t offset) -> auto &;

  template <size_t... Ind> auto readParams(std::index_sequence<Ind...>);
  template <size_t Params> auto readParams();

  template <size_t Params> static auto parseModes(int64_t value);

  std::deque<int64_t> program;
  std::deque<int64_t> inputs;
  int64_t ip{};
  int64_t relativeBase{};

public:
  template <typename Prog, typename Inputs = std::array<int64_t, 0>>
  explicit Interpreter(Prog &&prog, Inputs &&ins = {})
      : program(std::begin(prog), std::end(prog)),
        inputs(std::begin(ins), std::end(ins)) {}

  auto runUntilOutput() -> std::optional<int64_t>;

  template <typename Inputs = std::array<int64_t, 0>>
  auto runUntilOutput(Inputs &&newInputs) -> std::optional<int64_t> {
    using std::begin, std::end;
    inputs.insert(inputs.end(), std::begin(newInputs), std::end(newInputs));
    return runUntilOutput();
  }

  auto &operator[](size_t addr) const noexcept { return program[addr]; }
};

} // namespace reuk
