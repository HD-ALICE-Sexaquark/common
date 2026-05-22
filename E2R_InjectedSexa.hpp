#pragma once

#include <memory>
#include <vector>

#include <ROOT/RNTupleModel.hxx>

namespace E2R {

struct InjectedSexa {
    void AddToModel(ROOT::RNTupleModel *model) {
        ReactionID = model->MakeField<std::vector<int>>("ReactionID");
        Px = model->MakeField<std::vector<float>>("Sexa_Px");
        Py = model->MakeField<std::vector<float>>("Sexa_Py");
        Pz = model->MakeField<std::vector<float>>("Sexa_Pz");
        Nucleon_Px = model->MakeField<std::vector<float>>("Nucleon_Px");
        Nucleon_Py = model->MakeField<std::vector<float>>("Nucleon_Py");
        Nucleon_Pz = model->MakeField<std::vector<float>>("Nucleon_Pz");
    }

    // member variables //
    std::shared_ptr<std::vector<int>> ReactionID;
    std::shared_ptr<std::vector<float>> Px;
    std::shared_ptr<std::vector<float>> Py;
    std::shared_ptr<std::vector<float>> Pz;
    std::shared_ptr<std::vector<float>> Nucleon_Px;
    std::shared_ptr<std::vector<float>> Nucleon_Py;
    std::shared_ptr<std::vector<float>> Nucleon_Pz;
};

}  // namespace E2R
