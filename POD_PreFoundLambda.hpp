#pragma once

#include <array>
#include <cstdint>

#include "Constants.hpp"

namespace POD {

struct PreFoundLambda {
    // (anti)lambda info //
    unsigned int PreFoundEntry{0};
    float Decay_X{Common::DummyFloat};
    float Decay_Y{Common::DummyFloat};
    float Decay_Z{Common::DummyFloat};
    float DcaV0Daughters{Common::DummyFloat};
    bool PassesOnFlyLikelihood{false};  // passed the likelihood/causality cuts of the on-the-fly v0 finder

    // negative daughter //
    unsigned int Neg_EsdEntry{0};
    std::array<float, 6> Neg_State{};                                        // from parametrization from AliESDv0
    std::array<float, Common::NCovMatrixComponents_State6> Neg_CovMatrix{};  // from parametrization from AliESDv0
    float Neg_PreDCAxy{Common::DummyFloat};                                  // signed pre-calculated DCA wrt PV
    float Neg_PreDCAz{Common::DummyFloat};                                   // signed pre-calculated DCA wrt PV
    float Neg_NSigmasProton{Common::DummyFloat};
    float Neg_NSigmasKaon{Common::DummyFloat};
    float Neg_NSigmasPion{Common::DummyFloat};
    float Neg_TPC_Signal{Common::DummyFloat};
    float Neg_TPC_NCrossedRows{Common::DummyFloat};
    unsigned short Neg_TPC_NClusters{0};
    unsigned short Neg_TPC_NClustersFindable{0};
    float Neg_TPC_Chi2{Common::DummyFloat};
    float Neg_TPC_Length{Common::DummyFloat};  // in the active zone
    bool Neg_ITS_Refit{false};
    std::uint8_t Neg_ITS_ClusterMap{0};
    int Neg_TOF_BunchCrossing{Common::DummyInt};
    // -- related to (anti)lambda
    float Neg_PCAwrtV0_Px{Common::DummyFloat};
    float Neg_PCAwrtV0_Py{Common::DummyFloat};
    float Neg_PCAwrtV0_Pz{Common::DummyFloat};

    // positive daughter //
    unsigned int Pos_EsdEntry{0};
    std::array<float, 6> Pos_State{};                                        // from parametrization from AliESDv0
    std::array<float, Common::NCovMatrixComponents_State6> Pos_CovMatrix{};  // from parametrization from AliESDv0
    float Pos_PreDCAxy{Common::DummyFloat};                                  // signed pre-calculated DCA wrt PV
    float Pos_PreDCAz{Common::DummyFloat};                                   // signed pre-calculated DCA wrt PV
    float Pos_NSigmasProton{Common::DummyFloat};
    float Pos_NSigmasKaon{Common::DummyFloat};
    float Pos_NSigmasPion{Common::DummyFloat};
    float Pos_TPC_Signal{Common::DummyFloat};
    float Pos_TPC_NCrossedRows{Common::DummyFloat};
    unsigned short Pos_TPC_NClusters{0};
    unsigned short Pos_TPC_NClustersFindable{0};
    float Pos_TPC_Chi2{Common::DummyFloat};
    float Pos_TPC_Length{Common::DummyFloat};  // in the active zone
    bool Pos_ITS_Refit{false};
    std::uint8_t Pos_ITS_ClusterMap{0};
    int Pos_TOF_BunchCrossing{Common::DummyInt};
    // -- related to (anti)lambda
    float Pos_PCAwrtV0_Px{Common::DummyFloat};
    float Pos_PCAwrtV0_Py{Common::DummyFloat};
    float Pos_PCAwrtV0_Pz{Common::DummyFloat};
};

}  // namespace POD
