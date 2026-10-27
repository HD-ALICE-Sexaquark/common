#pragma once

#include <array>
#include <cstdint>

#include "Constants.hpp"

namespace POD {

struct PreFoundLambda {
    // (anti)lambda info //
    unsigned int PreFoundEntry;
    float Decay_X;
    float Decay_Y;
    float Decay_Z;
    float DcaV0Daughters;
    bool PassesOnFlyLikelihood;  // passed the likelihood/causality cuts of the on-the-fly v0 finder

    // negative daughter //
    unsigned int Neg_EsdEntry;
    std::array<float, 6> Neg_State;                                        // from parametrization from AliESDv0
    std::array<float, Common::NCovMatrixComponents_State6> Neg_CovMatrix;  // from parametrization from AliESDv0
    float Neg_PreDCAxy;                                                    // signed pre-calculated DCA wrt PV
    float Neg_PreDCAz;                                                     // signed pre-calculated DCA wrt PV
    float Neg_NSigmasProton;
    float Neg_NSigmasKaon;
    float Neg_NSigmasPion;
    float Neg_TPC_Signal;
    float Neg_TPC_NCrossedRows;
    unsigned short Neg_TPC_NClusters;
    unsigned short Neg_TPC_NClustersFindable;
    float Neg_TPC_Chi2;
    float Neg_TPC_Length;  // in the active zone
    bool Neg_ITS_Refit;
    std::uint8_t Neg_ITS_ClusterMap;
    int Neg_TOF_BunchCrossing;
    // -- related to (anti)lambda
    float Neg_PCAwrtV0_Px;
    float Neg_PCAwrtV0_Py;
    float Neg_PCAwrtV0_Pz;

    // positive daughter //
    unsigned int Pos_EsdEntry;
    std::array<float, 6> Pos_State;                                        // from parametrization from AliESDv0
    std::array<float, Common::NCovMatrixComponents_State6> Pos_CovMatrix;  // from parametrization from AliESDv0
    float Pos_PreDCAxy;                                                    // signed pre-calculated DCA wrt PV
    float Pos_PreDCAz;                                                     // signed pre-calculated DCA wrt PV
    float Pos_NSigmasProton;
    float Pos_NSigmasKaon;
    float Pos_NSigmasPion;
    float Pos_TPC_Signal;
    float Pos_TPC_NCrossedRows;
    unsigned short Pos_TPC_NClusters;
    unsigned short Pos_TPC_NClustersFindable;
    float Pos_TPC_Chi2;
    float Pos_TPC_Length;  // in the active zone
    bool Pos_ITS_Refit;
    std::uint8_t Pos_ITS_ClusterMap;
    int Pos_TOF_BunchCrossing;
    // -- related to (anti)lambda
    float Pos_PCAwrtV0_Px;
    float Pos_PCAwrtV0_Py;
    float Pos_PCAwrtV0_Pz;
};

}  // namespace POD
