#pragma once

#include "DB_Particles.hpp"
#include "Math.hpp"
#include "POD_PreFoundLambda.hpp"

// H-Dibaryon library: the decay trees and the duplicate detection. The angular correlations of the closed tree are
// `Cached::Unit::Correlations`.
namespace HD {

namespace CMath = Common::Math;

// Decay Tree //

struct DecayTree {
    DB::Particles::Definition hdibaryon;
    DB::Particles::Definition lambda;
    DB::Particles::Definition neg;  // lambda's negative daughter
    DB::Particles::Definition pos;  // lambda's positive daughter
};

inline constexpr DecayTree Channel = {
    DB::Particles::Particle("Hdibaryon"),
    DB::Particles::Particle("Lambda"),
    DB::Particles::Particle("PiMinus"),
    DB::Particles::Particle("Proton"),
};
inline constexpr DecayTree AntiChannel = {
    DB::Particles::Particle("AntiHdibaryon"),
    DB::Particles::Particle("AntiLambda"),
    DB::Particles::Particle("AntiProton"),
    DB::Particles::Particle("PiPlus"),
};

constexpr const DecayTree& GetDecayTree(bool anti_channel) { return anti_channel ? AntiChannel : Channel; }

// Duplicate Detection via entries //

inline bool SameDaughterEntries(const POD::PreFoundLambda& a) {
    // Are this lambda's legs are the same entry?
    return a.Neg_EsdEntry == a.Pos_EsdEntry;
}

inline bool SameDaughterEntries(const POD::PreFoundLambda& a, const POD::PreFoundLambda& b) {
    // Do these two lambdas share a leg?
    return a.Neg_EsdEntry == b.Neg_EsdEntry || a.Neg_EsdEntry == b.Pos_EsdEntry || a.Pos_EsdEntry == b.Neg_EsdEntry ||
           a.Pos_EsdEntry == b.Pos_EsdEntry;
}

inline bool SameLambdasEntries(const POD::PreFoundLambda& a, const POD::PreFoundLambda& b) {
    return a.PreFoundEntry == b.PreFoundEntry || (a.Neg_EsdEntry == b.Neg_EsdEntry && a.Pos_EsdEntry == b.Pos_EsdEntry);
}

// Check whether two lambdas are the same.
// This function must be applied at input-level. Both competing candidates may be genuine.
inline bool SameLambda(const POD::PreFoundLambda& a, const POD::PreFoundLambda& b, double max_tracks_delta_r, double max_tracks_rel_delta_p) {
    if (SameLambdasEntries(a, b)) return true;  // early return; small performance boost
    return (CMath::IsSameHelix(a.Neg_State, b.Neg_State, max_tracks_delta_r, max_tracks_rel_delta_p) &&
            CMath::IsSameHelix(a.Pos_State, b.Pos_State, max_tracks_delta_r, max_tracks_rel_delta_p));
}

}  // namespace HD
