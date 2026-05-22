#pragma once

#include <memory>

#include <ROOT/RNTupleModel.hxx>

namespace E2R {

struct Event {

    Event(ROOT::RNTupleModel *model, bool is_mc) {
        RunNumber = model->MakeField<unsigned int>("RunNumber");
        DirNumber = model->MakeField<unsigned int>("DirNumber");
        if (!is_mc) DirNumberB = model->MakeField<unsigned int>("DirNumberB");
        EventNumber = model->MakeField<unsigned int>("EventNumber");
        Centrality = model->MakeField<float>("Centrality");
        MagneticField = model->MakeField<float>("MagneticField");
        PV_NContributors = model->MakeField<int>("PV_NContributors");
        PV_Dispersion = model->MakeField<float>("PV_Dispersion");
        PV_X = model->MakeField<float>("PV_X");
        PV_Y = model->MakeField<float>("PV_Y");
        PV_Z = model->MakeField<float>("PV_Z");
        PV_CovMatrix = model->MakeField<std::array<float, 6>>("PV_CovMatrix");
        SPD_PV_Z = model->MakeField<float>("SPD_PV_Z");
        SPD_PV_ZErr = model->MakeField<float>("SPD_PV_ZErr");
        NTracks = model->MakeField<int>("NTracks");
        NTPCClusters = model->MakeField<int>("NTPCClusters");
        IsMB = model->MakeField<bool>("IsMB");
        IsHighMultV0 = model->MakeField<bool>("IsHighMultV0");
        IsHighMultSPD = model->MakeField<bool>("IsHighMultSPD");
        IsCentral = model->MakeField<bool>("IsCentral");
        IsSemiCentral = model->MakeField<bool>("IsSemiCentral");
        if (is_mc) {
            MC_PV_X = model->MakeField<float>("MC_PV_X");
            MC_PV_Y = model->MakeField<float>("MC_PV_Y");
            MC_PV_Z = model->MakeField<float>("MC_PV_Z");
            MC_IsGenPileup = model->MakeField<bool>("MC_IsGenPileup");
            MC_IsSBCPileup = model->MakeField<bool>("MC_IsSBCPileup");
        }
    }

    // member variables //
    std::shared_ptr<unsigned int> RunNumber;
    std::shared_ptr<unsigned int> DirNumber;
    std::shared_ptr<unsigned int> DirNumberB;   // (real data only) component after-the-dot of data directories
    std::shared_ptr<unsigned int> EventNumber;  // event number in file
    std::shared_ptr<float> Centrality;          // centrality percentile
    std::shared_ptr<float> MagneticField;
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
