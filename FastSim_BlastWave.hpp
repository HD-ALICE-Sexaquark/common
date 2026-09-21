#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <random>

#include <TMath.h>

#include "DB_Centrality.hpp"

// The blast-wave pt spectrum of the injected antisexaquark, one per centrality class. Parameters taken from
// "Production of charged pions, kaons, and (anti-)protons in Pb-Pb and inelastic pp collisions at sqrt(s_NN) = 5.02 TeV",
// ALICE Collab. PRC 101, 044907 (2020).
namespace FastSim::BlastWave {

inline constexpr std::size_t NClasses = DB::Centrality::NClasses;
static_assert(NClasses == 10, "the blast-wave parameters below are tabulated for the ten ALICE centrality classes");

// kinetic freeze-out temperature (GeV)
inline constexpr std::array<double, NClasses> KineticFOTemp{0.090, 0.091, 0.094, 0.097, 0.101, 0.108, 0.115, 0.129, 0.147, 0.161};

// average transverse velocity
inline constexpr std::array<double, NClasses> AvgBetaTemp{0.663, 0.660, 0.655, 0.643, 0.622, 0.595, 0.557, 0.506, 0.435, 0.355};

// radial flow power
inline constexpr std::array<double, NClasses> RadialFlowPower{0.735, 0.736, 0.739, 0.771, 0.828, 0.908, 1.052, 1.262, 1.678, 2.423};

// `dN/dpt ~ pt mt int_0^1 r I0(pt sinh(rho) / T) K1(mt cosh(rho) / T) dr`, with `rho = atanh(beta_s r^n)` and
// `beta_s = (n + 2) <beta> / 2`. Unnormalised. The integral is a composite Simpson rule over `r`: the integrand is
// smooth on [0, 1] (`beta_s < 1` for every class), and the result only seeds the tabulated sampler below.
inline double Density(double pt, double m0, std::size_t cc, std::size_t n_intervals = 200) {

    const double t_kin = KineticFOTemp[cc];
    const double np = RadialFlowPower[cc];
    const double beta_s = (np + 2.) * AvgBetaTemp[cc] / 2.;
    const double mt = std::sqrt(pt * pt + m0 * m0);

    const auto integrand = [&](double r) {
        const double rho = std::atanh(beta_s * std::pow(r, np));
        return r * TMath::BesselI0(pt * std::sinh(rho) / t_kin) * TMath::BesselK1(mt * std::cosh(rho) / t_kin);
    };

    const std::size_t n = n_intervals + (n_intervals % 2);  // Simpson needs an even count
    const double h = 1. / static_cast<double>(n);
    double sum = integrand(0.) + integrand(1.);
    for (std::size_t i = 1; i < n; ++i) sum += (i % 2 == 1 ? 4. : 2.) * integrand(static_cast<double>(i) * h);

    return pt * mt * sum * h / 3.;
}

// One tabulated sampler per centrality class, built once for a particle of mass `m0`: `Density` is evaluated at
// `n_nodes + 1` equidistant nodes on [0, max_pt] and interpolated linearly in between (replaces `TF1::GetRandom`).
class PtSampler {
   public:
    PtSampler(double m0, double max_pt, std::size_t n_nodes = 400) {
        for (std::size_t cc = 0; cc < NClasses; ++cc) {
            fDistributions[cc] = std::piecewise_linear_distribution<double>(n_nodes, 0., max_pt, [&](double pt) { return Density(pt, m0, cc); });
        }
    }

    template <std::uniform_random_bit_generator G>
    [[nodiscard]] double operator()(G& rng, std::size_t cc) {
        return fDistributions[cc](rng);
    }

   private:
    std::array<std::piecewise_linear_distribution<double>, NClasses> fDistributions;
};

}  // namespace FastSim::BlastWave
