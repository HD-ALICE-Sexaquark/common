#pragma once

#include <cmath>
#include <format>
#include <numbers>
#include <random>
#include <stdexcept>
#include <utility>

#include <Math/GenVector/Boost.h>
#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

// The kinematics of a fast simulation for two-body final states only.
// What `TGenPhaseSpace` and `TLorentzVector` do, on `GenVector` alone.
namespace FastSim::Kinematics {

namespace RMath = ROOT::Math;

// A particle of mass `mass` with transverse momentum `pt`, rapidity `y` and azimuth `phi`.
inline RMath::PxPyPzEVector FromPtRapidityPhi(double mass, double pt, double y, double phi) {
    const double mt = std::sqrt(mass * mass + pt * pt);
    return {pt * std::cos(phi), pt * std::sin(phi), mt * std::sinh(y), mt * std::cosh(y)};
}

// A unit vector, isotropic.
template <std::uniform_random_bit_generator G>
[[nodiscard]] RMath::XYZVector IsotropicDirection(G& rng) {
    const double cos_theta = std::uniform_real_distribution<double>{-1., 1.}(rng);
    const double sin_theta = std::sqrt(1. - cos_theta * cos_theta);
    const double phi = std::uniform_real_distribution<double>{-std::numbers::pi, std::numbers::pi}(rng);
    return {sin_theta * std::cos(phi), sin_theta * std::sin(phi), cos_theta};
}

// `mother -> 1 + 2`: isotropic in the mother's rest frame, then boosted to the lab.
// Throws below threshold.
template <std::uniform_random_bit_generator G>
[[nodiscard]] std::pair<RMath::PxPyPzEVector, RMath::PxPyPzEVector> TwoBodyDecay(G& rng, const RMath::PxPyPzEVector& mother, double m1, double m2) {

    const double m = mother.M();
    const double sum = m1 + m2;
    const double diff = m1 - m2;
    if (m <= sum) {
        throw std::runtime_error(std::format("two-body decay below threshold: M = {:.4f} <= m1 + m2 = {:.4f}", m, sum));
    }
    const double p_star = std::sqrt((m * m - sum * sum) * (m * m - diff * diff)) / (2. * m);
    const RMath::XYZVector p1 = p_star * IsotropicDirection(rng);

    // `BoostToCM` is the lab -> rest-frame boost; its inverse takes the rest-frame products back to the lab
    RMath::Boost to_lab{mother.BoostToCM()};
    to_lab.Invert();

    return {to_lab(RMath::PxPyPzEVector{p1.X(), p1.Y(), p1.Z(), std::sqrt(p_star * p_star + m1 * m1)}),
            to_lab(RMath::PxPyPzEVector{-p1.X(), -p1.Y(), -p1.Z(), std::sqrt(p_star * p_star + m2 * m2)})};
}

// Where a particle of lifetime `ctau` (cm), produced at `origin` with 4-momentum `lv`, decays: an exponential decay
// length `L = (p / m) ctau (-ln u)` along its direction.
template <std::uniform_random_bit_generator G>
[[nodiscard]] RMath::XYZPoint DecayVertex(G& rng, const RMath::XYZPoint& origin, const RMath::PxPyPzEVector& lv, double ctau) {
    const double decay_length = (lv.P() / lv.M()) * ctau * std::exponential_distribution<double>{1.}(rng);
    return origin + decay_length * lv.Vect().Unit();
}

}  // namespace FastSim::Kinematics
