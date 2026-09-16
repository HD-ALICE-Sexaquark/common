#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <format>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "Cached_Hdibaryon.hpp"
#include "Cached_Lambda.hpp"
#include "Cached_Unit.hpp"
#include "Cuts.hpp"

// -- Verifier Mode -- //
//
// A cut is one line: `{.name, .extract, .min, .max, .veto}` (see `Cuts.hpp`). Its name is its cut-flow bin label; the
// bins of a cut flow are the structural tests of the stage, then these, in order. A set bundles one list per stage
// plus the scalars the hand-written (structural) pre-seed tests need. Sets are picked with `t2ds verify --cuts <name>`.
//
// The commented lines are the inventory of cuts considered but not applied (PENDING: to tune).
//
// # Signal Region
// ## PreFoundLambda
//    |pz| < 3 GeV/c
//    pt < 4 GeV/c
//    |y| < 0.8
// ## (Anti)Protons
//    |pz| < 2.5 GeV/c
//    pt < 3.5 GeV
// ## Pi(Minus/Plus)
//    |pz| < 0.6 GeV/c
//    pt < 0.8 GeV
// ## (Anti)H-dibaryon
//    |pz| < 6 GeV/c
//    pt < 8 GeV
//    |y| < 0.8

namespace T2DS::CutSets::Verifier {

using Cached::Unit::PostSeed;

// ## pre-found (anti)lambdas ## //

// post-seed/pre-fit
inline constexpr auto Lambda_PreFit_Default = std::to_array<Cuts::Cut<PostSeed>>({
    // -- geometry
    {.name = "Seed_Max_DCA_BetweenDaughters", .extract = [](const PostSeed& c) { return c.DCA_Between(); }, .max = 10.},  // (cm)
    {.name = "Seed_Radius2D", .extract = [](const PostSeed& c) { return c.Radius2D(); }, .min = 0., .max = 75.},         // (cm)
    // -- kinematics
    {.name = "Seed_Mass", .extract = [](const PostSeed& c) { return c.Mass(); }, .min = 1.09, .max = 1.12},  // (GeV/c^2)
});

// post-fit
inline constexpr auto Lambda_PostFit_Default = std::to_array<Cuts::Cut<Cached::Lambda>>({
    // -- kinematics
    {.name = "Fit_AbsMax_Pz", .extract = [](const Cached::Lambda& c) { return std::abs(c.Fit_Pz()); }, .max = 3.},  // (GeV/c)
    {.name = "Fit_Max_Pt", .extract = [](const Cached::Lambda& c) { return c.Fit_Pt(); }, .max = 4.},               // (GeV/c)
    // {.name = "Fit_Min_Pt", .extract = [](const Cached::Lambda& c) { return c.Fit_Pt(); }, .min = 0.75},  // (GeV/c)
    {.name = "Fit_AbsMax_Rapidity", .extract = [](const Cached::Lambda& c) { return std::abs(c.Fit_Rapidity()); }, .max = 0.8},  // signal region = 0.8; goal = 0.5
    // {.name = "Fit_UnpinnedMass", .extract = [](const Cached::Lambda& c) { return c.Fit_UnpinnedMass(); }, .min = , .max = },
    // -- kinematics + geometry
    // {.name = "Min_CPA_WithRespectToPV", .extract = [](const Cached::Lambda& c) { return c.CPA_WithRespectToPV(); }, .min = 0.99},
    // {.name = "Max_DCA_WithRespectToPV", .extract = [](const Cached::Lambda& c) { return c.DCA_WithRespectToPV(); }, .max = 0.5},  // (cm)
    // {.name = "AbsMax_ArmRadiusDev", .extract = [](const Cached::Lambda& c) { return std::abs(c.Fit_ArmRadius_AsLambda() - 1.); }, .max = 0.1},
    // -- fit bookkeeping
    // {.name = "Max_Chi2NDF", .extract = [](const Cached::Lambda& c) { return c.Chi2NDF(); }, .max = 5.},
    // -- (anti)protons
    {.name = "Fit_AbsMax_Proton_Pz", .extract = [](const Cached::Lambda& c) { return std::abs(c.Fit_Proton_Pz()); }, .max = 2.5},  // (GeV/c)
    {.name = "Fit_Max_Proton_Pt", .extract = [](const Cached::Lambda& c) { return c.Fit_Proton_Pt(); }, .max = 3.5},               // (GeV/c)
    // {.name = "Fit_Min_Proton_Pt", .extract = [](const Cached::Lambda& c) { return c.Fit_Proton_Pt(); }, .min = 0.75},  // (GeV/c)
    // -- pi(minus/plus)
    {.name = "Fit_AbsMax_Pion_Pz", .extract = [](const Cached::Lambda& c) { return std::abs(c.Fit_Pion_Pz()); }, .max = 0.6},  // (GeV/c)
    {.name = "Fit_Max_Pion_Pt", .extract = [](const Cached::Lambda& c) { return c.Fit_Pion_Pt(); }, .max = 0.8},               // (GeV/c)
    // {.name = "Fit_Min_Pion_Pt", .extract = [](const Cached::Lambda& c) { return c.Fit_Pion_Pt(); }, .min = 0.2},  // (GeV/c)
});

// ## (anti)lambda pairs ## //

// post-seed/pre-fit
inline constexpr auto LambdaPair_PreFit_Default = std::to_array<Cuts::Cut<PostSeed>>({
    // -- geometry
    {.name = "Seed_Max_DCA_BetweenLambdas", .extract = [](const PostSeed& c) { return c.DCA_Between(); }, .max = 10.},  // (cm)
    {.name = "Seed_Max_Radius2D", .extract = [](const PostSeed& c) { return c.Radius2D(); }, .max = 10.},               // (cm)
});

// post-fit
inline constexpr auto LambdaPair_PostFit_Default = std::to_array<Cuts::Cut<Cached::Hdibaryon>>({
    // -- kinematics
    {.name = "Fit_AbsMax_Pz", .extract = [](const Cached::Hdibaryon& c) { return std::abs(c.Fit_Pz()); }, .max = 6.},  // (GeV/c)
    // {.name = "Fit_Min_Pt", .extract = [](const Cached::Hdibaryon& c) { return c.Fit_Pt(); }, .min = 2.5},  // (GeV/c)
    {.name = "Fit_Max_Pt", .extract = [](const Cached::Hdibaryon& c) { return c.Fit_Pt(); }, .max = 8.},  // (GeV/c)
    // {.name = "Fit_Min_Mass", .extract = [](const Cached::Hdibaryon& c) { return c.Fit_Mass(); }, .min = 2.22},  // (GeV/c^2)
    {.name = "Fit_Max_Mass", .extract = [](const Cached::Hdibaryon& c) { return c.Fit_Mass(); }, .max = 2.24},  // (GeV/c^2)
    // {.name = "Fit_AbsMax_Rapidity", .extract = [](const Cached::Hdibaryon& c) { return std::abs(c.Fit_Rapidity()); }, .max = 0.8},  // signal region = 0.8; goal = 0.5
    // -- kinematics + geometry
    // {.name = "Max_FlightLength_WithRespectToPV", .extract = [](const Cached::Hdibaryon& c) { return c.FlightLength_WithRespectToPV(); }, .max = 1.},  // (cm); PENDING: re-tune
    // {.name = "Min_CPA_WithRespectToPV", .extract = [](const Cached::Hdibaryon& c) { return c.CPA_WithRespectToPV(); }, .min = 0.99},
    // -- fit bookkeeping
    // {.name = "Max_Chi2NDF", .extract = [](const Cached::Hdibaryon& c) { return c.Chi2NDF(); }, .max = 2.5},
    // {.name = "Max_Chi2PVC", .extract = [](const Cached::Hdibaryon& c) { return c.Chi2PVC(); }, .max = 5.},
    // -- (anti)lambdas w.r.t. the (anti)h-dibaryon decay vertex
    // {.name = "Max_L1_FlightLength_WithRespectToDV", .extract = [](const Cached::Hdibaryon& c) { return c.L1_FlightLength_WithRespectToDV(); }, .max = 75.},  // (cm)
    // {.name = "Min_L1_FlightLength_WithRespectToDV", .extract = [](const Cached::Hdibaryon& c) { return c.L1_FlightLength_WithRespectToDV(); }, .min = 7.5},  // (cm)
    // {.name = "Min_L1_CPA_WithRespectToDV", .extract = [](const Cached::Hdibaryon& c) { return c.L1_CPA_WithRespectToDV(); }, .min = 0.99},
    // {.name = "Max_L2_FlightLength_WithRespectToDV", .extract = [](const Cached::Hdibaryon& c) { return c.L2_FlightLength_WithRespectToDV(); }, .max = 75.},  // (cm)
    // {.name = "Min_L2_FlightLength_WithRespectToDV", .extract = [](const Cached::Hdibaryon& c) { return c.L2_FlightLength_WithRespectToDV(); }, .min = 7.5},  // (cm)
    // {.name = "Min_L2_CPA_WithRespectToDV", .extract = [](const Cached::Hdibaryon& c) { return c.L2_CPA_WithRespectToDV(); }, .min = 0.99},
});

// ## the sets ## //

struct Set {
    // pre-seed, structural (hand-written tests)
    // -- tracking can split one physical particle into two ESD entries; these tell them apart
    double Max_TracksDeltaR;     // PENDING: to tune
    double Max_TracksRelDeltaP;  // PENDING: to tune
    // -- distance between the (anti)lambdas' decay vertices
    double Min_Distance_BetweenLambdasDVs;  // (cm) PENDING: to tune
    // -- invariant mass of the two fitted (anti)lambdas, before seeding
    double Min_PreSeed_Mass;  // (GeV/c^2)
    double Max_PreSeed_Mass;  // (GeV/c^2)
    // pre-found (anti)lambdas
    Cuts::List<PostSeed> Lambda_PreFit;
    Cuts::List<Cached::Lambda> Lambda_PostFit;
    // (anti)lambda pairs
    Cuts::List<PostSeed> LambdaPair_PreFit;
    Cuts::List<Cached::Hdibaryon> LambdaPair_PostFit;
};

inline constexpr Set Default{
    .Max_TracksDeltaR = 0.02,
    .Max_TracksRelDeltaP = 0.05,
    .Min_Distance_BetweenLambdasDVs = 0.01,
    .Min_PreSeed_Mass = 2.2,
    .Max_PreSeed_Mass = 2.4,
    .Lambda_PreFit = Lambda_PreFit_Default,
    .Lambda_PostFit = Lambda_PostFit_Default,
    .LambdaPair_PreFit = LambdaPair_PreFit_Default,
    .LambdaPair_PostFit = LambdaPair_PostFit_Default,
};

// ## the registry ## //

inline constexpr auto Registry = std::to_array<std::pair<std::string_view, const Set*>>({
    {"default", &Default},
});

inline constexpr auto Names = [] {
    std::array<std::string_view, Registry.size()> names{};
    for (std::size_t i = 0; i < Registry.size(); ++i) names[i] = Registry[i].first;
    return names;
}();

[[nodiscard]] inline const Set& Get(std::string_view name) {
    for (const auto& [set_name, set] : Registry) {
        if (set_name == name) return *set;
    }
    throw std::runtime_error(std::format("unknown verifier cut set \"{}\"", name));
}

}  // namespace T2DS::CutSets::Verifier
