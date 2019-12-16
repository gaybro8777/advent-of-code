#include <charconv>
#include <iostream>
#include <string>
#include <vector>

auto computeIndex(const std::vector<int> &pattern, size_t position) {
  const auto repetitions = position + 1;

  int64_t sum{};

  for (size_t i = 0; i < pattern.size(); i += 4 * repetitions) {
    for (size_t j = 0; j != repetitions; ++j) {
      if (const auto addInd = i + j + repetitions - 1; addInd < pattern.size())
        sum += pattern[addInd];

      if (const auto subInd = i + j + (3 * repetitions) - 1;
          subInd < pattern.size())
        sum -= pattern[subInd];
    }
  }

  return std::abs(sum) % 10;
}

auto fft(std::vector<int> pattern, size_t steps) {
  auto copy{pattern};
  const auto size = pattern.size();

  for (size_t i = 0; i != steps; ++i) {
    for (size_t j = 0; j != size; ++j) {
      const auto ind = size - j - 1;

      copy[ind] = [&]() -> int {
        if (j == size - 1)
          return pattern[ind];

        if (ind > size / 2)
          return (pattern[ind] + copy[ind + 1]) % 10;

        return computeIndex(pattern, ind);
      }();
    }

    std::swap(copy, pattern);
  }

  return pattern;
}

auto subFft(std::vector<int> pattern, size_t steps, const size_t minIndex) {
  auto copy{pattern};
  const auto size = pattern.size();

  for (size_t i = 0; i != steps; ++i) {
    for (size_t j = 0; j != size - minIndex; ++j) {
      const auto ind = size - j - 1;

      copy[ind] = [&]() -> int {
        if (ind == size - 1)
          return pattern[ind];

        return (pattern[ind] + copy[ind + 1]) % 10;
      }();
    }

    std::swap(copy, pattern);
  }

  return pattern;
}

auto digits(std::string_view str) {
  std::vector<int> result(str.size());

  for (size_t i = 0; i != str.size(); ++i)
    std::from_chars(str.data() + i, str.data() + i + 1, result[i]);

  return result;
}

auto repeat(const std::vector<int> &v, size_t times) {
  std::vector<int> result;

  for (auto i = 0; i != times; ++i)
    result.insert(result.cend(), v.cbegin(), v.cend());

  return result;
}

constexpr auto input =
    "59756772370948995765943195844952640015210703313486295362653878290009098923"
    "60976926147353400939518848086432595978647008476260766631250309150546625879"
    "60622306527696338182826534978530181082815676278997225486022574636085303312"
    "99936274116326038606007040084159138769832784921878333830514041948066594667"
    "15259394515917081677982026475871510149473924453309569603933607051097561219"
    "04173910678964102623108358300065446320834214473855422569161412563838133606"
    "62952845638955872442636455511906111157861890394133454959320174572270568292"
    "97262125346089562586261622899814730167085034083199304361731693874836198471"
    "4845874270986989103792418940945322846146634931990046966552";

auto main() -> int {
  const auto d = digits(input);
  const auto r = fft(d, 100);

  for (auto i = 0; i != 8; ++i)
    std::cout << r[i];

  std::cout << '\n';

  size_t offset{};
  std::from_chars(input, input + 7, offset);

  const auto d2 = repeat(d, 10000);
  const auto r2 = subFft(d2, 100, offset);

  for (auto i = 0; i != 8; ++i)
    std::cout << r2[offset + i];

  std::cout << '\n';
}
