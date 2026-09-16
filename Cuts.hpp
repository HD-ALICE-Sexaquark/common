#pragma once

#include <limits>
#include <span>
#include <string_view>
#include <vector>

namespace Cuts {

// A scalar selection on a `Cached::` object: keep (or, when `veto`, reject) the candidates whose `extract(c)` lies
// inside `[min, max]`. Both bounds are inclusive, so the boundary value itself is kept (or vetoed).
//
// - one-sided:        `.min = 20.` or `.max = 90.`
// - keep-window:      `.min = 20., .max = 90.`
// - exclusion band:   `.min = 0.48, .max = 0.51, .veto = true`
//
// Absolute-value cuts belong in the extractor (`std::abs(c.Fit_Pz())` + `.max`). `Common::DummyDouble` fails any
// `.min`, and its absolute value fails any `.max`; a veto band lets it through, unless the extractor maps it.
template <typename C>
struct Cut {
    std::string_view name;        // the cut-flow bin label
    double (*extract)(const C&);  // plain function pointer: a capture-less lambda converts
    double min{-std::numeric_limits<double>::infinity()};
    double max{+std::numeric_limits<double>::infinity()};
    bool veto{false};  // false: keep inside `[min, max]`; true: reject inside it
};

// The cuts of one stage, in the order they run. A view onto a namespace-scope `constexpr` array.
template <typename C>
using List = std::span<const Cut<C>>;

template <typename C>
[[nodiscard]] constexpr bool Passes(const Cut<C>& cut, const C& c) {
    const double value = cut.extract(c);
    const bool inside = value >= cut.min && value <= cut.max;
    return inside != cut.veto;
}

template <typename C>
[[nodiscard]] std::vector<std::string_view> NamesOf(List<C> list) {
    std::vector<std::string_view> names;
    names.reserve(list.size());
    for (const auto& cut : list) names.emplace_back(cut.name);
    return names;
}

}  // namespace Cuts
