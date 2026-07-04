#pragma once

#include <array>

#include "Constants.hpp"

namespace POD {

struct PreFoundLambda {
    // (anti)lambda info
    unsigned int PreFoundEntry;
    float Decay_X;         // (updated after fit)
    float Decay_Y;         // (updated after fit)
    float Decay_Z;         // (updated after fit)
    float DcaV0Daughters;  // (updated after fit)
    // negative daughter
    unsigned int Neg_EsdEntry;
    std::array<float, 6> Neg_State;                                        // from parametrization from AliESDv0
    std::array<float, Common::NCovMatrixComponents_State6> Neg_CovMatrix;  // from parametrization from AliESDv0
    float Neg_PreDCAxy;
    float Neg_PreDCAz;
    float Neg_NSigmasProton;
    float Neg_NSigmasKaon;
    float Neg_NSigmasPion;
    // -- related to (anti)lambda
    float Neg_PCAwrtV0_Px;  // (updated after fit)
    float Neg_PCAwrtV0_Py;  // (updated after fit)
    float Neg_PCAwrtV0_Pz;  // (updated after fit)
    // positive daughter
    unsigned int Pos_EsdEntry;
    std::array<float, 6> Pos_State;                                        // from parametrization from AliESDv0
    std::array<float, Common::NCovMatrixComponents_State6> Pos_CovMatrix;  // from parametrization from AliESDv0
    float Pos_PreDCAxy;
    float Pos_PreDCAz;
    float Pos_NSigmasProton;
    float Pos_NSigmasKaon;
    float Pos_NSigmasPion;
    // -- related to (anti)lambda
    float Pos_PCAwrtV0_Px;  // (updated after fit)
    float Pos_PCAwrtV0_Py;  // (updated after fit)
    float Pos_PCAwrtV0_Pz;  // (updated after fit)
};

namespace Extended {

struct PreFoundLambda : POD::PreFoundLambda {
    float Px;
    float Py;
    float Pz;
    float Energy;
    std::array<float, Common::NCovMatrixComponents_State7> CovMatrix;
    float Chi2NDF;
    // daughters
    float Neg_Energy;
    float Pos_Energy;
};

}  // namespace Extended

}  // namespace POD
