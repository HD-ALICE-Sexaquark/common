#pragma once

#include "POD_InjectedSexa.hpp"
#include "POD_Track.hpp"

namespace POD {

struct ChannelH {
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
    // -- Kaon1
    POD::Track Kaon1;
    float Kaon1_PCAwrtSV_X;
    float Kaon1_PCAwrtSV_Y;
    float Kaon1_PCAwrtSV_Z;
    float Kaon1_PCAwrtSV_Px;
    float Kaon1_PCAwrtSV_Py;
    float Kaon1_PCAwrtSV_Pz;
    // -- Kaon2
    POD::Track Kaon2;
    float Kaon2_PCAwrtSV_X;
    float Kaon2_PCAwrtSV_Y;
    float Kaon2_PCAwrtSV_Z;
    float Kaon2_PCAwrtSV_Px;
    float Kaon2_PCAwrtSV_Py;
    float Kaon2_PCAwrtSV_Pz;
    // -- mc info
    std::optional<POD::InjectedSexa> MC;
    std::optional<bool> IsSignal;
    std::optional<bool> IsHybrid;
};

}  // namespace POD
