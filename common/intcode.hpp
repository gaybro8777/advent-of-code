#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <optional>
#include <variant>

namespace reuk {

template <class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> Overloaded(Ts...)->Overloaded<Ts...>;

class Interpreter final {
public:
  template <typename Prog, typename Inputs = std::array<int64_t, 0>>
  explicit Interpreter(Prog &&prog, Inputs &&ins = {})
      : program(std::begin(prog), std::end(prog)),
        inputs(std::begin(ins), std::end(ins)) {}

  struct Ongoing final {};
  struct Failure final {};
  struct Success final {
    int64_t value{};
  };

  using StepResult = std::variant<Ongoing, Failure, Success>;

  auto step() -> StepResult;
  auto runUntilOutput() -> std::optional<int64_t>;

  template <typename Inputs> auto queueInputs(Inputs &&in) {
    using std::begin, std::end;
    inputs.insert(inputs.end(), begin(in), end(in));
  }

  auto &operator[](size_t addr) const noexcept { return program[addr]; }

private:
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
};

} // namespace reuk
