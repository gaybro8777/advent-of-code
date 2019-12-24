#pragma once

namespace aoc {

template <typename Ts> struct [[nodiscard]] ScopeGuard final
    : Ts{~ScopeGuard() noexcept {Ts::operator()(); }
};

template <typename Ts> ScopeGuard(Ts)->ScopeGuard<Ts>;

}
