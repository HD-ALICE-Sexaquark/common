#pragma once

#include <optional>

#include "POD_McParticle.hpp"

namespace POD {

struct OnTheFlyLambda {
    // -- (anti)lambda info
    unsigned int OnTheFlyEntry;
    float Decay_X;
    float Decay_Y;
    float Decay_Z;
    float Px;
    float Py;
    float Pz;
    float DcaV0Daughters;
    // -- negative daughter
    unsigned int Neg_EsdEntry;
    float Neg_PCAwrtV0_Px;
    float Neg_PCAwrtV0_Py;
    float Neg_PCAwrtV0_Pz;
    float Neg_PreDCAxy;
    float Neg_PreDCAz;
    float Neg_NSigmaProton;
    float Neg_NSigmaKaon;
    float Neg_NSigmaPion;
    // -- positive daughter
    unsigned int Pos_EsdEntry;
    float Pos_PCAwrtV0_Px;
    float Pos_PCAwrtV0_Py;
    float Pos_PCAwrtV0_Pz;
    float Pos_PreDCAxy;
    float Pos_PreDCAz;
    float Pos_NSigmaProton;
    float Pos_NSigmaKaon;
    float Pos_NSigmaPion;
    // -- linked mc
    std::optional<unsigned int> Neg_McEntry;  // (mc only)
    std::optional<unsigned int> Pos_McEntry;  // (mc only)
    std::optional<POD::McParticle> MC;
    std::optional<POD::McParticle> Neg_MC;
    std::optional<POD::McParticle> Pos_MC;
};

}  // namespace POD
