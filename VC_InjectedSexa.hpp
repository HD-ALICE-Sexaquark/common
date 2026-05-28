#pragma once

#include <memory>
#include <vector>

#include <ROOT/RNTupleModel.hxx>

namespace Vector {

struct InjectedSexa {

    InjectedSexa() = default;
    explicit InjectedSexa(ROOT::RNTupleModel *model, bool include_sv) { AddFieldsTo(model, include_sv); }

    void AddFieldsTo(ROOT::RNTupleModel *model, bool include_sv) {
        ReactionID = model->MakeField<std::vector<unsigned int>>("ReactionID");
        if (include_sv) {
            SV_X = model->MakeField<std::vector<float>>("SV_X");
            SV_Y = model->MakeField<std::vector<float>>("SV_Y");
            SV_Z = model->MakeField<std::vector<float>>("SV_Z");
        }
        Sexa_Px = model->MakeField<std::vector<float>>("Sexa_Px");
        Sexa_Py = model->MakeField<std::vector<float>>("Sexa_Py");
        Sexa_Pz = model->MakeField<std::vector<float>>("Sexa_Pz");
        Nucleon_Px = model->MakeField<std::vector<float>>("Nucleon_Px");
        Nucleon_Py = model->MakeField<std::vector<float>>("Nucleon_Py");
        Nucleon_Pz = model->MakeField<std::vector<float>>("Nucleon_Pz");
    }

    void Clear(bool include_sv) {
        ReactionID->clear();
        if (include_sv) {
            SV_X->clear();
            SV_Y->clear();
            SV_Z->clear();
        }
        Sexa_Px->clear();
        Sexa_Py->clear();
        Sexa_Pz->clear();
        Nucleon_Px->clear();
        Nucleon_Py->clear();
        Nucleon_Pz->clear();
    }

    // member variables //
    std::shared_ptr<std::vector<unsigned int>> ReactionID;
    std::shared_ptr<std::vector<float>> SV_X;
    std::shared_ptr<std::vector<float>> SV_Y;
    std::shared_ptr<std::vector<float>> SV_Z;
    std::shared_ptr<std::vector<float>> Sexa_Px;
    std::shared_ptr<std::vector<float>> Sexa_Py;
    std::shared_ptr<std::vector<float>> Sexa_Pz;
    std::shared_ptr<std::vector<float>> Nucleon_Px;
    std::shared_ptr<std::vector<float>> Nucleon_Py;
    std::shared_ptr<std::vector<float>> Nucleon_Pz;
};

}  // namespace Vector
