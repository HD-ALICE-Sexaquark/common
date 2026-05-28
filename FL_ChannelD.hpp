#pragma once

#include <ROOT/RNTupleModel.hxx>

#include "FL_Sexaquark.hpp"
#include "FL_Track.hpp"
#include "FL_V0.hpp"

namespace Flat {

struct ChannelD : Sexaquark {

    ChannelD() = default;
    ChannelD(ROOT::RNTupleModel *model, bool is_mc) : Sexaquark{model, is_mc} { AddFieldsTo(model, is_mc); }

    void AddFieldsTo(ROOT::RNTupleModel *model, bool is_mc) {
        // -- V0
        V0.AddFieldsTo(model, is_mc, "V0");
        V0_PCAwrtSV_X = model->MakeField<float>("V0_PCAwrtSV_X");
        V0_PCAwrtSV_Y = model->MakeField<float>("V0_PCAwrtSV_Y");
        V0_PCAwrtSV_Z = model->MakeField<float>("V0_PCAwrtSV_Z");
        V0_PCAwrtSV_Px = model->MakeField<float>("V0_PCAwrtSV_Px");
        V0_PCAwrtSV_Py = model->MakeField<float>("V0_PCAwrtSV_Py");
        V0_PCAwrtSV_Pz = model->MakeField<float>("V0_PCAwrtSV_Pz");
        // -- Kaon
        Kaon.AddFieldsTo(model, is_mc, "Kaon", "SV", true);
        Kaon_PCAwrtSV_X = model->MakeField<float>("Kaon_PCAwrtSV_X");
        Kaon_PCAwrtSV_Y = model->MakeField<float>("Kaon_PCAwrtSV_Y");
        Kaon_PCAwrtSV_Z = model->MakeField<float>("Kaon_PCAwrtSV_Z");
        Kaon_PCAwrtSV_Px = model->MakeField<float>("Kaon_PCAwrtSV_Px");
        Kaon_PCAwrtSV_Py = model->MakeField<float>("Kaon_PCAwrtSV_Py");
        Kaon_PCAwrtSV_Pz = model->MakeField<float>("Kaon_PCAwrtSV_Pz");
    }

    // composition //
    Flat::V0 V0;
    Flat::Track Kaon;
    // member variables //
    // -- V0
    std::shared_ptr<float> V0_PCAwrtSV_X;
    std::shared_ptr<float> V0_PCAwrtSV_Y;
    std::shared_ptr<float> V0_PCAwrtSV_Z;
    std::shared_ptr<float> V0_PCAwrtSV_Px;
    std::shared_ptr<float> V0_PCAwrtSV_Py;
    std::shared_ptr<float> V0_PCAwrtSV_Pz;
    // -- Kaon
    std::shared_ptr<float> Kaon_PCAwrtSV_X;
    std::shared_ptr<float> Kaon_PCAwrtSV_Y;
    std::shared_ptr<float> Kaon_PCAwrtSV_Z;
    std::shared_ptr<float> Kaon_PCAwrtSV_Px;
    std::shared_ptr<float> Kaon_PCAwrtSV_Py;
    std::shared_ptr<float> Kaon_PCAwrtSV_Pz;
};

}  // namespace Flat
