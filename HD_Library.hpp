#pragma once

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
    DB::Particles::Particle("PiPlus"),
    DB::Particles::Particle("AntiProton"),
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

constexpr InfoCorrelation GetAngles(const ROOT::Math::PxPyPzEVector& lv_h_lab, const ROOT::Math::PxPyPzEVector& lv_l1_lab,
                                    const ROOT::Math::PxPyPzEVector& lv_l2_lab, const ROOT::Math::PxPyPzEVector& lv_p1_lab,
                                    const ROOT::Math::PxPyPzEVector& lv_p2_lab) {
    // 1. boost protons to their respective lambda rest frames //
    auto boost_to_l1_rf = ROOT::Math::Boost(lv_l1_lab.BoostToCM());
    auto p1_in_l1_rf = boost_to_l1_rf(lv_p1_lab);
    auto boost_to_l2_rf = ROOT::Math::Boost(lv_l2_lab.BoostToCM());
    auto p2_in_l2_rf = boost_to_l2_rf(lv_p2_lab);
    // -- cache unit vectors
    auto u_h = lv_h_lab.Vect().Unit();
    auto u_l1 = lv_l1_lab.Vect().Unit();
    auto u_l2 = lv_l2_lab.Vect().Unit();
    auto u_p1 = p1_in_l1_rf.Vect().Unit();
    auto u_p2 = p2_in_l2_rf.Vect().Unit();
    // -- angle between protons
    double cos_theta_pp = u_p1.Dot(u_p2);
    double theta_pp = std::acos(cos_theta_pp);
    // -- angle between lambdas and h-dibaryon
    double cos_theta_star_l1 = u_l1.Dot(u_h);
    double cos_theta_star_l2 = u_l2.Dot(u_h);
    // -- angle between protons and their respective lambdas
    double cos_theta_star_p1 = u_p1.Dot(u_l1);
    double cos_theta_star_p2 = u_p2.Dot(u_l2);
    // -- relative momentum
    auto boost_to_h_rf = ROOT::Math::Boost(lv_h_lab.BoostToCM());
    auto l1_in_h_rf = boost_to_h_rf(lv_l1_lab);
    double q_rel = std::abs(l1_in_h_rf.Vect().R());  // = |l2_in_h_rf|
    return {cos_theta_pp, theta_pp, cos_theta_star_l1, cos_theta_star_l2, cos_theta_star_p1, cos_theta_star_p2, q_rel};
}

}  // namespace HD
