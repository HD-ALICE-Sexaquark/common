#pragma once

#include <array>
#include <optional>

namespace POD {

struct Event {
    unsigned int RunNumber;
    unsigned int DirNumber;
    std::optional<unsigned int> DirNumberB;  // (rd only)
    unsigned int EventNumber;
    float Centrality;
    float MagneticField;
    float PV_X;
    float PV_Y;
    float PV_Z;
    std::optional<int> PV_NContributors;               // (optional indeed)
    std::optional<float> PV_Dispersion;                // (optional indeed)
    std::optional<std::array<float, 6>> PV_CovMatrix;  // (optional indeed)
    std::optional<float> SPD_PV_Z;                     // (optional indeed)
    std::optional<float> SPD_PV_ZErr;                  // (optional indeed)
    std::optional<int> NTracks;                        // (optional indeed)
    std::optional<int> NTPCClusters;                   // (optional indeed)
    std::optional<bool> IsMB;                          // (optional indeed)
    std::optional<bool> IsHighMultV0;                  // (optional indeed)
    std::optional<bool> IsHighMultSPD;                 // (optional indeed)
    std::optional<bool> IsCentral;                     // (optional indeed)
    std::optional<bool> IsSemiCentral;                 // (optional indeed)
    std::optional<float> MC_PV_X;                      // (mc only)
    std::optional<float> MC_PV_Y;                      // (mc only)
    std::optional<float> MC_PV_Z;                      // (mc only)
    std::optional<bool> MC_IsGenPileup;                // (mc only)
    std::optional<bool> MC_IsSBCPileup;                // (mc only)
};

}  // namespace POD
