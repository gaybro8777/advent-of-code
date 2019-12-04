#include <algorithm>
#include <array>
#include <charconv>
#include <functional>
#include <iostream>

template <typename Fn> bool isValid(int pwd, Fn &&extraCheck) {
  std::array<char, 6> chars;
  const auto result =
      std::to_chars(chars.data(), chars.data() + chars.size(), pwd);

  if (result.ptr != chars.data() + chars.size() || result.ec != std::errc{})
    return false;

  if (!std::is_sorted(chars.cbegin(), chars.cend()))
    return false;

  return std::any_of(chars.cbegin(), chars.cend(), [&](auto v) {
    return extraCheck(std::count(chars.cbegin(), chars.cend(), v), 2);
  });
}

int main() {
  auto countA = 0;
  auto countB = 0;

  for (auto pwd = 153517; pwd <= 630395; ++pwd) {
    countA += isValid(pwd, std::greater_equal<>{}) ? 1 : 0;
    countB += isValid(pwd, std::equal_to<>{}) ? 1 : 0;
  }

  std::cout << countA << '\n';
  std::cout << countB << '\n';
}
