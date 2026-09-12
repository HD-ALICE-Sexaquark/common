#pragma once

#include <array>
#include <cstdint>

#include "Constants.hpp"

namespace POD {

// NOTES:
// - not added:
//   - `Fit_P* = Fit_Dau1_* + Fit_Dau2_*` (closure)
//   - `Dau{1,2}_Fit_{X,Y,Z}`, because it's the same as candidate's `Fit_{X,Y,Z}`
//   - `MassErr` nor `MassPull`, because of unknown mass and Fermi motion
struct Sexaquark {
    // post-fit
    // -- antisexaquark
    std::array<float, Common::NCovMatrixComponents_State3> SV_CovMatrix;
    float Fit_X;  // secondary vertex
    float Fit_Y;
    float Fit_Z;
    // -- daughter 1
    float Fit_Dau1_Px;
    float Fit_Dau1_Py;
    float Fit_Dau1_Pz;
    float Fit_Dau1_Energy;
    // -- daughter 2
    float Fit_Dau2_Px;
    float Fit_Dau2_Py;
    float Fit_Dau2_Pz;
    float Fit_Dau2_Energy;
    // post-seed/pre-fit
    // -- daughter 1: (anti)lambda [A,D] | (pos/neg) kaon [H]
    float Seed_Dau1_X;  // the PCA is the seeder's
    float Seed_Dau1_Y;
    float Seed_Dau1_Z;
    float Seed_Dau1_Px;
    float Seed_Dau1_Py;
    float Seed_Dau1_Pz;
    float Seed_Dau1_Energy;
    // -- daughter 2: kaon-zero-short [A] | (pos/neg) kaon [D,H]
    float Seed_Dau2_X;
    float Seed_Dau2_Y;
    float Seed_Dau2_Z;
    float Seed_Dau2_Px;
    float Seed_Dau2_Py;
    float Seed_Dau2_Pz;
    float Seed_Dau2_Energy;
    // extra
    float Chi2;
    std::uint8_t FitFlags;  // `Common::FitFlag` mask
    bool IsWrongSignChannel;
};

}  // namespace POD
