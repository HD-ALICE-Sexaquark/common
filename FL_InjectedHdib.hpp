#pragma once

#include <memory>
#include <string>

#include <ROOT/RNTupleModel.hxx>

namespace Flat {

struct InjectedHdib {

    InjectedHdib() = default;
    explicit InjectedHdib(ROOT::RNTupleModel *model, bool embedded_to_rec) { AddFieldsTo(model, embedded_to_rec); }

    void AddFieldsTo(ROOT::RNTupleModel *model, bool embedded_to_rec) {
        std::string prefix = embedded_to_rec ? "MC_" : "";
        if (!embedded_to_rec) {
            RunNumber = model->MakeField<unsigned int>("RunNumber");
            DirNumber = model->MakeField<unsigned int>("DirNumber");
            EventNumber = model->MakeField<unsigned int>("EventNumber");
        }
        InjectionID = model->MakeField<int>(std::format("{}InjectionID", prefix));
        PdgCode = model->MakeField<int>(std::format("{}PdgCode", prefix));
        Decay_X = model->MakeField<float>(std::format("{}Decay_X", prefix));
        Decay_Y = model->MakeField<float>(std::format("{}Decay_Y", prefix));
        Decay_Z = model->MakeField<float>(std::format("{}Decay_Z", prefix));
        Px = model->MakeField<float>(std::format("{}Px", prefix));
        Py = model->MakeField<float>(std::format("{}Py", prefix));
        Pz = model->MakeField<float>(std::format("{}Pz", prefix));
        Energy = model->MakeField<float>(std::format("{}Energy", prefix));
    }

    // member variables //
    // -- event info
    std::shared_ptr<unsigned int> RunNumber;
    std::shared_ptr<unsigned int> DirNumber;
    std::shared_ptr<unsigned int> EventNumber;
    // -- h-dibaryon info
    std::shared_ptr<int> InjectionID;
    std::shared_ptr<int> PdgCode;
    std::shared_ptr<float> Decay_X;
    std::shared_ptr<float> Decay_Y;
    std::shared_ptr<float> Decay_Z;
    std::shared_ptr<float> Px;
    std::shared_ptr<float> Py;
    std::shared_ptr<float> Pz;
    std::shared_ptr<float> Energy;
};

}  // namespace Flat
