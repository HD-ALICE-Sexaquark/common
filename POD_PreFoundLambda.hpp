#pragma once

#include <array>

#include "Constants.hpp"

namespace POD {

struct PreFoundLambda {
    // -- (anti)lambda info
    unsigned int PreFoundEntry;
    float Decay_X;
    float Decay_Y;
    float Decay_Z;
    float DcaV0Daughters;
    float Chi2;
    // -- negative daughter
    unsigned int Neg_EsdEntry;
    std::array<float, 6> Neg_State;
    std::array<float, Common::NCovMatrixComponents_State6> Neg_CovMatrix;
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
    std::array<float, 6> Pos_State;
    std::array<float, Common::NCovMatrixComponents_State6> Pos_CovMatrix;
    float Pos_PCAwrtV0_Px;
    float Pos_PCAwrtV0_Py;
    float Pos_PCAwrtV0_Pz;
    float Pos_PreDCAxy;
    float Pos_PreDCAz;
    float Pos_NSigmaProton;
    float Pos_NSigmaKaon;
    float Pos_NSigmaPion;
};

namespace Extended {

struct PreFoundLambda : POD::PreFoundLambda {
    // kinematics
    float Px;
    float Py;
    float Pz;
    float Energy;
    std::array<float, Common::NCovMatrixComponents_State7> CovMatrix;
    // negative daughter
    float Neg_Energy;
    // positive daughter
    float Pos_Energy;
};

}  // namespace Extended

}  // namespace POD
