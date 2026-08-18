#pragma once

#include <array>

#include "Constants.hpp"

namespace POD {

struct PreFoundLambda {
    // (anti)lambda info //
    unsigned int PreFoundEntry;
    float Decay_X;         // (updated after new fit)
    float Decay_Y;         // (updated after new fit)
    float Decay_Z;         // (updated after new fit)
    float DcaV0Daughters;  // (updated after new fit)

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
    float Neg_PCAwrtV0_Px;  // (updated after new fit)
    float Neg_PCAwrtV0_Py;  // (updated after new fit)
    float Neg_PCAwrtV0_Pz;  // (updated after new fit)

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
    float Pos_PCAwrtV0_Px;  // (updated after new fit)
    float Pos_PCAwrtV0_Py;  // (updated after new fit)
    float Pos_PCAwrtV0_Pz;  // (updated after new fit)
};

namespace Extended {

struct PreFoundLambda : POD::PreFoundLambda {
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
    // flag
    bool IsAntiLambda;
};

}  // namespace Extended

}  // namespace POD
