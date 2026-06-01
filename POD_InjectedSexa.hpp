#pragma once

#include <optional>

namespace POD {

struct InjectedSexa {
    unsigned int ReactionID;
    float Px;
    float Py;
    float Pz;
    float Nucleon_Px;
    float Nucleon_Py;
    float Nucleon_Pz;
    // (filled in `Packager`)
    std::optional<float> SV_X;
    std::optional<float> SV_Y;
    std::optional<float> SV_Z;
    std::optional<float> Energy;
    std::optional<float> Nucleon_Energy;
};

}  // namespace POD
