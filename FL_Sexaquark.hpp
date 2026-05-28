#pragma once

#include <memory>

#include <ROOT/RNTupleModel.hxx>

#include "FL_InjectedSexa.hpp"

namespace Flat {

struct Sexaquark {

    Sexaquark() = default;
    Sexaquark(ROOT::RNTupleModel *model, bool is_mc) {
        // -- event info
        RunNumber = model->MakeField<unsigned int>("RunNumber");
        DirNumber = model->MakeField<unsigned int>("DirNumber");
        if (!is_mc) DirNumberB = model->MakeField<unsigned int>("DirNumberB");
        EventNumber = model->MakeField<unsigned int>("EventNumber");
        Centrality = model->MakeField<float>("Centrality");
        MagneticField = model->MakeField<float>("MagneticField");
        PV_X = model->MakeField<float>("PV_X");
        PV_Y = model->MakeField<float>("PV_Y");
        PV_Z = model->MakeField<float>("PV_Z");
        if (is_mc) {
            MC_PV_X = model->MakeField<float>("MC_PV_X");
            MC_PV_Y = model->MakeField<float>("MC_PV_Y");
            MC_PV_Z = model->MakeField<float>("MC_PV_Z");
        }
        // -- candidate info
        SV_X = model->MakeField<float>("SV_X");
        SV_Y = model->MakeField<float>("SV_Y");
        SV_Z = model->MakeField<float>("SV_Z");
        Px = model->MakeField<float>("Px");
        Py = model->MakeField<float>("Py");
        Pz = model->MakeField<float>("Pz");
        Energy = model->MakeField<float>("E");
        Chi2NDF = model->MakeField<float>("Chi2NDF");
        E_MinusNucleon = model->MakeField<float>("E_MinusNucleon");
        AntiChannel = model->MakeField<bool>("AntiChannel");
        // -- mc info
        if (is_mc) {
            MC.AddFieldsTo(model, true);
            IsSignal = model->MakeField<bool>("IsSignal");
            IsHybrid = model->MakeField<bool>("IsHybrid");
        }
    }

    // member variables //
    // -- event info
    std::shared_ptr<unsigned int> RunNumber;
    std::shared_ptr<unsigned int> DirNumber;
    std::shared_ptr<unsigned int> DirNumberB;  // (rd only)
    std::shared_ptr<unsigned int> EventNumber;
    std::shared_ptr<float> Centrality;
    std::shared_ptr<float> MagneticField;
    std::shared_ptr<float> PV_X;
    std::shared_ptr<float> PV_Y;
    std::shared_ptr<float> PV_Z;
    std::shared_ptr<float> MC_PV_X;  // (mc only)
    std::shared_ptr<float> MC_PV_Y;  // (mc only)
    std::shared_ptr<float> MC_PV_Z;  // (mc only)
    // -- candidate info
    std::shared_ptr<float> SV_X;
    std::shared_ptr<float> SV_Y;
    std::shared_ptr<float> SV_Z;
    std::shared_ptr<float> Px;
    std::shared_ptr<float> Py;
    std::shared_ptr<float> Pz;
    std::shared_ptr<float> Energy;
    std::shared_ptr<float> Chi2NDF;
    std::shared_ptr<float> E_MinusNucleon;
    std::shared_ptr<bool> AntiChannel;
    // -- mc info
    Flat::InjectedSexa MC;
    std::shared_ptr<bool> IsSignal;
    std::shared_ptr<bool> IsHybrid;
};

}  // namespace Flat
