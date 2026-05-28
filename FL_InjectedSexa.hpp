#pragma once

#include <memory>
#include <string>

#include <ROOT/RNTupleModel.hxx>

namespace Flat {

struct InjectedSexa {

    InjectedSexa() = default;
    explicit InjectedSexa(ROOT::RNTupleModel *model, bool embedded_to_rec) { AddFieldsTo(model, embedded_to_rec); }

    void AddFieldsTo(ROOT::RNTupleModel *model, bool embedded_to_rec) {
        std::string prefix = embedded_to_rec ? "MC_" : "";
        if (!embedded_to_rec) {
            RunNumber = model->MakeField<unsigned int>("RunNumber");
            DirNumber = model->MakeField<unsigned int>("DirNumber");
            EventNumber = model->MakeField<unsigned int>("EventNumber");
        }
        ReactionID = model->MakeField<int>(std::format("{}ReactionID", prefix));
        SV_X = model->MakeField<float>(std::format("{}SV_X", prefix));
        SV_Y = model->MakeField<float>(std::format("{}SV_Y", prefix));
        SV_Z = model->MakeField<float>(std::format("{}SV_Z", prefix));
        Px = model->MakeField<float>(std::format("{}Px", prefix));
        Py = model->MakeField<float>(std::format("{}Py", prefix));
        Pz = model->MakeField<float>(std::format("{}Pz", prefix));
        Energy = model->MakeField<float>(std::format("{}Energy", prefix));
        Nucleon_Px = model->MakeField<float>(std::format("{}Nucleon_Px", prefix));
        Nucleon_Py = model->MakeField<float>(std::format("{}Nucleon_Py", prefix));
        Nucleon_Pz = model->MakeField<float>(std::format("{}Nucleon_Pz", prefix));
        Nucleon_Energy = model->MakeField<float>(std::format("{}Nucleon_Energy", prefix));
    }

    // member variables //
    // -- event info
    std::shared_ptr<unsigned int> RunNumber;
    std::shared_ptr<unsigned int> DirNumber;
    std::shared_ptr<unsigned int> EventNumber;
    // -- anti-sexaquark info
    std::shared_ptr<int> ReactionID;
    std::shared_ptr<float> SV_X;
    std::shared_ptr<float> SV_Y;
    std::shared_ptr<float> SV_Z;
    std::shared_ptr<float> Px;
    std::shared_ptr<float> Py;
    std::shared_ptr<float> Pz;
    std::shared_ptr<float> Energy;
    std::shared_ptr<float> Nucleon_Px;
    std::shared_ptr<float> Nucleon_Py;
    std::shared_ptr<float> Nucleon_Pz;
    std::shared_ptr<float> Nucleon_Energy;
};

}  // namespace Flat
