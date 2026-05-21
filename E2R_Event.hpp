#pragma once

#include <memory>

namespace E2R {

struct Event {
    std::shared_ptr<unsigned int> RunNumber;    // run number
    std::shared_ptr<unsigned int> DirNumber;    // directory number
    std::shared_ptr<unsigned int> DirNumberB;   // component after-the-dot of data directories
    std::shared_ptr<unsigned int> EventNumber;  // event number
    std::shared_ptr<float> Centrality;          // centrality percentile
    std::shared_ptr<float> MagneticField;       // magnetic field
    std::shared_ptr<int> PV_NContributors;
    std::shared_ptr<float> PV_Dispersion;
    std::shared_ptr<float> PV_X;
    std::shared_ptr<float> PV_Y;
    std::shared_ptr<float> PV_Z;
    std::shared_ptr<std::array<float, 6>> PV_CovMatrix;
    std::shared_ptr<float> SPD_PV_Z;
    std::shared_ptr<float> SPD_PV_ZErr;
    std::shared_ptr<int> NTracks;
    std::shared_ptr<int> NTPCClusters;
    std::shared_ptr<bool> IsMB;
    std::shared_ptr<bool> IsHighMultV0;
    std::shared_ptr<bool> IsHighMultSPD;
    std::shared_ptr<bool> IsCentral;
    std::shared_ptr<bool> IsSemiCentral;
    // mc info
    std::shared_ptr<float> MC_PV_X;
    std::shared_ptr<float> MC_PV_Y;
    std::shared_ptr<float> MC_PV_Z;
    std::shared_ptr<bool> MC_IsGenPileup;
    std::shared_ptr<bool> MC_IsSBCPileup;
};

}  // namespace E2R
