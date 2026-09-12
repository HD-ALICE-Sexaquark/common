#pragma once

#include <array>
#include <cstdint>

#include "Constants.hpp"

namespace POD {

// NOTES:
// - not stored:
//   - `Fit_P*` = `Fit_Neg_* + Fit_Pos_*` (closure)
// - decay length + err are not stored, because the origin vertex is unknown at this stage
struct V0 {
    // post-fit
    // -- v0
    std::array<float, Common::NCovMatrixComponents_State7> CovMatrix;
    float Fit_X;
    float Fit_Y;
    float Fit_Z;
    // -- negative daughter
    float Fit_Neg_Px;
    float Fit_Neg_Py;
    float Fit_Neg_Pz;
    float Fit_Neg_Energy;
    // -- positive daughter
    float Fit_Pos_Px;
    float Fit_Pos_Py;
    float Fit_Pos_Pz;
    float Fit_Pos_Energy;
    // post-seed/pre-fit
    // -- negative daughter
    float Seed_Neg_X;
    float Seed_Neg_Y;
    float Seed_Neg_Z;
    float Seed_Neg_Px;
    float Seed_Neg_Py;
    float Seed_Neg_Pz;
    float Seed_Neg_Energy;
    // post-seed/pre-fit
    // -- positive daughter
    float Seed_Pos_X;
    float Seed_Pos_Y;
    float Seed_Pos_Z;
    float Seed_Pos_Px;
    float Seed_Pos_Py;
    float Seed_Pos_Pz;
    float Seed_Pos_Energy;
    // extra
    float Fit_MassErr;
    float Fit_MassPull;
    float Chi2;             // vertex chi2, plus `Fit_MassPull^2` when the mother was pinned
    std::uint8_t FitFlags;  // `Common::FitFlag` mask
};

}  // namespace POD
