#pragma once

#include <memory>
#include <string_view>

#include <ROOT/RNTupleModel.hxx>

#include "FL_Track.hpp"

namespace Flat {

struct V0 {

    V0() = default;
    V0(ROOT::RNTupleModel *model, bool is_mc, std::string_view acronym) { AddFieldsTo(model, is_mc, acronym); }

    void AddFieldsTo(ROOT::RNTupleModel *model, bool is_mc, std::string_view acronym) {
        Decay_X = model->MakeField<float>(std::format("{}_Decay_X", acronym));
        Decay_Y = model->MakeField<float>(std::format("{}_Decay_Y", acronym));
        Decay_Z = model->MakeField<float>(std::format("{}_Decay_Z", acronym));
        Px = model->MakeField<float>(std::format("{}_Px", acronym));
        Py = model->MakeField<float>(std::format("{}_Py", acronym));
        Pz = model->MakeField<float>(std::format("{}_Pz", acronym));
        Energy = model->MakeField<float>(std::format("{}_Energy", acronym));
        Chi2NDF = model->MakeField<float>(std::format("{}_Chi2NDF", acronym));
        if (is_mc) {
            McEntry = model->MakeField<int>(std::format("MC_{}_McEntry", acronym));
            PdgCode = model->MakeField<int>(std::format("MC_{}_PdgCode", acronym));
            Origin_X = model->MakeField<float>(std::format("MC_{}_Origin_X", acronym));
            Origin_Y = model->MakeField<float>(std::format("MC_{}_Origin_Y", acronym));
            Origin_Z = model->MakeField<float>(std::format("MC_{}_Origin_Z", acronym));
            MC_Decay_X = model->MakeField<float>(std::format("MC_{}_Decay_X", acronym));
            MC_Decay_Y = model->MakeField<float>(std::format("MC_{}_Decay_Y", acronym));
            MC_Decay_Z = model->MakeField<float>(std::format("MC_{}_Decay_Z", acronym));
            MC_Px = model->MakeField<float>(std::format("MC_{}_Px", acronym));
            MC_Py = model->MakeField<float>(std::format("MC_{}_Py", acronym));
            MC_Pz = model->MakeField<float>(std::format("MC_{}_Pz", acronym));
            MC_Energy = model->MakeField<float>(std::format("MC_{}_Energy", acronym));
            IsTrue = model->MakeField<bool>(std::format("MC_{}_IsTrue", acronym));
            IsSecondary = model->MakeField<bool>(std::format("MC_{}_IsSecondary", acronym));
            IsSignal = model->MakeField<bool>(std::format("MC_{}_IsSignal", acronym));
            IsHybrid = model->MakeField<bool>(std::format("MC_{}_IsHybrid", acronym));
            ReactionID = model->MakeField<int>(std::format("MC_{}_ReactionID", acronym));
            Mother_McEntry = model->MakeField<int>(std::format("MC_{}_Mother_McEntry", acronym));
            Mother_PdgCode = model->MakeField<int>(std::format("MC_{}_Mother_PdgCode", acronym));
        }
    }

    // composition //
    Flat::Track Neg;
    Flat::Track Pos;
    // member variables //
    std::shared_ptr<float> Decay_X;
    std::shared_ptr<float> Decay_Y;
    std::shared_ptr<float> Decay_Z;
    std::shared_ptr<float> Px;
    std::shared_ptr<float> Py;
    std::shared_ptr<float> Pz;
    std::shared_ptr<float> Energy;
    std::shared_ptr<float> Chi2NDF;
    // mc info
    std::shared_ptr<int> McEntry;
    std::shared_ptr<int> PdgCode;
    std::shared_ptr<float> Origin_X;
    std::shared_ptr<float> Origin_Y;
    std::shared_ptr<float> Origin_Z;
    std::shared_ptr<float> MC_Decay_X;
    std::shared_ptr<float> MC_Decay_Y;
    std::shared_ptr<float> MC_Decay_Z;
    std::shared_ptr<float> MC_Px;
    std::shared_ptr<float> MC_Py;
    std::shared_ptr<float> MC_Pz;
    std::shared_ptr<float> MC_Energy;
    std::shared_ptr<bool> IsTrue;
    std::shared_ptr<bool> IsSecondary;
    std::shared_ptr<bool> IsSignal;
    std::shared_ptr<bool> IsHybrid;
    std::shared_ptr<int> ReactionID;
    std::shared_ptr<int> Mother_McEntry;
    std::shared_ptr<int> Mother_PdgCode;
};

}  // namespace Flat
