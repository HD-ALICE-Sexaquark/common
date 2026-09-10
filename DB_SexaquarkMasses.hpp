#pragma once

#include <array>
#include <cstddef>

namespace DB::SexaquarkMasses {

// ## Injected Antisexaquark Mass Hypotheses ## //

inline constexpr std::size_t NHypotheses = 5;

inline constexpr std::array<double, NHypotheses> Value{1.73, 1.80, 1.87, 1.94, 2.01};

inline constexpr double Tolerance = 0.01;  // (GeV/c^2) half-width of the window around a hypothesis

// Returns `NHypotheses` (one past the last valid index) for a mass matching no hypothesis.
[[nodiscard]] constexpr std::size_t IndexOf(double mass) {
    for (std::size_t mm = 0; mm < NHypotheses; ++mm) {
        if (mass > Value[mm] - Tolerance && mass < Value[mm] + Tolerance) return mm;
    }
    return NHypotheses;
}

}  // namespace DB::SexaquarkMasses
