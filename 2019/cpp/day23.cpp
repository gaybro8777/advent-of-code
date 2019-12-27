#include "intcode.hpp"

#include <catch2/catch.hpp>

#include <array>
#include <iostream>

namespace {
constexpr int64_t program[] {3,62,1001,62,11,10,109,2247,105,1,0,641,1773,1571,1189,1509,1313,1540,717,2113,1911,1350,1668,1113,748,1255,571,1701,1808,1381,1051,606,958,1639,989,1224,1478,896,1604,810,779,2146,1878,929,682,1985,1284,861,1443,2214,1412,1152,1950,2177,2084,1020,2018,1841,1732,1082,2051,0,0,0,0,0,0,0,0,0,0,0,0,3,64,1008,64,-1,62,1006,62,88,1006,61,170,1106,0,73,3,65,20102,1,64,1,20102,1,66,2,21101,0,105,0,1106,0,436,1201,1,-1,64,1007,64,0,62,1005,62,73,7,64,67,62,1006,62,73,1002,64,2,133,1,133,68,133,102,1,0,62,1001,133,1,140,8,0,65,63,2,63,62,62,1005,62,73,1002,64,2,161,1,161,68,161,1101,0,1,0,1001,161,1,169,102,1,65,0,1102,1,1,61,1102,0,1,63,7,63,67,62,1006,62,203,1002,63,2,194,1,68,194,194,1006,0,73,1001,63,1,63,1105,1,178,21102,1,210,0,106,0,69,1202,1,1,70,1101,0,0,63,7,63,71,62,1006,62,250,1002,63,2,234,1,72,234,234,4,0,101,1,234,240,4,0,4,70,1001,63,1,63,1105,1,218,1106,0,73,109,4,21101,0,0,-3,21102,0,1,-2,20207,-2,67,-1,1206,-1,293,1202,-2,2,283,101,1,283,283,1,68,283,283,22001,0,-3,-3,21201,-2,1,-2,1106,0,263,21201,-3,0,-3,109,-4,2106,0,0,109,4,21102,1,1,-3,21102,0,1,-2,20207,-2,67,-1,1206,-1,342,1202,-2,2,332,101,1,332,332,1,68,332,332,22002,0,-3,-3,21201,-2,1,-2,1106,0,312,21202,-3,1,-3,109,-4,2105,1,0,109,1,101,1,68,358,21001,0,0,1,101,3,68,366,21001,0,0,2,21101,0,376,0,1106,0,436,21202,1,1,0,109,-1,2105,1,0,1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,4294967296,8589934592,17179869184,34359738368,68719476736,137438953472,274877906944,549755813888,1099511627776,2199023255552,4398046511104,8796093022208,17592186044416,35184372088832,70368744177664,140737488355328,281474976710656,562949953421312,1125899906842624,109,8,21202,-6,10,-5,22207,-7,-5,-5,1205,-5,521,21101,0,0,-4,21102,0,1,-3,21102,51,1,-2,21201,-2,-1,-2,1201,-2,385,471,20102,1,0,-1,21202,-3,2,-3,22207,-7,-1,-5,1205,-5,496,21201,-3,1,-3,22102,-1,-1,-5,22201,-7,-5,-7,22207,-3,-6,-5,1205,-5,515,22102,-1,-6,-5,22201,-3,-5,-3,22201,-1,-4,-4,1205,-2,461,1105,1,547,21102,1,-1,-4,21202,-6,-1,-6,21207,-7,0,-5,1205,-5,547,22201,-7,-6,-7,21201,-4,1,-4,1106,0,529,21201,-4,0,-7,109,-8,2106,0,0,109,1,101,1,68,563,21001,0,0,0,109,-1,2105,1,0,1101,63493,0,66,1102,3,1,67,1102,598,1,68,1101,0,302,69,1101,1,0,71,1101,604,0,72,1105,1,73,0,0,0,0,0,0,2,79367,1102,1,81559,66,1101,0,3,67,1101,0,633,68,1102,302,1,69,1102,1,1,71,1101,0,639,72,1106,0,73,0,0,0,0,0,0,42,67763,1101,9199,0,66,1102,1,1,67,1102,1,668,68,1101,556,0,69,1101,0,6,71,1102,670,1,72,1105,1,73,1,20268,2,158734,41,47578,41,71367,36,18089,36,36178,36,54267,1102,72467,1,66,1102,1,3,67,1101,0,709,68,1101,0,302,69,1101,1,0,71,1102,715,1,72,1105,1,73,0,0,0,0,0,0,42,135526,1101,0,20353,66,1101,1,0,67,1101,0,744,68,1101,0,556,69,1102,1,1,71,1102,746,1,72,1105,1,73,1,34703,33,72467,1101,0,59197,66,1101,0,1,67,1102,1,775,68,1102,1,556,69,1101,1,0,71,1101,777,0,72,1106,0,73,1,13,9,243844,1101,0,96289,66,1101,1,0,67,1101,806,0,68,1102,1,556,69,1101,0,1,71,1101,0,808,72,1106,0,73,1,28,37,44782,1102,1,63667,66,1101,0,1,67,1101,837,0,68,1101,556,0,69,1102,11,1,71,1101,0,839,72,1106,0,73,1,1,33,217401,27,72379,1,20399,20,81559,37,22391,11,3467,45,84794,34,93253,8,47981,15,63493,46,197331,1102,1,18089,66,1102,1,3,67,1101,0,888,68,1102,1,302,69,1102,1,1,71,1102,894,1,72,1105,1,73,0,0,0,0,0,0,5,324692,1101,93851,0,66,1101,2,0,67,1101,0,923,68,1102,1,302,69,1101,0,1,71,1102,927,1,72,1106,0,73,0,0,0,0,9,60961,1102,89269,1,66,1101,1,0,67,1102,956,1,68,1102,1,556,69,1102,0,1,71,1101,0,958,72,1105,1,73,1,1100,1101,88651,0,66,1101,0,1,67,1102,1,985,68,1101,0,556,69,1101,1,0,71,1102,987,1,72,1105,1,73,1,-182,46,65777,1102,1,48079,66,1101,0,1,67,1102,1,1016,68,1101,556,0,69,1101,1,0,71,1101,1018,0,72,1105,1,73,1,-135,33,144934,1101,0,29789,66,1101,0,1,67,1102,1,1047,68,1102,556,1,69,1101,0,1,71,1102,1049,1,72,1106,0,73,1,177,27,217137,1101,0,54779,66,1101,0,1,67,1101,1078,0,68,1102,556,1,69,1101,1,0,71,1102,1,1080,72,1106,0,73,1,1117,37,67173,1102,41953,1,66,1102,1,1,67,1101,0,1109,68,1102,1,556,69,1101,0,1,71,1101,0,1111,72,1106,0,73,1,125,40,17786,1102,1,36151,66,1101,0,1,67,1102,1,1140,68,1101,556,0,69,1101,5,0,71,1102,1142,1,72,1106,0,73,1,2,49,1514,26,187702,9,182883,47,175179,47,233572,1102,1,8893,66,1102,4,1,67,1102,1,1179,68,1102,1,302,69,1102,1,1,71,1102,1187,1,72,1106,0,73,0,0,0,0,0,0,0,0,47,350358,1102,86467,1,66,1102,1,1,67,1101,1216,0,68,1101,556,0,69,1101,3,0,71,1102,1218,1,72,1105,1,73,1,5,40,26679,40,35572,47,58393,1101,73771,0,66,1101,0,1,67,1101,0,1251,68,1101,556,0,69,1101,1,0,71,1102,1253,1,72,1105,1,73,1,160,47,291965,1101,0,56093,66,1102,1,1,67,1101,0,1282,68,1102,556,1,69,1102,0,1,71,1101,1284,0,72,1106,0,73,1,1760,1102,87877,1,66,1101,1,0,67,1101,1311,0,68,1101,556,0,69,1102,0,1,71,1102,1,1313,72,1105,1,73,1,1198,1102,1,81173,66,1101,4,0,67,1102,1,1340,68,1101,253,0,69,1102,1,1,71,1102,1348,1,72,1105,1,73,0,0,0,0,0,0,0,0,17,68399,1102,1,81457,66,1102,1,1,67,1102,1377,1,68,1101,556,0,69,1102,1,1,71,1101,0,1379,72,1106,0,73,1,14437,1,61197,1102,1,84463,66,1101,0,1,67,1102,1408,1,68,1102,1,556,69,1102,1,1,71,1101,1410,0,72,1106,0,73,1,53233,27,144758,1102,3767,1,66,1102,1,1,67,1102,1,1439,68,1102,1,556,69,1101,1,0,71,1102,1,1441,72,1105,1,73,1,691,20,244677,1102,22391,1,66,1102,1,3,67,1102,1,1470,68,1102,302,1,69,1101,1,0,71,1101,1476,0,72,1106,0,73,0,0,0,0,0,0,15,126986,1101,103591,0,66,1101,0,1,67,1101,0,1505,68,1102,556,1,69,1101,1,0,71,1102,1,1507,72,1105,1,73,1,39313,11,6934,1102,1,92951,66,1102,1,1,67,1102,1,1536,68,1102,1,556,69,1102,1,1,71,1101,1538,0,72,1105,1,73,1,43753,20,163118,1101,98561,0,66,1102,1,1,67,1102,1567,1,68,1101,0,556,69,1101,1,0,71,1101,0,1569,72,1106,0,73,1,7,9,304805,1101,79367,0,66,1102,1,2,67,1102,1,1598,68,1101,302,0,69,1101,1,0,71,1102,1,1602,72,1106,0,73,0,0,0,0,5,243519,1102,1,72379,66,1101,3,0,67,1101,0,1631,68,1102,302,1,69,1101,1,0,71,1102,1,1637,72,1105,1,73,0,0,0,0,0,0,42,203289,1101,30071,0,66,1102,1,1,67,1101,0,1666,68,1102,556,1,69,1102,0,1,71,1102,1668,1,72,1106,0,73,1,1991,1102,1,3467,66,1102,1,2,67,1102,1695,1,68,1102,1,302,69,1102,1,1,71,1101,1699,0,72,1105,1,73,0,0,0,0,45,42397,1102,1,18461,66,1102,1,1,67,1102,1,1728,68,1101,556,0,69,1102,1,1,71,1101,0,1730,72,1106,0,73,1,-284,1,40798,1102,58393,1,66,1101,6,0,67,1102,1759,1,68,1101,0,302,69,1102,1,1,71,1101,0,1771,72,1106,0,73,0,0,0,0,0,0,0,0,0,0,0,0,17,136798,1102,20399,1,66,1102,1,3,67,1101,1800,0,68,1101,302,0,69,1101,0,1,71,1101,1806,0,72,1105,1,73,0,0,0,0,0,0,42,271052,1101,68399,0,66,1102,1,2,67,1101,1835,0,68,1102,351,1,69,1101,0,1,71,1102,1839,1,72,1105,1,73,0,0,0,0,255,9199,1102,65777,1,66,1102,1,4,67,1101,1868,0,68,1102,1,302,69,1101,1,0,71,1101,1876,0,72,1106,0,73,0,0,0,0,0,0,0,0,41,23789,1102,1,72367,66,1101,1,0,67,1102,1905,1,68,1102,1,556,69,1102,1,2,71,1101,0,1907,72,1105,1,73,1,73,9,121922,46,263108,1101,60961,0,66,1101,0,5,67,1102,1,1938,68,1101,302,0,69,1101,0,1,71,1101,1948,0,72,1106,0,73,0,0,0,0,0,0,0,0,0,0,5,81173,1101,0,23789,66,1102,1,3,67,1102,1,1977,68,1102,1,302,69,1101,0,1,71,1101,1983,0,72,1105,1,73,0,0,0,0,0,0,5,162346,1102,93253,1,66,1102,2,1,67,1102,1,2012,68,1102,1,302,69,1101,1,0,71,1102,2016,1,72,1105,1,73,0,0,0,0,8,95962,1102,1,42397,66,1102,1,2,67,1102,1,2045,68,1101,0,302,69,1101,0,1,71,1102,1,2049,72,1105,1,73,0,0,0,0,34,186506,1102,1,757,66,1101,2,0,67,1102,1,2078,68,1101,0,302,69,1101,1,0,71,1102,2082,1,72,1106,0,73,0,0,0,0,26,93851,1102,58363,1,66,1102,1,1,67,1101,0,2111,68,1102,556,1,69,1101,0,0,71,1101,0,2113,72,1105,1,73,1,1191,1101,47981,0,66,1101,0,2,67,1102,2140,1,68,1101,302,0,69,1102,1,1,71,1101,2144,0,72,1105,1,73,0,0,0,0,15,190479,1101,54973,0,66,1101,1,0,67,1101,0,2173,68,1101,556,0,69,1101,0,1,71,1102,2175,1,72,1106,0,73,1,3,46,131554,1102,1,67763,66,1101,0,4,67,1101,0,2204,68,1102,253,1,69,1101,1,0,71,1101,2212,0,72,1106,0,73,0,0,0,0,0,0,0,0,49,757,1102,1,16651,66,1102,1,1,67,1102,2241,1,68,1101,0,556,69,1101,0,2,71,1102,2243,1,72,1105,1,73,1,10,40,8893,47,116786};

template <size_t... Ix> auto getInterpreters(std::index_sequence<Ix...>) {
  return std::array{aoc::Interpreter{program, std::array{Ix}}...};
}

auto getInterpreters() {
  return getInterpreters(std::make_index_sequence<50>{});
}

auto part1() {
  auto network{getInterpreters()};

  for (;;) {
    for (auto &comp : network) {
      if (const auto out = std::visit(
              aoc::Overloaded{
                  [&](aoc::Interpreter::Success s) -> std::optional<int64_t> {
                    const auto addr = s.value;
                    const auto x = comp.runUntilOutput();
                    const auto y = comp.runUntilOutput();

                    if (addr < 0 || network.size() <= addr)
                      return *y;

                    network[addr].queueInputs(std::array{*x, *y});
                    return {};
                  },
                  [&](aoc::Interpreter::NoInput) -> std::optional<int64_t> {
                    comp.queueInputs(std::array{-1});
                    return {};
                  },
                  [](auto) -> std::optional<int64_t> { return {}; },
              },
              comp.step()))
        return *out;
    }
  }
}

auto part2() {
  auto network{getInterpreters()};

  int64_t lastNatY{};
  int64_t natX{};
  int64_t natY{};
  int64_t idleTimer{};

  for (;;) {
    for (auto &comp : network) {
      std::visit(aoc::Overloaded{
                     [&](aoc::Interpreter::Success s) {
                       idleTimer = 0;
                       const auto addr = s.value;
                       const auto x = comp.runUntilOutput();
                       const auto y = comp.runUntilOutput();

                       if (addr < 0 || network.size() <= addr) {
                         natX = *x;
                         natY = *y;
                         return;
                       }

                       network[addr].queueInputs(std::array{*x, *y});
                     },
                     [&](aoc::Interpreter::NoInput) {
                       comp.queueInputs(std::array{-1});
                       idleTimer += 1;
                     },
                     [&](auto) { idleTimer += 1; },
                 },
                 comp.step());
    }

    if (idleTimer > 100000) {
      idleTimer = 0;
      network[0].queueInputs(std::array{natX, natY});
      if (lastNatY == natY)
        return natY;

      lastNatY = natY;
    }
  }
}
} // namespace

TEST_CASE("day23") {
  REQUIRE(part1() == 18513);
  REQUIRE(part2() == 13286);
}