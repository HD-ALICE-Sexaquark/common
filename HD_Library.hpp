#pragma once

#include <cmath>

#include <Math/Boost.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>
#include <Math/VectorUtil.h>

#include "Constants.hpp"
#include "DB_Particles.hpp"
#include "Math.hpp"
#include "POD_PreFoundLambda.hpp"

// H-Dibaryon library.
namespace HD {

namespace RMath = ROOT::Math;
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

constexpr bool IsAntiChannel(const POD::Extended::PreFoundLambda& l1, const POD::Extended::PreFoundLambda& l2) {
    return l1.IsAntiLambda && l2.IsAntiLambda;
}
constexpr bool IsMixedChannel(const POD::Extended::PreFoundLambda& l1, const POD::Extended::PreFoundLambda& l2) {
    return l1.IsAntiLambda != l2.IsAntiLambda;
}

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

// Cache Calculations //

struct InfoCorrelation {
    double cos_theta_pp{Common::DummyDouble};
    double theta_pp{Common::DummyDouble};
    double cos_theta_star_l1{Common::DummyDouble};
    double cos_theta_star_l2{Common::DummyDouble};
    double cos_theta_star_p1{Common::DummyDouble};
    double cos_theta_star_p2{Common::DummyDouble};
    double q_rel{Common::DummyDouble};
};

inline InfoCorrelation GetAngles(const RMath::PxPyPzEVector& h_lab, const RMath::PxPyPzEVector& l1_lab, const RMath::PxPyPzEVector& l2_lab,
                                 const RMath::PxPyPzEVector& p1_lab, const RMath::PxPyPzEVector& p2_lab) {
    // 1. boost everything to H rest frame
    auto boost_to_h = RMath::Boost(h_lab.BoostToCM());
    RMath::PxPyPzEVector l1_in_h = boost_to_h(l1_lab);
    RMath::PxPyPzEVector l2_in_h = boost_to_h(l2_lab);
    RMath::PxPyPzEVector p1_in_h = boost_to_h(p1_lab);
    RMath::PxPyPzEVector p2_in_h = boost_to_h(p2_lab);
    // 2. boost protons to their respective lambda rest frames
    auto boost_to_l1 = RMath::Boost(l1_in_h.BoostToCM());
    auto boost_to_l2 = RMath::Boost(l2_in_h.BoostToCM());
    RMath::PxPyPzEVector p1_in_l1 = boost_to_l1(p1_in_h);
    RMath::PxPyPzEVector p2_in_l2 = boost_to_l2(p2_in_h);
    // -- cache sqrt calls
    auto u_h = h_lab.Vect().Unit();
    auto u_l1 = l1_in_h.Vect().Unit();
    auto u_l2 = l2_in_h.Vect().Unit();
    // -- angle between protons
    double cos_theta_pp = p1_in_l1.Vect().Unit().Dot(p2_in_l2.Vect().Unit());
    double theta_pp = std::acos(cos_theta_pp);
    // -- angle between lambdas and h-dibaryon
    double cos_theta_star_l1 = u_l1.Dot(u_h);
    double cos_theta_star_l2 = u_l2.Dot(u_h);
    // -- angle between protons and their respective lambdas
    double cos_theta_star_p1 = p1_in_l1.Vect().Unit().Dot(u_l1);
    double cos_theta_star_p2 = p2_in_l2.Vect().Unit().Dot(u_l2);
    // -- relative momentum
    double q_rel = l1_in_h.Vect().R();  // = |l2_in_h|
    return {cos_theta_pp, theta_pp, cos_theta_star_l1, cos_theta_star_l2, cos_theta_star_p1, cos_theta_star_p2, q_rel};
}

}  // namespace HD
