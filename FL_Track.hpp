#pragma once

#include <memory>
#include <string_view>

#include <ROOT/RNTupleModel.hxx>

namespace Flat {

struct Track {

    Track() = default;
    Track(ROOT::RNTupleModel *model, bool is_mc, std::string_view acronym, std::string_view name_ref_vtx, bool include_gm) {
        AddFieldsTo(model, is_mc, acronym, name_ref_vtx, include_gm);
    }

    void AddFieldsTo(ROOT::RNTupleModel *model, bool is_mc, std::string_view acronym, std::string_view name_ref_vtx, bool include_gm) {
        EsdEntry = model->MakeField<unsigned int>(std::format("{}_EsdEntry", acronym));
        X = model->MakeField<float>(std::format("{}_X", acronym));
        Y = model->MakeField<float>(std::format("{}_Y", acronym));
        Z = model->MakeField<float>(std::format("{}_Z", acronym));
        Px = model->MakeField<float>(std::format("{}_Px", acronym));
        Py = model->MakeField<float>(std::format("{}_Py", acronym));
        Pz = model->MakeField<float>(std::format("{}_Pz", acronym));
        PreDCAxy = model->MakeField<float>(std::format("{}_PreDCAxy", acronym));
        PreDCAz = model->MakeField<float>(std::format("{}_PreDCAz", acronym));
        TPC_Signal = model->MakeField<float>(std::format("{}_TPC_Signal", acronym));
        NSigmaPion = model->MakeField<float>(std::format("{}_NSigmaPion", acronym));
        NSigmaKaon = model->MakeField<float>(std::format("{}_NSigmaKaon", acronym));
        NSigmaProton = model->MakeField<float>(std::format("{}_NSigmaProton", acronym));
        PCAwrtXX_X = model->MakeField<float>(std::format("{}_PCAwrt{}_X", acronym, name_ref_vtx));
        PCAwrtXX_Y = model->MakeField<float>(std::format("{}_PCAwrt{}_Y", acronym, name_ref_vtx));
        PCAwrtXX_Z = model->MakeField<float>(std::format("{}_PCAwrt{}_Z", acronym, name_ref_vtx));
        PCAwrtXX_Px = model->MakeField<float>(std::format("{}_PCAwrt{}_Px", acronym, name_ref_vtx));
        PCAwrtXX_Py = model->MakeField<float>(std::format("{}_PCAwrt{}_Py", acronym, name_ref_vtx));
        PCAwrtXX_Pz = model->MakeField<float>(std::format("{}_PCAwrt{}_Pz", acronym, name_ref_vtx));
        if (is_mc) {
            McEntry = model->MakeField<unsigned int>(std::format("MC_{}_McEntry", acronym));
            PdgCode = model->MakeField<int>(std::format("MC_{}_PdgCode", acronym));
            Origin_X = model->MakeField<float>(std::format("MC_{}_Origin_X", acronym));
            Origin_Y = model->MakeField<float>(std::format("MC_{}_Origin_Y", acronym));
            Origin_Z = model->MakeField<float>(std::format("MC_{}_Origin_Z", acronym));
            MC_Px = model->MakeField<float>(std::format("MC_{}_Px", acronym));
            MC_Py = model->MakeField<float>(std::format("MC_{}_Py", acronym));
            MC_Pz = model->MakeField<float>(std::format("MC_{}_Pz", acronym));
            MC_Energy = model->MakeField<float>(std::format("MC_{}_Energy", acronym));
            IsTrue = model->MakeField<bool>(std::format("MC_{}_IsTrue", acronym));
            IsSecondary = model->MakeField<bool>(std::format("MC_{}_IsSecondary", acronym));
            IsSignal = model->MakeField<bool>(std::format("MC_{}_IsSignal", acronym));
            ReactionID = model->MakeField<int>(std::format("MC_{}_ReactionID", acronym));
            Mother_McEntry = model->MakeField<int>(std::format("MC_{}_Mother_McEntry", acronym));
            Mother_PdgCode = model->MakeField<int>(std::format("MC_{}_Mother_PdgCode", acronym));
            if (include_gm) {
                GM_McEntry = model->MakeField<int>(std::format("MC_{}_GM_McEntry", acronym));
                GM_PdgCode = model->MakeField<int>(std::format("MC_{}_GM_PdgCode", acronym));
            }
        }
    }

    // member variables //
    std::shared_ptr<unsigned int> EsdEntry;
    std::shared_ptr<float> X;
    std::shared_ptr<float> Y;
    std::shared_ptr<float> Z;
    std::shared_ptr<float> Px;
    std::shared_ptr<float> Py;
    std::shared_ptr<float> Pz;
    std::shared_ptr<float> PreDCAxy;
    std::shared_ptr<float> PreDCAz;
    std::shared_ptr<float> TPC_Signal;
    std::shared_ptr<float> NSigmaPion;
    std::shared_ptr<float> NSigmaKaon;
    std::shared_ptr<float> NSigmaProton;
    std::shared_ptr<float> PCAwrtXX_X;
    std::shared_ptr<float> PCAwrtXX_Y;
    std::shared_ptr<float> PCAwrtXX_Z;
    std::shared_ptr<float> PCAwrtXX_Px;
    std::shared_ptr<float> PCAwrtXX_Py;
    std::shared_ptr<float> PCAwrtXX_Pz;
    // mc info
    std::shared_ptr<unsigned int> McEntry;
    std::shared_ptr<int> PdgCode;
    std::shared_ptr<float> Origin_X;
    std::shared_ptr<float> Origin_Y;
    std::shared_ptr<float> Origin_Z;
    std::shared_ptr<float> MC_Px;
    std::shared_ptr<float> MC_Py;
    std::shared_ptr<float> MC_Pz;
    std::shared_ptr<float> MC_Energy;
    std::shared_ptr<bool> IsTrue;
    std::shared_ptr<bool> IsSecondary;
    std::shared_ptr<bool> IsSignal;
    std::shared_ptr<int> ReactionID;
    std::shared_ptr<int> Mother_McEntry;
    std::shared_ptr<int> Mother_PdgCode;
    std::shared_ptr<int> GM_McEntry;
    std::shared_ptr<int> GM_PdgCode;
};

}  // namespace Flat
