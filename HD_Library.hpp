#pragma once

#include <cmath>

#include <Math/GenVector/Boost.h>
#include <Math/Vector3D.h>
#include <Math/Vector4Dfwd.h>

#include "Constants.hpp"
#include "DB_Particles.hpp"

// H-Dibaryon library.
namespace HD {

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

constexpr const DecayTree& GetDecayTree(bool anti_channel) noexcept { return anti_channel ? AntiChannel : Channel; }

// Cache Calculations //

struct InfoCorrelation {
    double cos_theta_pp{Common::DummyFloat};
    double theta_pp{Common::DummyFloat};
    double cos_theta_star_l1{Common::DummyFloat};
    double cos_theta_star_l2{Common::DummyFloat};
    double cos_theta_star_p1{Common::DummyFloat};
    double cos_theta_star_p2{Common::DummyFloat};
    double q_rel{Common::DummyFloat};
};

inline InfoCorrelation GetAngles(const ROOT::Math::PxPyPzEVector& h_lab, const ROOT::Math::PxPyPzEVector& l1_lab,
                                 const ROOT::Math::PxPyPzEVector& l2_lab, const ROOT::Math::PxPyPzEVector& p1_lab,
                                 const ROOT::Math::PxPyPzEVector& p2_lab) {
    // 1. boost everything to H rest frame
    auto boost_to_h = ROOT::Math::Boost(h_lab.BoostToCM());
    ROOT::Math::PxPyPzEVector l1_in_h = boost_to_h(l1_lab);
    ROOT::Math::PxPyPzEVector l2_in_h = boost_to_h(l2_lab);
    ROOT::Math::PxPyPzEVector p1_in_h = boost_to_h(p1_lab);
    ROOT::Math::PxPyPzEVector p2_in_h = boost_to_h(p2_lab);
    // 2. boost protons to their respective lambda rest frames
    auto boost_to_l1 = ROOT::Math::Boost(l1_in_h.BoostToCM());
    auto boost_to_l2 = ROOT::Math::Boost(l2_in_h.BoostToCM());
    ROOT::Math::PxPyPzEVector p1_in_l1 = boost_to_l1(p1_in_h);
    ROOT::Math::PxPyPzEVector p2_in_l2 = boost_to_l2(p2_in_h);
    // -- cache sqrt calls
    auto u_h = h_lab.Vect().Unit();
    auto u_l1 = l1_in_h.Vect().Unit();
    auto u_l2 = l2_in_h.Vect().Unit();
    // -- angle between protons
    double cos_theta_pp = p1_in_l1.Vect().Unit().Dot(p2_in_l2.Vect().Unit());
    double theta_pp = std::acos(cos_theta_pp);  // not used here
    // -- angle between lambdas and h-dibaryon
    double cos_theta_star_l1 = u_l1.Dot(u_h);
    double cos_theta_star_l2 = u_l2.Dot(u_h);
    // -- angle between protons and their respective lambdas
    double cos_theta_star_p1 = p1_in_l1.Vect().Unit().Dot(u_l1);
    double cos_theta_star_p2 = p2_in_l2.Vect().Unit().Dot(u_l2);
    // -- relative momentum
    double q_rel = l1_in_h.Vect().R();  // = |l2_in_h| // not used here
    return {cos_theta_pp, theta_pp, cos_theta_star_l1, cos_theta_star_l2, cos_theta_star_p1, cos_theta_star_p2, q_rel};
}

}  // namespace HD
