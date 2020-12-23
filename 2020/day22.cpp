#include <catch2/catch.hpp>

#include <charconv>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace {
#if 1
std::array constexpr in_player_1{29, 25, 9,  1,  17, 28, 12, 49, 8,
                                 15, 41, 31, 39, 24, 40, 23, 6,  21,
                                 13, 45, 20, 2,  42, 47, 10};

std::array constexpr in_player_2{46, 27, 44, 18, 30, 50, 37, 11, 43,
                                 35, 34, 4,  22, 7,  33, 16, 36, 26,
                                 48, 19, 38, 14, 5,  3,  32};
#else
std::array constexpr in_player_1{9, 2, 6, 3, 1};
std::array constexpr in_player_2{5, 8, 4, 7, 10};
#endif

struct State {
  std::vector<int> player_1, player_2;
};

auto tie(State const &s) { return std::tie(s.player_1, s.player_2); }
auto operator<(State const &a, State const &b) { return tie(a) < tie(b); }

auto get_score(State const &state) {
  auto const &winner = state.player_1.empty() ? state.player_2 : state.player_1;

  int score = 0;

  for (size_t i = 0; i != winner.size(); ++i)
    score += winner[i] * (winner.size() - i);

  return score;
}

void push(std::vector<int> &winner, std::vector<int> &loser) {
  winner.push_back(winner.front());
  winner.push_back(loser.front());
  winner.erase(winner.cbegin());
  loser.erase(loser.cbegin());
}

State play_game(State state) {
  std::set<State> previous;

  while (!(state.player_1.empty() || state.player_2.empty())) {
    if (previous.contains(state))
      return State{state.player_1, {}};

    previous.insert(state);

    if (state.player_1.front() < state.player_1.size() &&
        state.player_2.front() < state.player_2.size()) {
      auto const subgame =
          play_game(State{std::vector(std::next(state.player_1.cbegin()),
                                      std::next(state.player_1.cbegin(),
                                                state.player_1.front() + 1)),
                          std::vector(std::next(state.player_2.cbegin()),
                                      std::next(state.player_2.cbegin(),
                                                state.player_2.front() + 1))});

      if (subgame.player_1.empty())
        push(state.player_2, state.player_1);
      else
        push(state.player_1, state.player_2);
    } else if (state.player_1.front() < state.player_2.front()) {
      push(state.player_2, state.player_1);
    } else {
      push(state.player_1, state.player_2);
    }
  }

  return state;
}
} // namespace

TEST_CASE("day22") {
#if 0
  auto const a = [&] {
    State state{std::vector(in_player_1.cbegin(), in_player_1.cend()),
                std::vector(in_player_2.cbegin(), in_player_2.cend())};

    while (!(state.player_1.empty() || state.player_2.empty())) {
      if (state.player_1.front() < state.player_2.front()) {
        push(state.player_2, state.player_1);
      } else {
        push(state.player_1, state.player_2);
      }
    }

    return get_score(state);
  }();

  auto const b = get_score(
      play_game(State{std::vector(in_player_1.cbegin(), in_player_1.cend()),
                      std::vector(in_player_2.cbegin(), in_player_2.cend())}));
#endif
}
