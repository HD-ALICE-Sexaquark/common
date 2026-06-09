#pragma once

#include <array>

namespace POD {

struct Event {
    unsigned int RunNumber;
    unsigned int DirNumber;
    unsigned int DirNumberB;  // (valuable for rd only)
    unsigned int EventNumber;
    float Centrality;
    float MagneticField;
    float PV_X;
    float PV_Y;
    float PV_Z;
};

namespace Extended {
struct Event : POD::Event {
    int PV_NContributors;
    float PV_Dispersion;
    std::array<float, 6> PV_CovMatrix;
    float SPD_PV_Z;
    float SPD_PV_ZErr;
    int NTracks;
    int NTPCClusters;
    bool IsMB;
    bool IsCentral;
    bool IsSemiCentral;
};
}  // namespace Extended

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
