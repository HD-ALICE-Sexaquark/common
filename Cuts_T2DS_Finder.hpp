#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <format>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "Cached_ChannelA.hpp"
#include "Cached_ChannelD.hpp"
#include "Cached_ChannelH.hpp"
#include "Cached_Track.hpp"
#include "Cached_Unit.hpp"
#include "Cached_V0.hpp"
#include "Cuts.hpp"

// -- Finder Mode -- //
//
// A cut is one line: `{.name, .extract, .min, .max, .veto}` (see `Cuts.hpp`). Its name is its cut-flow bin label; the
// bins of a cut flow are the structural tests of the stage, then these, in order. A set bundles one list per stage
// plus the scalars the hand-written (structural) pre-seed tests need. Sets are picked with `t2ds find --cuts <name>`.
//
// The commented lines are the inventory of cuts considered but not applied (PENDING: to tune).

namespace T2DS::CutSets::Finder {

using Cached::Unit::PostSeed;

// ## tracks ## //

// (anti)protons
inline constexpr auto Proton_Default = std::to_array<Cuts::Cut<Cached::Track>>({
    {.name = "AbsMax_NSigmasProton", .extract = [](const Cached::Track& c) { return c.AbsNSigmasProton(); }, .max = 3.},  // mandatory for classification
    // {.name = "Min_DCAxy", .extract = [](const Cached::Track& c) { return c.DCAxy(); }, .min = 30.},
});

// charged kaons
inline constexpr auto Kaon_Default = std::to_array<Cuts::Cut<Cached::Track>>({
    {.name = "AbsMax_NSigmasKaon", .extract = [](const Cached::Track& c) { return c.AbsNSigmasKaon(); }, .max = 3.},  // mandatory for classification
    // -- misID vetoes
    // {.name = "AbsMin_NSigmasPion", .extract = [](const Cached::Track& c) { return c.AbsNSigmasPion(); }, .min = },
    // {.name = "AbsMin_NSigmasProton", .extract = [](const Cached::Track& c) { return c.AbsNSigmasProton(); }, .min = },
    // -- track quality
    // {.name = "Min_TPC_NCrossedRows", .extract = [](const Cached::Track& c) { return static_cast<double>(c.TPC_NCrossedRows); }, .min = },
    // {.name = "Max_TPC_Chi2_NCls", .extract = [](const Cached::Track& c) { return c.TPC_Chi2_NCls(); }, .max = },
    // -- anti-pointing
    // {.name = "Min_DCAxy", .extract = [](const Cached::Track& c) { return c.DCAxy(); }, .min = },
    // {.name = "Min_DCAz", .extract = [](const Cached::Track& c) { return c.DCAz(); }, .min = },
});

// charged pions
inline constexpr auto Pion_Default = std::to_array<Cuts::Cut<Cached::Track>>({
    {.name = "AbsMax_NSigmasPion", .extract = [](const Cached::Track& c) { return c.AbsNSigmasPion(); }, .max = 3.},  // mandatory for classification
    // {.name = "Min_DCAxy", .extract = [](const Cached::Track& c) { return c.DCAxy(); }, .min = 30.},
});

// ## (anti)lambdas ## //

// post-seed/pre-fit
inline constexpr auto Lambda_PreFit_Default = std::to_array<Cuts::Cut<PostSeed>>({
    // -- geometry
    {.name = "Seed_Max_DCA_BetweenDaughters", .extract = [](const PostSeed& c) { return c.DCA_Between(); }, .max = 2.},  // (cm)
    {.name = "Seed_Min_Radius2D", .extract = [](const PostSeed& c) { return c.Radius2D(); }, .min = 25.},              // (cm)
    {.name = "Seed_Max_Radius2D", .extract = [](const PostSeed& c) { return c.Radius2D(); }, .max = 180.},             // (cm)
    // -- kinematics
    {.name = "Seed_Min_Mass", .extract = [](const PostSeed& c) { return c.Mass(); }, .min = 1.105},  // (GeV/c^2)
    {.name = "Seed_Max_Mass", .extract = [](const PostSeed& c) { return c.Mass(); }, .max = 1.125},  // (GeV/c^2)
    // {.name = "Seed_AbsMax_ArmAlpha", .extract = [](const PostSeed& c) { return std::abs(c.ArmAlpha()); }, .max = },
    // {.name = "Seed_Max_ArmQt", .extract = [](const PostSeed& c) { return c.ArmQt(); }, .max = },
    // {.name = "Seed_Veto_Mass_AsK0S", .extract = [](const PostSeed& c) { return c.Mass_AsK0S(); }, .min = , .max = , .veto = true},
    //   > daughters' kinematics (`Dau1` = neg, `Dau2` = pos)
    // {.name = "Seed_Min_Neg_Pt", .extract = [](const PostSeed& c) { return c.Dau1_Pt(); }, .min = },
    // {.name = "Seed_AbsMax_Neg_Eta", .extract = [](const PostSeed& c) { return std::abs(c.Dau1_Eta()); }, .max = },
    // {.name = "Seed_Min_Pos_Pt", .extract = [](const PostSeed& c) { return c.Dau2_Pt(); }, .min = },
    // {.name = "Seed_AbsMax_Pos_Eta", .extract = [](const PostSeed& c) { return std::abs(c.Dau2_Eta()); }, .max = },
});

// post-fit
inline constexpr auto Lambda_PostFit_Default = std::to_array<Cuts::Cut<Cached::V0>>({
    // -- kinematics
    // {.name = "Fit_AbsMax_Pz", .extract = [](const Cached::V0& c) { return std::abs(c.Fit_Pz()); }, .max = },
    // {.name = "Fit_Max_Pt", .extract = [](const Cached::V0& c) { return c.Fit_Pt(); }, .max = },
    {.name = "Fit_AbsMax_Rapidity", .extract = [](const Cached::V0& c) { return std::abs(c.Fit_Rapidity()); }, .max = 1.},
    // {.name = "Fit_AbsMax_ArmAlpha", .extract = [](const Cached::V0& c) { return std::abs(c.Fit_ArmAlpha()); }, .max = },
    // {.name = "Fit_Max_ArmQt", .extract = [](const Cached::V0& c) { return c.Fit_ArmQt(); }, .max = },
    {.name = "Fit_AbsMax_ArmQtOverAlpha", .extract = [](const Cached::V0& c) { return std::abs(c.Fit_ArmQtOverAlpha()); }, .max = 0.2},  // loose lambda+antilambda triangular selection
    // {.name = "Fit_Veto_Mass_AsK0S", .extract = [](const Cached::V0& c) { return c.Fit_Mass_AsK0S(); }, .min = , .max = , .veto = true},
    // {.name = "Fit_Veto_Mass_AsPhoton", .extract = [](const Cached::V0& c) { return c.Fit_Mass_AsPhoton(); }, .max = , .veto = true},
    // {.name = "Fit_Veto_ArmRadius_AsK0S", .extract = [](const Cached::V0& c) { return c.Fit_ArmRadius_AsK0S(); }, .min = , .max = , .veto = true},
    // -- geometry
    {.name = "Min_Decay_Radius2D", .extract = [](const Cached::V0& c) { return c.Decay_Radius2D(); }, .min = 20.},  // (cm)
    // -- kinematics + geometry
    {.name = "Min_DCA_WithRespectToPV", .extract = [](const Cached::V0& c) { return c.DCA_WithRespectToPV(); }, .min = 10.},  // (cm)
    // -- fit bookkeeping
    {.name = "Max_Chi2Vtx", .extract = [](const Cached::V0& c) { return c.Chi2Vtx(); }, .max = 35.},
    // -- mass-related
    // {.name = "Fit_UnpinnedMass", .extract = [](const Cached::V0& c) { return c.Fit_UnpinnedMass(); }, .min = , .max = },
    {.name = "AbsMax_MassPull", .extract = [](const Cached::V0& c) { return std::abs(static_cast<double>(c.Fit_MassPull)); }, .max = 10.},
    // {.name = "Max_MassErr", .extract = [](const Cached::V0& c) { return static_cast<double>(c.Fit_MassErr); }, .max = },
});

// ## kaons zero short ## //

// post-seed/pre-fit
inline constexpr auto KaonZeroShort_PreFit_Default = std::to_array<Cuts::Cut<PostSeed>>({
    // -- geometry
    {.name = "Seed_Max_DCA_BetweenDaughters", .extract = [](const PostSeed& c) { return c.DCA_Between(); }, .max = 2.},  // (cm)
    {.name = "Seed_Min_Radius2D", .extract = [](const PostSeed& c) { return c.Radius2D(); }, .min = 20.},              // (cm)
    {.name = "Seed_Max_Radius2D", .extract = [](const PostSeed& c) { return c.Radius2D(); }, .max = 180.},             // (cm)
    // -- kinematics
    {.name = "Seed_Min_Mass", .extract = [](const PostSeed& c) { return c.Mass(); }, .min = 0.45},  // (GeV/c^2)
    {.name = "Seed_Max_Mass", .extract = [](const PostSeed& c) { return c.Mass(); }, .max = 0.55},  // (GeV/c^2)
    // {.name = "Seed_AbsMax_ArmAlpha", .extract = [](const PostSeed& c) { return std::abs(c.ArmAlpha()); }, .max = },
    // {.name = "Seed_Max_ArmQt", .extract = [](const PostSeed& c) { return c.ArmQt(); }, .max = },
    // {.name = "Seed_Veto_Mass_AsLambda", .extract = [](const PostSeed& c) { return c.Mass_AsLambda(); }, .min = , .max = , .veto = true},
    // {.name = "Seed_Veto_Mass_AsAntiLambda", .extract = [](const PostSeed& c) { return c.Mass_AsAntiLambda(); }, .min = , .max = , .veto = true},
    //   > daughters' kinematics (`Dau1` = neg, `Dau2` = pos)
    // {.name = "Seed_Min_Neg_Pt", .extract = [](const PostSeed& c) { return c.Dau1_Pt(); }, .min = },
    // {.name = "Seed_AbsMax_Neg_Eta", .extract = [](const PostSeed& c) { return std::abs(c.Dau1_Eta()); }, .max = },
    // {.name = "Seed_Min_Pos_Pt", .extract = [](const PostSeed& c) { return c.Dau2_Pt(); }, .min = },
    // {.name = "Seed_AbsMax_Pos_Eta", .extract = [](const PostSeed& c) { return std::abs(c.Dau2_Eta()); }, .max = },
});

// post-fit
inline constexpr auto KaonZeroShort_PostFit_Default = std::to_array<Cuts::Cut<Cached::V0>>({
    // -- kinematics
    // {.name = "Fit_AbsMax_Pz", .extract = [](const Cached::V0& c) { return std::abs(c.Fit_Pz()); }, .max = },
    // {.name = "Fit_Max_Pt", .extract = [](const Cached::V0& c) { return c.Fit_Pt(); }, .max = },
    {.name = "Fit_AbsMax_Rapidity", .extract = [](const Cached::V0& c) { return std::abs(c.Fit_Rapidity()); }, .max = 1.},
    // {.name = "Fit_AbsMax_ArmAlpha", .extract = [](const Cached::V0& c) { return std::abs(c.Fit_ArmAlpha()); }, .max = },
    // {.name = "Fit_Max_ArmQt", .extract = [](const Cached::V0& c) { return c.Fit_ArmQt(); }, .max = },
    // {.name = "Fit_Veto_Mass_AsLambda", .extract = [](const Cached::V0& c) { return c.Fit_Mass_AsLambda(); }, .min = , .max = , .veto = true},
    // {.name = "Fit_Veto_Mass_AsAntiLambda", .extract = [](const Cached::V0& c) { return c.Fit_Mass_AsAntiLambda(); }, .min = , .max = , .veto = true},
    // {.name = "Fit_Veto_Mass_AsPhoton", .extract = [](const Cached::V0& c) { return c.Fit_Mass_AsPhoton(); }, .max = , .veto = true},
    // {.name = "Fit_Veto_ArmRadius_AsLambda", .extract = [](const Cached::V0& c) { return c.Fit_ArmRadius_AsLambda(); }, .min = , .max = , .veto = true},
    // {.name = "Fit_Veto_ArmRadius_AsAntiLambda", .extract = [](const Cached::V0& c) { return c.Fit_ArmRadius_AsAntiLambda(); }, .min = , .max = , .veto = true},
    // -- geometry
    {.name = "Min_Decay_Radius2D", .extract = [](const Cached::V0& c) { return c.Decay_Radius2D(); }, .min = 20.},  // (cm)
    // -- kinematics + geometry
    {.name = "Min_DCA_WithRespectToPV", .extract = [](const Cached::V0& c) { return c.DCA_WithRespectToPV(); }, .min = 10.},  // (cm)
    // -- fit bookkeeping
    {.name = "Max_Chi2Vtx", .extract = [](const Cached::V0& c) { return c.Chi2Vtx(); }, .max = 25.},
    // -- mass-related
    // {.name = "Fit_UnpinnedMass", .extract = [](const Cached::V0& c) { return c.Fit_UnpinnedMass(); }, .min = , .max = },
    {.name = "AbsMax_MassPull", .extract = [](const Cached::V0& c) { return std::abs(static_cast<double>(c.Fit_MassPull)); }, .max = 10.},
    // {.name = "Max_MassErr", .extract = [](const Cached::V0& c) { return static_cast<double>(c.Fit_MassErr); }, .max = },
});

// ## channel A : antisexaquark + neutron -> antilambda + K0S ## //

// post-seed/pre-fit
inline constexpr auto ChannelA_PreFit_Default = std::to_array<Cuts::Cut<PostSeed>>({
    // -- geometry
    {.name = "Seed_Max_DCA_BetweenDaughters", .extract = [](const PostSeed& c) { return c.DCA_Between(); }, .max = 2.},  // (cm)
    {.name = "Seed_Min_Radius2D", .extract = [](const PostSeed& c) { return c.Radius2D(); }, .min = 20.},              // (cm)
    {.name = "Seed_Max_Radius2D", .extract = [](const PostSeed& c) { return c.Radius2D(); }, .max = 90.},              // (cm)
});

// post-fit
inline constexpr auto ChannelA_PostFit_Default = std::to_array<Cuts::Cut<Cached::ChannelA>>({
    // -- kinematics (of the products' sum, Fermi-smeared)
    // {.name = "Fit_AbsMax_Pz", .extract = [](const Cached::ChannelA& c) { return std::abs(c.Fit_Pz()); }, .max = },
    // {.name = "Fit_Pt", .extract = [](const Cached::ChannelA& c) { return c.Fit_Pt(); }, .min = , .max = },
    // {.name = "Fit_Mass", .extract = [](const Cached::ChannelA& c) { return c.Fit_Mass(); }, .min = , .max = },
    // {.name = "Fit_AbsMax_Rapidity", .extract = [](const Cached::ChannelA& c) { return std::abs(c.Fit_Rapidity()); }, .max = },
    // {.name = "Fit_CosTheta_BetweenDaughters", .extract = [](const Cached::ChannelA& c) { return c.Fit_CosTheta_BetweenDaughters(); }, .min = , .max = },
    // -- kinematics, after subtracting the struck nucleon
    // {.name = "NAA_Mass", .extract = [](const Cached::ChannelA& c) { return c.NucleonAlongAxis_Mass(); }, .min = , .max = },
    // -- required Fermi momentum, per projectile-mass hypothesis
    // {.name = "Max_RNM_1800MeV", .extract = [](const Cached::ChannelA& c) { return c.RequiredNucleonMomentum_1800MeV(); }, .max = },
    // -- geometry
    // {.name = "SV_Radius2D", .extract = [](const Cached::ChannelA& c) { return c.SV_Radius2D(); }, .min = , .max = },
    // -- kinematics + geometry, w.r.t. the primary vertex
    // {.name = "Max_Qt_WithRespectToPV", .extract = [](const Cached::ChannelA& c) { return c.Qt_WithRespectToPV(); }, .max = },
    // {.name = "Min_FlightLength_WithRespectToPV", .extract = [](const Cached::ChannelA& c) { return c.FlightLength_WithRespectToPV(); }, .min = },
    // -- fit bookkeeping
    {.name = "Max_Chi2Vtx", .extract = [](const Cached::ChannelA& c) { return c.Chi2Vtx(); }, .max = 25.},
    // (anti)lambda's fit vs antisexaquark's fit
    // {.name = "Min_V0A_FlightLength_WithRespectToSV", .extract = [](const Cached::ChannelA& c) { return c.V0A_FlightLength_WithRespectToSV(); }, .min = 5.},
    // K0S's fit vs antisexaquark's fit
    // {.name = "Min_V0B_FlightLength_WithRespectToSV", .extract = [](const Cached::ChannelA& c) { return c.V0B_FlightLength_WithRespectToSV(); }, .min = 5.},
    // (anti)lambda's fit vs K0S's fit ~ duplicate test
    {.name = "Min_Distance_BetweenV0sDVs", .extract = [](const Cached::ChannelA& c) { return c.Distance_BetweenV0sDVs(); }, .min = 0.01},  // (cm)
});

// ## channel D : antisexaquark + proton -> antilambda + K+ ## //

// post-seed/pre-fit
inline constexpr auto ChannelD_PreFit_Default = std::to_array<Cuts::Cut<PostSeed>>({
    // -- geometry
    {.name = "Seed_Max_DCA_BetweenDaughters", .extract = [](const PostSeed& c) { return c.DCA_Between(); }, .max = 10.},  // (cm)
    {.name = "Seed_Min_Radius2D", .extract = [](const PostSeed& c) { return c.Radius2D(); }, .min = 20.},               // (cm)
    {.name = "Seed_Max_Radius2D", .extract = [](const PostSeed& c) { return c.Radius2D(); }, .max = 180.},              // (cm)
});

// post-fit
inline constexpr auto ChannelD_PostFit_Default = std::to_array<Cuts::Cut<Cached::ChannelD>>({
    // -- fit bookkeeping
    {.name = "Max_Chi2Vtx", .extract = [](const Cached::ChannelD& c) { return c.Chi2Vtx(); }, .max = 100.},
    // (anti)lambda's fit vs antisexaquark's fit
    {.name = "Min_V0_FlightLength_WithRespectToSV", .extract = [](const Cached::ChannelD& c) { return c.V0_FlightLength_WithRespectToSV(); }, .min = 5.},  // (cm)
    // charged kaon's track vs antisexaquark's fit
    // {.name = "Max_Kaon_DCA_WithRespectToSV", .extract = [](const Cached::ChannelD& c) { return c.Kaon_DCA_WithRespectToSV(); }, .max = },
});

// ## channel H : antisexaquark + proton -> K+ K+ X ## //

// post-seed/pre-fit
inline constexpr auto ChannelH_PreFit_Default = std::to_array<Cuts::Cut<PostSeed>>({
    // -- geometry
    {.name = "Seed_Max_DCA_BetweenDaughters", .extract = [](const PostSeed& c) { return c.DCA_Between(); }, .max = 10.},  // (cm)
    {.name = "Seed_Min_Radius2D", .extract = [](const PostSeed& c) { return c.Radius2D(); }, .min = 20.},               // (cm)
    {.name = "Seed_Max_Radius2D", .extract = [](const PostSeed& c) { return c.Radius2D(); }, .max = 180.},              // (cm)
});

// post-fit
inline constexpr auto ChannelH_PostFit_Default = std::to_array<Cuts::Cut<Cached::ChannelH>>({
    // -- fit bookkeeping
    {.name = "Max_Chi2Vtx", .extract = [](const Cached::ChannelH& c) { return c.Chi2Vtx(); }, .max = 100.},
});

// ## the sets ## //

struct Set {
    // pre-seed, structural (hand-written tests)
    // -- tracking can split one physical particle into two ESD entries; these tell them apart
    double Max_TracksDeltaR;     // PENDING: to tune
    double Max_TracksRelDeltaP;  // PENDING: to tune
    // tracks
    Cuts::List<Cached::Track> Proton;
    Cuts::List<Cached::Track> Kaon;
    Cuts::List<Cached::Track> Pion;
    // v0s
    Cuts::List<PostSeed> Lambda_PreFit;
    Cuts::List<Cached::V0> Lambda_PostFit;
    Cuts::List<PostSeed> KaonZeroShort_PreFit;
    Cuts::List<Cached::V0> KaonZeroShort_PostFit;
    // channels
    Cuts::List<PostSeed> ChannelA_PreFit;
    Cuts::List<Cached::ChannelA> ChannelA_PostFit;
    Cuts::List<PostSeed> ChannelD_PreFit;
    Cuts::List<Cached::ChannelD> ChannelD_PostFit;
    Cuts::List<PostSeed> ChannelH_PreFit;
    Cuts::List<Cached::ChannelH> ChannelH_PostFit;
};

inline constexpr Set Default{
    .Max_TracksDeltaR = 0.02,
    .Max_TracksRelDeltaP = 0.05,
    .Proton = Proton_Default,
    .Kaon = Kaon_Default,
    .Pion = Pion_Default,
    .Lambda_PreFit = Lambda_PreFit_Default,
    .Lambda_PostFit = Lambda_PostFit_Default,
    .KaonZeroShort_PreFit = KaonZeroShort_PreFit_Default,
    .KaonZeroShort_PostFit = KaonZeroShort_PostFit_Default,
    .ChannelA_PreFit = ChannelA_PreFit_Default,
    .ChannelA_PostFit = ChannelA_PostFit_Default,
    .ChannelD_PreFit = ChannelD_PreFit_Default,
    .ChannelD_PostFit = ChannelD_PostFit_Default,
    .ChannelH_PreFit = ChannelH_PreFit_Default,
    .ChannelH_PostFit = ChannelH_PostFit_Default,
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
    throw std::runtime_error(std::format("unknown finder cut set \"{}\"", name));
}

}  // namespace T2DS::CutSets::Finder
