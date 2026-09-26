#pragma once

#include <array>

#include "Constants.hpp"

namespace POD {

struct Event {
    unsigned int RunNumber;
    unsigned int DirNumber;
    unsigned int DirNumberB;  // (valuable for rd only)
    unsigned int EventNumber;
    float MagneticField;
    float PV_X;
    float PV_Y;
    float PV_Z;
    std::array<float, Common::NCovMatrixComponents_State3> PV_CovMatrix;
    float Centrality;
    bool IsINT7;
    bool IsCentral;
    bool IsSemiCentral;
};

namespace MC {
struct Event {
    float PV_X;
    float PV_Y;
    float PV_Z;
    bool IsGenPileup;
    bool IsSBCPileup;
};
}  // namespace MC

}  // namespace POD
