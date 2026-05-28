#pragma once

#include <memory>

#include <ROOT/RNTupleModel.hxx>

#include "common/FL_InjectedHdib.hpp"
#include "common/FL_OnTheFlyLambda.hpp"

namespace Flat {

struct LambdaPair {

    LambdaPair() = default;
    LambdaPair(ROOT::RNTupleModel *model, bool is_mc) { AddFieldsTo(model, is_mc); }

    void AddFieldsTo(ROOT::RNTupleModel *model, bool is_mc) {
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
        Decay_X = model->MakeField<float>("Decay_X");
        Decay_Y = model->MakeField<float>("Decay_Y");
        Decay_Z = model->MakeField<float>("Decay_Z");
        Px = model->MakeField<float>("Px");
        Py = model->MakeField<float>("Py");
        Pz = model->MakeField<float>("Pz");
        Energy = model->MakeField<float>("E");
        AntiChannel = model->MakeField<bool>("AntiChannel");
        // -- lambda1
        Lambda1.AddFieldsTo(model, is_mc, "L1");
        Lambda1_PCAwrtDV_X = model->MakeField<float>("L1_PCAwrtDV_X");
        Lambda1_PCAwrtDV_Y = model->MakeField<float>("L1_PCAwrtDV_Y");
        Lambda1_PCAwrtDV_Z = model->MakeField<float>("L1_PCAwrtDV_Z");
        Lambda1_PCAwrtDV_Px = model->MakeField<float>("L1_PCAwrtDV_Px");
        Lambda1_PCAwrtDV_Py = model->MakeField<float>("L1_PCAwrtDV_Py");
        Lambda1_PCAwrtDV_Pz = model->MakeField<float>("L1_PCAwrtDV_Pz");
        // -- lambda2
        Lambda2.AddFieldsTo(model, is_mc, "L2");
        Lambda2_PCAwrtDV_X = model->MakeField<float>("L2_PCAwrtDV_X");
        Lambda2_PCAwrtDV_Y = model->MakeField<float>("L2_PCAwrtDV_Y");
        Lambda2_PCAwrtDV_Z = model->MakeField<float>("L2_PCAwrtDV_Z");
        Lambda2_PCAwrtDV_Px = model->MakeField<float>("L2_PCAwrtDV_Px");
        Lambda2_PCAwrtDV_Py = model->MakeField<float>("L2_PCAwrtDV_Py");
        Lambda2_PCAwrtDV_Pz = model->MakeField<float>("L2_PCAwrtDV_Pz");
        // -- mc info
        if (is_mc) MC.AddFieldsTo(model, true);
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
    std::shared_ptr<float> Decay_X;
    std::shared_ptr<float> Decay_Y;
    std::shared_ptr<float> Decay_Z;
    std::shared_ptr<float> Px;
    std::shared_ptr<float> Py;
    std::shared_ptr<float> Pz;
    std::shared_ptr<float> Energy;
    std::shared_ptr<bool> AntiChannel;
    // -- lambda1
    Flat::OnTheFlyLambda Lambda1;
    std::shared_ptr<float> Lambda1_PCAwrtDV_X;
    std::shared_ptr<float> Lambda1_PCAwrtDV_Y;
    std::shared_ptr<float> Lambda1_PCAwrtDV_Z;
    std::shared_ptr<float> Lambda1_PCAwrtDV_Px;
    std::shared_ptr<float> Lambda1_PCAwrtDV_Py;
    std::shared_ptr<float> Lambda1_PCAwrtDV_Pz;
    // -- lambda2
    Flat::OnTheFlyLambda Lambda2;
    std::shared_ptr<float> Lambda2_PCAwrtDV_X;
    std::shared_ptr<float> Lambda2_PCAwrtDV_Y;
    std::shared_ptr<float> Lambda2_PCAwrtDV_Z;
    std::shared_ptr<float> Lambda2_PCAwrtDV_Px;
    std::shared_ptr<float> Lambda2_PCAwrtDV_Py;
    std::shared_ptr<float> Lambda2_PCAwrtDV_Pz;
    // -- mc info
    Flat::InjectedHdib MC;
};

}  // namespace Flat
