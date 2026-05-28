#pragma once

#include <ROOT/RNTupleModel.hxx>

#include "FL_Sexaquark.hpp"
#include "FL_V0.hpp"

namespace Flat {

struct ChannelA : Sexaquark {

    ChannelA() = default;
    ChannelA(ROOT::RNTupleModel *model, bool is_mc) : Sexaquark{model, is_mc} { AddFieldsTo(model, is_mc); }

    void AddFieldsTo(ROOT::RNTupleModel *model, bool is_mc) {
        // -- V0A
        V0A.AddFieldsTo(model, is_mc, "V0A");
        V0A_PCAwrtSV_X = model->MakeField<float>("V0A_PCAwrtSV_X");
        V0A_PCAwrtSV_Y = model->MakeField<float>("V0A_PCAwrtSV_Y");
        V0A_PCAwrtSV_Z = model->MakeField<float>("V0A_PCAwrtSV_Z");
        V0A_PCAwrtSV_Px = model->MakeField<float>("V0A_PCAwrtSV_Px");
        V0A_PCAwrtSV_Py = model->MakeField<float>("V0A_PCAwrtSV_Py");
        V0A_PCAwrtSV_Pz = model->MakeField<float>("V0A_PCAwrtSV_Pz");
        // -- V0B
        V0B.AddFieldsTo(model, is_mc, "V0B");
        V0B_PCAwrtSV_X = model->MakeField<float>("V0B_PCAwrtSV_X");
        V0B_PCAwrtSV_Y = model->MakeField<float>("V0B_PCAwrtSV_Y");
        V0B_PCAwrtSV_Z = model->MakeField<float>("V0B_PCAwrtSV_Z");
        V0B_PCAwrtSV_Px = model->MakeField<float>("V0B_PCAwrtSV_Px");
        V0B_PCAwrtSV_Py = model->MakeField<float>("V0B_PCAwrtSV_Py");
        V0B_PCAwrtSV_Pz = model->MakeField<float>("V0B_PCAwrtSV_Pz");
    }

    // -- composition
    Flat::V0 V0A;
    Flat::V0 V0B;
    // -- V0A
    std::shared_ptr<float> V0A_PCAwrtSV_X;
    std::shared_ptr<float> V0A_PCAwrtSV_Y;
    std::shared_ptr<float> V0A_PCAwrtSV_Z;
    std::shared_ptr<float> V0A_PCAwrtSV_Px;
    std::shared_ptr<float> V0A_PCAwrtSV_Py;
    std::shared_ptr<float> V0A_PCAwrtSV_Pz;
    // -- V0B
    std::shared_ptr<float> V0B_PCAwrtSV_X;
    std::shared_ptr<float> V0B_PCAwrtSV_Y;
    std::shared_ptr<float> V0B_PCAwrtSV_Z;
    std::shared_ptr<float> V0B_PCAwrtSV_Px;
    std::shared_ptr<float> V0B_PCAwrtSV_Py;
    std::shared_ptr<float> V0B_PCAwrtSV_Pz;
};

}  // namespace Flat
