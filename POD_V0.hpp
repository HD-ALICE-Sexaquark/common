#pragma once

#include "Constants.hpp"
#include "POD_McParticle.hpp"
#include "POD_Track.hpp"

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
    POD::Track Neg;
    float Neg_PCAwrtV0_X;
    float Neg_PCAwrtV0_Y;
    float Neg_PCAwrtV0_Z;
    float Neg_PCAwrtV0_Px;
    float Neg_PCAwrtV0_Py;
    float Neg_PCAwrtV0_Pz;
    // -- positive daughter
    POD::Track Pos;
    float Pos_PCAwrtV0_X;
    float Pos_PCAwrtV0_Y;
    float Pos_PCAwrtV0_Z;
    float Pos_PCAwrtV0_Px;
    float Pos_PCAwrtV0_Py;
    float Pos_PCAwrtV0_Pz;
    // -- mc info
    std::optional<POD::McParticle> MC;
};

}  // namespace POD
