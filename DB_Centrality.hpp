#pragma once

#include <array>
#include <cstddef>
#include <string_view>

namespace DB::Centrality {

// ## Centrality Classes ## //

inline constexpr std::size_t NClasses = 10;

inline constexpr std::array<std::string_view, NClasses> Name{"0-5",   "5-10",  "10-20", "20-30", "30-40",  //
                                                             "40-50", "50-60", "60-70", "70-80", "80-90"};

inline constexpr std::array<float, NClasses + 1> Edges{0., 5., 10., 20., 30., 40., 50., 60., 70., 80., 90.};

// Returns `NClasses` (one past the last valid index) for a centrality outside [0, 90).
[[nodiscard]] constexpr std::size_t ClassOf(float centrality) {
    for (std::size_t cc = 0; cc < NClasses; ++cc) {
        if (centrality >= Edges[cc] && centrality < Edges[cc + 1]) return cc;
    }
    return NClasses;
}

}  // namespace DB::Centrality
