#pragma once

#include <array>

#include "Constants.hpp"

namespace POD {

struct V0 {
    // candidate info
    float Decay_X;
    float Decay_Y;
    float Decay_Z;
    float Px;
    float Py;
    float Pz;
    float Energy;
    std::array<float, Common::NCovMatrixComponents_State7> CovMatrix;
    float Chi2NDF;
    // negative daughter
    float Neg_PCAwrtV0_X;
    float Neg_PCAwrtV0_Y;
    float Neg_PCAwrtV0_Z;
    float Neg_Fit_Px;
    float Neg_Fit_Py;
    float Neg_Fit_Pz;
    float Neg_Fit_Energy;
    // positive daughter
    float Pos_PCAwrtV0_X;
    float Pos_PCAwrtV0_Y;
    float Pos_PCAwrtV0_Z;
    float Pos_Fit_Px;
    float Pos_Fit_Py;
    float Pos_Fit_Pz;
    float Pos_Fit_Energy;
};

}  // namespace POD
