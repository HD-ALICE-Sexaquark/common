#pragma once

#include <array>

#include "Constants.hpp"

namespace POD {

struct PreFoundLambda {
    // (anti)lambda info //
    unsigned int PreFoundEntry;
    float Decay_X;
    float Decay_Y;
    float Decay_Z;
    float DcaV0Daughters;

    // negative daughter //
    unsigned int Neg_EsdEntry;
    std::array<float, 6> Neg_State;                                        // from parametrization from AliESDv0
    std::array<float, Common::NCovMatrixComponents_State6> Neg_CovMatrix;  // from parametrization from AliESDv0
    float Neg_PreDCAxy;
    float Neg_PreDCAz;
    float Neg_NSigmasProton;
    float Neg_NSigmasKaon;
    float Neg_NSigmasPion;
    float Neg_TPC_NCrossedRows;
    unsigned short Neg_TPC_NClusters;
    unsigned short Neg_TPC_NClustersFindable;
    float Neg_TPC_Chi2;
    // -- related to (anti)lambda
    float Neg_PCAwrtV0_Px;
    float Neg_PCAwrtV0_Py;
    float Neg_PCAwrtV0_Pz;

    // positive daughter //
    unsigned int Pos_EsdEntry;
    std::array<float, 6> Pos_State;                                        // from parametrization from AliESDv0
    std::array<float, Common::NCovMatrixComponents_State6> Pos_CovMatrix;  // from parametrization from AliESDv0
    float Pos_PreDCAxy;
    float Pos_PreDCAz;
    float Pos_NSigmasProton;
    float Pos_NSigmasKaon;
    float Pos_NSigmasPion;
    float Pos_TPC_NCrossedRows;
    unsigned short Pos_TPC_NClusters;
    unsigned short Pos_TPC_NClustersFindable;
    float Pos_TPC_Chi2;
    // -- related to (anti)lambda
    float Pos_PCAwrtV0_Px;
    float Pos_PCAwrtV0_Py;
    float Pos_PCAwrtV0_Pz;
};

}  // namespace POD
