#pragma once

#include <cmath>
#include <numbers>
#include <random>

#include <Math/Vector3D.h>

// The struck nucleon's Fermi motion, rewritten from GEANT4's `G4FermiMomentum` (the "classic" local Fermi gas): the
// nucleon sits at a point drawn from the nuclear density (Woods-Saxon for A > 16, harmonic-oscillator shell model
// otherwise), and its momentum is drawn uniformly inside the local Fermi sphere `p_max = hbar c (3 pi^2 rho)^(1/3)`.
// NOTE: `A = 1` (hydrogen) is not special-cased.
namespace FastSim::FermiMotion {

inline constexpr double hbarc = 0.197327;  // (GeV fm)
inline const double ConstOfPMax = hbarc * std::cbrt(3. * std::numbers::pi * std::numbers::pi);

// The nuclear density (fm^-3, per nucleon) at a point drawn from the density itself, by rejection.
template <std::uniform_random_bit_generator G>
[[nodiscard]] double Classic_SampleLocalDensity(G& rng, int A) {

    constexpr double pi = std::numbers::pi;
    std::uniform_real_distribution<double> symmetric{-1., 1.};
    std::uniform_real_distribution<double> unit{0., 1.};
    const auto a = static_cast<double>(A);

    if (A > 16) {
        // Woods-Saxon
        const double a13 = std::cbrt(a);
        const double r0 = 1.16 * (1.0 - 1.16 / (a13 * a13));
        const double radius = r0 * a13;
        const double a_surf = 0.545;
        const double rho0 = 3.0 / (4.0 * pi * r0 * r0 * r0 * a * (1.0 + (a_surf / radius) * (a_surf / radius) * pi * pi));
        const double max_r = radius + a_surf * std::log((1.0 - 0.001) / 0.001);  // where the relative density drops to 0.001
        while (true) {
            const double x = symmetric(rng) * max_r;
            const double y = symmetric(rng) * max_r;
            const double z = symmetric(rng) * max_r;
            const double r = std::sqrt(x * x + y * y + z * z);
            const double rel_density = 1.0 / (1.0 + std::exp((r - radius) / a_surf));
            if (unit(rng) < rel_density) return rho0 * rel_density;
        }
    }

    // shell model (Gaussian)
    const double rsq = 0.8133 * std::cbrt(a * a);
    const double inv_pi_rsq = 1.0 / (pi * rsq);
    const double rho0 = inv_pi_rsq * std::sqrt(inv_pi_rsq);
    const double max_r = std::sqrt(rsq * std::log(1.0 / 0.001));
    while (true) {
        const double x = symmetric(rng) * max_r;
        const double y = symmetric(rng) * max_r;
        const double z = symmetric(rng) * max_r;
        const double r2 = x * x + y * y + z * z;
        if (r2 > max_r * max_r) continue;
        const double rel_density = std::exp(-r2 / rsq);
        if (unit(rng) < rel_density) return rho0 * rel_density;
    }
}

// The struck nucleon's momentum (GeV/c): uniform inside the Fermi sphere of the local density.
template <std::uniform_random_bit_generator G>
[[nodiscard]] ROOT::Math::XYZVector Classic_SampleMomentum(G& rng, int A) {

    const double rho = Classic_SampleLocalDensity(rng, A) * static_cast<double>(A);
    const double max_momentum = ConstOfPMax * std::cbrt(rho);

    std::uniform_real_distribution<double> symmetric{-1., 1.};
    double px = 0.;
    double py = 0.;
    double pz = 0.;
    do {
        px = symmetric(rng);
        py = symmetric(rng);
        pz = symmetric(rng);
    } while (px * px + py * py + pz * pz > 1.);

    return {px * max_momentum, py * max_momentum, pz * max_momentum};
}

}  // namespace FastSim::FermiMotion
