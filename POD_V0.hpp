#pragma once

#include <array>

#include "Constants.hpp"

namespace POD {

struct V0 {
    // -- candidate info
    float Decay_X;
    float Decay_Y;
    float Decay_Z;
    float Px;
    float Py;
    float Pz;
    float Energy;
    std::array<float, Common::NCovMatrixComponents_State7> CovMatrix;
    float Chi2NDF;
    // -- negative daughter
    float Neg_PCAwrtV0_X;
    float Neg_PCAwrtV0_Y;
    float Neg_PCAwrtV0_Z;
    float Neg_PCAwrtV0_Px;
    float Neg_PCAwrtV0_Py;
    float Neg_PCAwrtV0_Pz;
    // -- positive daughter
    float Pos_PCAwrtV0_X;
    float Pos_PCAwrtV0_Y;
    float Pos_PCAwrtV0_Z;
    float Pos_PCAwrtV0_Px;
    float Pos_PCAwrtV0_Py;
    float Pos_PCAwrtV0_Pz;
};

}  // namespace POD
