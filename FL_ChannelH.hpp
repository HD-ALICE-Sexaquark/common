#pragma once

#include <ROOT/RNTupleModel.hxx>

#include "FL_Sexaquark.hpp"
#include "FL_Track.hpp"

namespace Flat {

struct ChannelH : Sexaquark {

    ChannelH() = default;
    ChannelH(ROOT::RNTupleModel *model, bool is_mc) : Sexaquark{model, is_mc} {}

    void AddFieldsTo(ROOT::RNTupleModel *model, bool is_mc) {
        // -- Kaon1
        Kaon1.AddFieldsTo(model, is_mc, "Kaon1", "SV", true);
        Kaon1_PCAwrtSV_X = model->MakeField<float>("Kaon1_PCAwrtSV_X");
        Kaon1_PCAwrtSV_Y = model->MakeField<float>("Kaon1_PCAwrtSV_Y");
        Kaon1_PCAwrtSV_Z = model->MakeField<float>("Kaon1_PCAwrtSV_Z");
        Kaon1_PCAwrtSV_Px = model->MakeField<float>("Kaon1_PCAwrtSV_Px");
        Kaon1_PCAwrtSV_Py = model->MakeField<float>("Kaon1_PCAwrtSV_Py");
        Kaon1_PCAwrtSV_Pz = model->MakeField<float>("Kaon1_PCAwrtSV_Pz");
        // -- Kaon2
        Kaon2.AddFieldsTo(model, is_mc, "Kaon2", "SV", true);
        Kaon2_PCAwrtSV_X = model->MakeField<float>("Kaon2_PCAwrtSV_X");
        Kaon2_PCAwrtSV_Y = model->MakeField<float>("Kaon2_PCAwrtSV_Y");
        Kaon2_PCAwrtSV_Z = model->MakeField<float>("Kaon2_PCAwrtSV_Z");
        Kaon2_PCAwrtSV_Px = model->MakeField<float>("Kaon2_PCAwrtSV_Px");
        Kaon2_PCAwrtSV_Py = model->MakeField<float>("Kaon2_PCAwrtSV_Py");
        Kaon2_PCAwrtSV_Pz = model->MakeField<float>("Kaon2_PCAwrtSV_Pz");
    }

    // composition //
    Track Kaon1;
    Track Kaon2;
    // member variables //
    // -- Kaon1
    std::shared_ptr<float> Kaon1_PCAwrtSV_X;
    std::shared_ptr<float> Kaon1_PCAwrtSV_Y;
    std::shared_ptr<float> Kaon1_PCAwrtSV_Z;
    std::shared_ptr<float> Kaon1_PCAwrtSV_Px;
    std::shared_ptr<float> Kaon1_PCAwrtSV_Py;
    std::shared_ptr<float> Kaon1_PCAwrtSV_Pz;
    // -- Kaon2
    std::shared_ptr<float> Kaon2_PCAwrtSV_X;
    std::shared_ptr<float> Kaon2_PCAwrtSV_Y;
    std::shared_ptr<float> Kaon2_PCAwrtSV_Z;
    std::shared_ptr<float> Kaon2_PCAwrtSV_Px;
    std::shared_ptr<float> Kaon2_PCAwrtSV_Py;
    std::shared_ptr<float> Kaon2_PCAwrtSV_Pz;
};

}  // namespace Flat
