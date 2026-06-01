#pragma once

#include "POD_InjectedSexa.hpp"
#include "POD_V0.hpp"

namespace POD {

struct ChannelA {
    // -- candidate info
    float SV_X;
    float SV_Y;
    float SV_Z;
    float Px;
    float Py;
    float Pz;
    float Energy;
    float Chi2NDF;
    float E_MinusNucleon;
    bool ControlChannel;
    // -- V0A
    POD::V0 V0A;
    float V0A_PCAwrtSV_X;
    float V0A_PCAwrtSV_Y;
    float V0A_PCAwrtSV_Z;
    float V0A_PCAwrtSV_Px;
    float V0A_PCAwrtSV_Py;
    float V0A_PCAwrtSV_Pz;
    // -- V0B
    POD::V0 V0B;
    float V0B_PCAwrtSV_X;
    float V0B_PCAwrtSV_Y;
    float V0B_PCAwrtSV_Z;
    float V0B_PCAwrtSV_Px;
    float V0B_PCAwrtSV_Py;
    float V0B_PCAwrtSV_Pz;
    // -- mc info
    std::optional<POD::InjectedSexa> MC;
    std::optional<bool> IsSignal;
    std::optional<bool> IsHybrid;
};

}  // namespace POD
