#include <catch2/catch.hpp>

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

namespace {

auto constexpr test_input =
    R"(light red bags contain 1 bright white bag, 2 muted yellow bags.
dark orange bags contain 3 bright white bags, 4 muted yellow bags.
bright white bags contain 1 shiny gold bag.
muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
dark olive bags contain 3 faded blue bags, 4 dotted black bags.
vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
faded blue bags contain no other bags.
dotted black bags contain no other bags.
)";

struct Inner {
  std::string kind;
  int num{};
};

struct Line {
  std::string outer;
  std::vector<Inner> inner;
};

std::istream &operator>>(std::istream &is, Line &line) {
  line.inner.clear();

  std::string l;
  std::getline(is, l);

  auto bags = l.find(" bag");

  if (bags == std::string::npos)
    return is;

  line.outer = l.substr(0, bags);

  while (true) {
    auto const number =
        std::find_if(std::next(l.begin(), bags), l.end(),
                     [](auto c) { return '0' <= c && c <= '9'; });

    if (number == l.end())
      return is;

    bags = l.find(" bag", std::distance(l.begin(), number));

    if (bags == std::string::npos)
      return is;

    line.inner.push_back(
        {std::string(number + 2, std::next(l.begin(), bags)), *number - '0'});
  }

  return is;
}

} // namespace

TEST_CASE("day7") {
  std::istringstream is{test_input};
  std::vector const lines(std::istream_iterator<Line>{is},
                          std::istream_iterator<Line>{});

  std::map<std::string, std::set<std::string>> containers;

  for (auto const &line : lines) {
    for (auto const &contents : line.inner) {
      auto const it = containers.find(contents.kind);

      if (it == containers.cend())
        containers.emplace(contents.kind, std::set<std::string>{line.outer});
      else
        it->second.insert(line.outer);
    }
  }
}