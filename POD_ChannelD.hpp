#pragma once

#include "POD_InjectedSexa.hpp"
#include "POD_Track.hpp"
#include "POD_V0.hpp"

namespace POD {

struct ChannelD {
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
    // -- V0
    POD::V0 V0;
    float V0_PCAwrtSV_X;
    float V0_PCAwrtSV_Y;
    float V0_PCAwrtSV_Z;
    float V0_PCAwrtSV_Px;
    float V0_PCAwrtSV_Py;
    float V0_PCAwrtSV_Pz;
    // -- Kaon
    POD::Track Kaon;
    float Kaon_PCAwrtSV_X;
    float Kaon_PCAwrtSV_Y;
    float Kaon_PCAwrtSV_Z;
    float Kaon_PCAwrtSV_Px;
    float Kaon_PCAwrtSV_Py;
    float Kaon_PCAwrtSV_Pz;
    // -- mc info
    std::optional<POD::InjectedSexa> MC;
    std::optional<bool> IsSignal;
    std::optional<bool> IsHybrid;
};

}  // namespace POD
