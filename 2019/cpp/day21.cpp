#include "aoc_intcode.hpp"

#include <catch2/catch.hpp>

#include <array>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <string>

namespace {
constexpr auto prog = std::array{
    109,   2050,  21101, 0,     966,   1,     21102, 13,    1,     0,     1105,
    1,     1378,  21101, 20,    0,     0,     1106,  0,     1337,  21101, 27,
    0,     0,     1106,  0,     1279,  1208,  1,     65,    748,   1005,  748,
    73,    1208,  1,     79,    748,   1005,  748,   110,   1208,  1,     78,
    748,   1005,  748,   132,   1208,  1,     87,    748,   1005,  748,   169,
    1208,  1,     82,    748,   1005,  748,   239,   21101, 0,     1041,  1,
    21102, 73,    1,     0,     1105,  1,     1421,  21102, 1,     78,    1,
    21102, 1041,  1,     2,     21102, 1,     88,    0,     1106,  0,     1301,
    21101, 0,     68,    1,     21102, 1,     1041,  2,     21101, 103,   0,
    0,     1105,  1,     1301,  1101,  0,     1,     750,   1105,  1,     298,
    21101, 82,    0,     1,     21102, 1,     1041,  2,     21101, 0,     125,
    0,     1106,  0,     1301,  1101,  0,     2,     750,   1105,  1,     298,
    21102, 79,    1,     1,     21101, 1041,  0,     2,     21101, 147,   0,
    0,     1105,  1,     1301,  21101, 84,    0,     1,     21101, 0,     1041,
    2,     21102, 162,   1,     0,     1105,  1,     1301,  1101,  0,     3,
    750,   1106,  0,     298,   21102, 65,    1,     1,     21101, 1041,  0,
    2,     21101, 0,     184,   0,     1106,  0,     1301,  21102, 1,     76,
    1,     21102, 1,     1041,  2,     21102, 1,     199,   0,     1105,  1,
    1301,  21102, 75,    1,     1,     21102, 1041,  1,     2,     21101, 0,
    214,   0,     1105,  1,     1301,  21102, 221,   1,     0,     1106,  0,
    1337,  21101, 0,     10,    1,     21101, 1041,  0,     2,     21101, 236,
    0,     0,     1106,  0,     1301,  1105,  1,     553,   21101, 0,     85,
    1,     21101, 1041,  0,     2,     21102, 254,   1,     0,     1105,  1,
    1301,  21102, 78,    1,     1,     21101, 0,     1041,  2,     21102, 1,
    269,   0,     1105,  1,     1301,  21101, 0,     276,   0,     1106,  0,
    1337,  21101, 10,    0,     1,     21101, 0,     1041,  2,     21102, 291,
    1,     0,     1105,  1,     1301,  1101,  0,     1,     755,   1105,  1,
    553,   21102, 1,     32,    1,     21102, 1041,  1,     2,     21101, 313,
    0,     0,     1105,  1,     1301,  21101, 320,   0,     0,     1106,  0,
    1337,  21101, 0,     327,   0,     1106,  0,     1279,  1201,  1,     0,
    749,   21102, 1,     65,    2,     21101, 0,     73,    3,     21101, 346,
    0,     0,     1106,  0,     1889,  1206,  1,     367,   1007,  749,   69,
    748,   1005,  748,   360,   1102,  1,     1,     756,   1001,  749,   -64,
    751,   1105,  1,     406,   1008,  749,   74,    748,   1006,  748,   381,
    1102,  -1,    1,     751,   1106,  0,     406,   1008,  749,   84,    748,
    1006,  748,   395,   1102,  -2,    1,     751,   1105,  1,     406,   21101,
    0,     1100,  1,     21101, 406,   0,     0,     1106,  0,     1421,  21102,
    32,    1,     1,     21102, 1,     1100,  2,     21102, 1,     421,   0,
    1106,  0,     1301,  21102, 1,     428,   0,     1106,  0,     1337,  21101,
    435,   0,     0,     1106,  0,     1279,  1202,  1,     1,     749,   1008,
    749,   74,    748,   1006,  748,   453,   1102,  1,     -1,    752,   1105,
    1,     478,   1008,  749,   84,    748,   1006,  748,   467,   1102,  -2,
    1,     752,   1105,  1,     478,   21102, 1168,  1,     1,     21101, 0,
    478,   0,     1105,  1,     1421,  21102, 485,   1,     0,     1105,  1,
    1337,  21101, 0,     10,    1,     21102, 1168,  1,     2,     21101, 0,
    500,   0,     1105,  1,     1301,  1007,  920,   15,    748,   1005,  748,
    518,   21101, 0,     1209,  1,     21102, 518,   1,     0,     1105,  1,
    1421,  1002,  920,   3,     529,   1001,  529,   921,   529,   1002,  750,
    1,     0,     1001,  529,   1,     537,   1002,  751,   1,     0,     1001,
    537,   1,     545,   102,   1,     752,   0,     1001,  920,   1,     920,
    1106,  0,     13,    1005,  755,   577,   1006,  756,   570,   21102, 1100,
    1,     1,     21101, 0,     570,   0,     1106,  0,     1421,  21101, 987,
    0,     1,     1106,  0,     581,   21101, 1001,  0,     1,     21101, 0,
    588,   0,     1106,  0,     1378,  1102,  1,     758,   594,   101,   0,
    0,     753,   1006,  753,   654,   20101, 0,     753,   1,     21102, 610,
    1,     0,     1105,  1,     667,   21101, 0,     0,     1,     21101, 621,
    0,     0,     1105,  1,     1463,  1205,  1,     647,   21102, 1,     1015,
    1,     21101, 635,   0,     0,     1105,  1,     1378,  21102, 1,     1,
    1,     21101, 0,     646,   0,     1105,  1,     1463,  99,    1001,  594,
    1,     594,   1106,  0,     592,   1006,  755,   664,   1102,  0,     1,
    755,   1106,  0,     647,   4,     754,   99,    109,   2,     1102,  726,
    1,     757,   22102, 1,     -1,    1,     21102, 1,     9,     2,     21102,
    1,     697,   3,     21101, 0,     692,   0,     1106,  0,     1913,  109,
    -2,    2106,  0,     0,     109,   2,     1002,  757,   1,     706,   1201,
    -1,    0,     0,     1001,  757,   1,     757,   109,   -2,    2106,  0,
    0,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     1,     1,
    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     255,
    63,    127,   159,   95,    223,   191,   0,     47,    203,   113,   251,
    42,    56,    249,   137,   54,    142,   175,   171,   79,    202,   118,
    46,    61,    167,   248,   253,   243,   110,   92,    229,   100,   178,
    227,   250,   125,   237,   102,   200,   136,   189,   123,   99,    172,
    206,   212,   154,   241,   186,   117,   245,   86,    188,   244,   162,
    77,    228,   222,   185,   35,    71,    168,   215,   70,    55,    115,
    199,   109,   38,    57,    51,    101,   139,   50,    231,   143,   155,
    196,   247,   205,   207,   152,   232,   58,    68,    39,    220,   239,
    60,    252,   214,   103,   181,   221,   217,   53,    204,   98,    177,
    170,   179,   213,   116,   94,    218,   169,   198,   216,   174,   114,
    187,   59,    254,   119,   166,   219,   138,   246,   120,   107,   141,
    121,   140,   157,   153,   158,   156,   182,   201,   84,    76,    190,
    235,   234,   233,   238,   122,   226,   87,    163,   34,    183,   93,
    106,   230,   69,    43,    184,   108,   236,   62,    85,    197,   242,
    173,   111,   124,   49,    78,    126,   0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     20,    73,
    110,   112,   117,   116,   32,    105,   110,   115,   116,   114,   117,
    99,    116,   105,   111,   110,   115,   58,    10,    13,    10,    87,
    97,    108,   107,   105,   110,   103,   46,    46,    46,    10,    10,
    13,    10,    82,    117,   110,   110,   105,   110,   103,   46,    46,
    46,    10,    10,    25,    10,    68,    105,   100,   110,   39,    116,
    32,    109,   97,    107,   101,   32,    105,   116,   32,    97,    99,
    114,   111,   115,   115,   58,    10,    10,    58,    73,    110,   118,
    97,    108,   105,   100,   32,    111,   112,   101,   114,   97,    116,
    105,   111,   110,   59,    32,    101,   120,   112,   101,   99,    116,
    101,   100,   32,    115,   111,   109,   101,   116,   104,   105,   110,
    103,   32,    108,   105,   107,   101,   32,    65,    78,    68,    44,
    32,    79,    82,    44,    32,    111,   114,   32,    78,    79,    84,
    67,    73,    110,   118,   97,    108,   105,   100,   32,    102,   105,
    114,   115,   116,   32,    97,    114,   103,   117,   109,   101,   110,
    116,   59,    32,    101,   120,   112,   101,   99,    116,   101,   100,
    32,    115,   111,   109,   101,   116,   104,   105,   110,   103,   32,
    108,   105,   107,   101,   32,    65,    44,    32,    66,    44,    32,
    67,    44,    32,    68,    44,    32,    74,    44,    32,    111,   114,
    32,    84,    40,    73,    110,   118,   97,    108,   105,   100,   32,
    115,   101,   99,    111,   110,   100,   32,    97,    114,   103,   117,
    109,   101,   110,   116,   59,    32,    101,   120,   112,   101,   99,
    116,   101,   100,   32,    74,    32,    111,   114,   32,    84,    52,
    79,    117,   116,   32,    111,   102,   32,    109,   101,   109,   111,
    114,   121,   59,    32,    97,    116,   32,    109,   111,   115,   116,
    32,    49,    53,    32,    105,   110,   115,   116,   114,   117,   99,
    116,   105,   111,   110,   115,   32,    99,    97,    110,   32,    98,
    101,   32,    115,   116,   111,   114,   101,   100,   0,     109,   1,
    1005,  1262,  1270,  3,     1262,  21001, 1262,  0,     0,     109,   -1,
    2105,  1,     0,     109,   1,     21102, 1288,  1,     0,     1106,  0,
    1263,  20101, 0,     1262,  0,     1101,  0,     0,     1262,  109,   -1,
    2106,  0,     0,     109,   5,     21102, 1,     1310,  0,     1106,  0,
    1279,  21202, 1,     1,     -2,    22208, -2,    -4,    -1,    1205,  -1,
    1332,  22102, 1,     -3,    1,     21102, 1,     1332,  0,     1105,  1,
    1421,  109,   -5,    2106,  0,     0,     109,   2,     21102, 1,     1346,
    0,     1106,  0,     1263,  21208, 1,     32,    -1,    1205,  -1,    1363,
    21208, 1,     9,     -1,    1205,  -1,    1363,  1105,  1,     1373,  21101,
    1370,  0,     0,     1105,  1,     1279,  1106,  0,     1339,  109,   -2,
    2106,  0,     0,     109,   5,     2101,  0,     -4,    1386,  20101, 0,
    0,     -2,    22101, 1,     -4,    -4,    21102, 0,     1,     -3,    22208,
    -3,    -2,    -1,    1205,  -1,    1416,  2201,  -4,    -3,    1408,  4,
    0,     21201, -3,    1,     -3,    1106,  0,     1396,  109,   -5,    2105,
    1,     0,     109,   2,     104,   10,    22102, 1,     -1,    1,     21102,
    1436,  1,     0,     1106,  0,     1378,  104,   10,    99,    109,   -2,
    2105,  1,     0,     109,   3,     20002, 594,   753,   -1,    22202, -1,
    -2,    -1,    201,   -1,    754,   754,   109,   -3,    2106,  0,     0,
    109,   10,    21101, 0,     5,     -5,    21101, 1,     0,     -4,    21102,
    0,     1,     -3,    1206,  -9,    1555,  21101, 0,     3,     -6,    21102,
    1,     5,     -7,    22208, -7,    -5,    -8,    1206,  -8,    1507,  22208,
    -6,    -4,    -8,    1206,  -8,    1507,  104,   64,    1106,  0,     1529,
    1205,  -6,    1527,  1201,  -7,    716,   1515,  21002, 0,     -11,   -8,
    21201, -8,    46,    -8,    204,   -8,    1106,  0,     1529,  104,   46,
    21201, -7,    1,     -7,    21207, -7,    22,    -8,    1205,  -8,    1488,
    104,   10,    21201, -6,    -1,    -6,    21207, -6,    0,     -8,    1206,
    -8,    1484,  104,   10,    21207, -4,    1,     -8,    1206,  -8,    1569,
    21102, 0,     1,     -9,    1106,  0,     1689,  21208, -5,    21,    -8,
    1206,  -8,    1583,  21102, 1,     1,     -9,    1106,  0,     1689,  1201,
    -5,    716,   1589,  20101, 0,     0,     -2,    21208, -4,    1,     -1,
    22202, -2,    -1,    -1,    1205,  -2,    1613,  21202, -5,    1,     1,
    21101, 0,     1613,  0,     1105,  1,     1444,  1206,  -1,    1634,  22101,
    0,     -5,    1,     21102, 1,     1627,  0,     1106,  0,     1694,  1206,
    1,     1634,  21101, 2,     0,     -3,    22107, 1,     -4,    -8,    22201,
    -1,    -8,    -8,    1206,  -8,    1649,  21201, -5,    1,     -5,    1206,
    -3,    1663,  21201, -3,    -1,    -3,    21201, -4,    1,     -4,    1106,
    0,     1667,  21201, -4,    -1,    -4,    21208, -4,    0,     -1,    1201,
    -5,    716,   1676,  22002, 0,     -1,    -1,    1206,  -1,    1686,  21101,
    0,     1,     -4,    1106,  0,     1477,  109,   -10,   2106,  0,     0,
    109,   11,    21101, 0,     0,     -6,    21102, 1,     0,     -8,    21102,
    1,     0,     -7,    20208, -6,    920,   -9,    1205,  -9,    1880,  21202,
    -6,    3,     -9,    1201,  -9,    921,   1724,  21001, 0,     0,     -5,
    1001,  1724,  1,     1732,  21001, 0,     0,     -4,    21201, -4,    0,
    1,     21102, 1,     1,     2,     21101, 9,     0,     3,     21102, 1754,
    1,     0,     1106,  0,     1889,  1206,  1,     1772,  2201,  -10,   -4,
    1766,  1001,  1766,  716,   1766,  21001, 0,     0,     -3,    1106,  0,
    1790,  21208, -4,    -1,    -9,    1206,  -9,    1786,  22102, 1,     -8,
    -3,    1106,  0,     1790,  21201, -7,    0,     -3,    1001,  1732,  1,
    1796,  20101, 0,     0,     -2,    21208, -2,    -1,    -9,    1206,  -9,
    1812,  22102, 1,     -8,    -1,    1106,  0,     1816,  21202, -7,    1,
    -1,    21208, -5,    1,     -9,    1205,  -9,    1837,  21208, -5,    2,
    -9,    1205,  -9,    1844,  21208, -3,    0,     -1,    1105,  1,     1855,
    22202, -3,    -1,    -1,    1106,  0,     1855,  22201, -3,    -1,    -1,
    22107, 0,     -1,    -1,    1106,  0,     1855,  21208, -2,    -1,    -9,
    1206,  -9,    1869,  22101, 0,     -1,    -8,    1106,  0,     1873,  22101,
    0,     -1,    -7,    21201, -6,    1,     -6,    1105,  1,     1708,  21202,
    -8,    1,     -10,   109,   -11,   2106,  0,     0,     109,   7,     22207,
    -6,    -5,    -3,    22207, -4,    -6,    -2,    22201, -3,    -2,    -1,
    21208, -1,    0,     -6,    109,   -7,    2105,  1,     0,     0,     109,
    5,     1202,  -2,    1,     1912,  21207, -4,    0,     -1,    1206,  -1,
    1930,  21102, 0,     1,     -4,    21201, -4,    0,     1,     22102, 1,
    -3,    2,     21102, 1,     1,     3,     21101, 1949,  0,     0,     1106,
    0,     1954,  109,   -5,    2106,  0,     0,     109,   6,     21207, -4,
    1,     -1,    1206,  -1,    1977,  22207, -5,    -3,    -1,    1206,  -1,
    1977,  21202, -5,    1,     -5,    1106,  0,     2045,  21202, -5,    1,
    1,     21201, -4,    -1,    2,     21202, -3,    2,     3,     21101, 0,
    1996,  0,     1105,  1,     1954,  21201, 1,     0,     -5,    21102, 1,
    1,     -2,    22207, -5,    -3,    -1,    1206,  -1,    2015,  21102, 0,
    1,     -2,    22202, -3,    -2,    -3,    22107, 0,     -4,    -1,    1206,
    -1,    2037,  21202, -2,    1,     1,     21101, 2037,  0,     0,     105,
    1,     1912,  21202, -3,    -1,    -3,    22201, -5,    -3,    -5,    109,
    -6,    2105,  1,     0};

template <typename Range> auto runProgram(Range &&input) {
  aoc::Interpreter comp{prog, input};

  int64_t lastOut{};

  while (auto result = comp.runUntilOutput())
    lastOut = *result;

  return lastOut;
}

constexpr char pt1[] = R"(OR A J
AND B J
AND C J
NOT J J
AND D J
WALK
)";

constexpr char pt2[] = R"(OR E J
OR H J
AND D J
OR A T
AND B T
AND C T
NOT T T
AND T J
RUN
)";

} // namespace

TEST_CASE("day21") {
  REQUIRE(runProgram(pt1) == 19357761);
  REQUIRE(runProgram(pt2) == 1142249706);
}
