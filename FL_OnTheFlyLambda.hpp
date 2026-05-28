#pragma once

#include <memory>
#include <string_view>

#include <ROOT/RNTupleModel.hxx>

namespace Flat {

struct OnTheFlyLambda {

    OnTheFlyLambda() = default;
    OnTheFlyLambda(ROOT::RNTupleModel *model, bool is_mc, std::string_view prefix) { AddFieldsTo(model, is_mc, prefix); }

    void AddFieldsTo(ROOT::RNTupleModel *model, bool is_mc, std::string_view prefix) {
        // -- (anti)lambda info
        OnTheFlyEntry = model->MakeField<unsigned int>(std::format("{}_OnTheFlyEntry", prefix));
        Decay_X = model->MakeField<float>(std::format("{}_Decay_X", prefix));
        Decay_Y = model->MakeField<float>(std::format("{}_Decay_Y", prefix));
        Decay_Z = model->MakeField<float>(std::format("{}_Decay_Z", prefix));
        Px = model->MakeField<float>(std::format("{}_Px", prefix));
        Py = model->MakeField<float>(std::format("{}_Py", prefix));
        Pz = model->MakeField<float>(std::format("{}_Pz", prefix));
        Energy = model->MakeField<float>(std::format("{}_Energy", prefix));
        DcaV0Daughters = model->MakeField<float>(std::format("{}_DcaV0Daughters", prefix));
        if (is_mc) {
            McEntry = model->MakeField<int>(std::format("{}_McEntry", prefix));
            PdgCode = model->MakeField<int>(std::format("{}_PdgCode", prefix));
            MC_Px = model->MakeField<float>(std::format("{}_MC_Px", prefix));
            MC_Py = model->MakeField<float>(std::format("{}_MC_Py", prefix));
            MC_Pz = model->MakeField<float>(std::format("{}_MC_Pz", prefix));
            MC_Energy = model->MakeField<float>(std::format("{}_MC_Energy", prefix));
            Origin_X = model->MakeField<float>(std::format("{}_Origin_X", prefix));
            Origin_Y = model->MakeField<float>(std::format("{}_Origin_Y", prefix));
            Origin_Z = model->MakeField<float>(std::format("{}_Origin_Z", prefix));
            MC_Decay_X = model->MakeField<float>(std::format("{}_MC_Decay_X", prefix));
            MC_Decay_Y = model->MakeField<float>(std::format("{}_MC_Decay_Y", prefix));
            MC_Decay_Z = model->MakeField<float>(std::format("{}_MC_Decay_Z", prefix));
            IsTrue = model->MakeField<bool>(std::format("{}_IsTrue", prefix));
            IsSignal = model->MakeField<bool>(std::format("{}_IsSignal", prefix));
            IsHybrid = model->MakeField<bool>(std::format("{}_IsHybrid", prefix));
            InjectionID = model->MakeField<int>(std::format("{}_InjectionID", prefix));
        }
        // -- negative daughter
        Neg_EsdEntry = model->MakeField<unsigned int>(std::format("{}_Neg_EsdEntry", prefix));
        Neg_PCAwrtV0_Px = model->MakeField<float>(std::format("{}_Neg_PCAwrtV0_Px", prefix));
        Neg_PCAwrtV0_Py = model->MakeField<float>(std::format("{}_Neg_PCAwrtV0_Py", prefix));
        Neg_PCAwrtV0_Pz = model->MakeField<float>(std::format("{}_Neg_PCAwrtV0_Pz", prefix));
        Neg_PreDCAxy = model->MakeField<float>(std::format("{}_Neg_PreDCAxy", prefix));
        Neg_PreDCAz = model->MakeField<float>(std::format("{}_Neg_PreDCAz", prefix));
        Neg_NSigmaProton = model->MakeField<float>(std::format("{}_Neg_NSigmaProton", prefix));
        Neg_NSigmaKaon = model->MakeField<float>(std::format("{}_Neg_NSigmaKaon", prefix));
        Neg_NSigmaPion = model->MakeField<float>(std::format("{}_Neg_NSigmaPion", prefix));
        if (is_mc) {
            Neg_McEntry = model->MakeField<unsigned int>(std::format("{}_Neg_McEntry", prefix));
            Neg_PdgCode = model->MakeField<int>(std::format("{}_Neg_PdgCode", prefix));
            Neg_Origin_X = model->MakeField<float>(std::format("{}_Neg_Origin_X", prefix));
            Neg_Origin_Y = model->MakeField<float>(std::format("{}_Neg_Origin_Y", prefix));
            Neg_Origin_Z = model->MakeField<float>(std::format("{}_Neg_Origin_Z", prefix));
            Neg_MC_Px = model->MakeField<float>(std::format("{}_Neg_MC_Px", prefix));
            Neg_MC_Py = model->MakeField<float>(std::format("{}_Neg_MC_Py", prefix));
            Neg_MC_Pz = model->MakeField<float>(std::format("{}_Neg_MC_Pz", prefix));
            Neg_MC_Energy = model->MakeField<float>(std::format("{}_Neg_MC_Energy", prefix));
            Neg_IsTrue = model->MakeField<bool>(std::format("{}_Neg_IsTrue", prefix));
            Neg_IsSignal = model->MakeField<bool>(std::format("{}_Neg_IsSignal", prefix));
            Neg_InjectionID = model->MakeField<int>(std::format("{}_Neg_InjectionID", prefix));
            Neg_Mother_McEntry = model->MakeField<int>(std::format("{}_Neg_Mother_McEntry", prefix));
            Neg_Mother_PdgCode = model->MakeField<int>(std::format("{}_Neg_Mother_PdgCode", prefix));
        }
        // -- positive daughter
        Pos_EsdEntry = model->MakeField<unsigned int>(std::format("{}_Pos_EsdEntry", prefix));
        Pos_PCAwrtV0_Px = model->MakeField<float>(std::format("{}_Pos_PCAwrtV0_Px", prefix));
        Pos_PCAwrtV0_Py = model->MakeField<float>(std::format("{}_Pos_PCAwrtV0_Py", prefix));
        Pos_PCAwrtV0_Pz = model->MakeField<float>(std::format("{}_Pos_PCAwrtV0_Pz", prefix));
        Pos_PreDCAxy = model->MakeField<float>(std::format("{}_Pos_PreDCAxy", prefix));
        Pos_PreDCAz = model->MakeField<float>(std::format("{}_Pos_PreDCAz", prefix));
        Pos_NSigmaProton = model->MakeField<float>(std::format("{}_Pos_NSigmaProton", prefix));
        Pos_NSigmaKaon = model->MakeField<float>(std::format("{}_Pos_NSigmaKaon", prefix));
        Pos_NSigmaPion = model->MakeField<float>(std::format("{}_Pos_NSigmaPion", prefix));
        if (is_mc) {
            Pos_McEntry = model->MakeField<unsigned int>(std::format("{}_Pos_McEntry", prefix));
            Pos_PdgCode = model->MakeField<int>(std::format("{}_Pos_PdgCode", prefix));
            Pos_Origin_X = model->MakeField<float>(std::format("{}_Pos_Origin_X", prefix));
            Pos_Origin_Y = model->MakeField<float>(std::format("{}_Pos_Origin_Y", prefix));
            Pos_Origin_Z = model->MakeField<float>(std::format("{}_Pos_Origin_Z", prefix));
            Pos_MC_Px = model->MakeField<float>(std::format("{}_Pos_MC_Px", prefix));
            Pos_MC_Py = model->MakeField<float>(std::format("{}_Pos_MC_Py", prefix));
            Pos_MC_Pz = model->MakeField<float>(std::format("{}_Pos_MC_Pz", prefix));
            Pos_MC_Energy = model->MakeField<float>(std::format("{}_Pos_MC_Energy", prefix));
            Pos_IsTrue = model->MakeField<bool>(std::format("{}_Pos_IsTrue", prefix));
            Pos_IsSignal = model->MakeField<bool>(std::format("{}_Pos_IsSignal", prefix));
            Pos_InjectionID = model->MakeField<int>(std::format("{}_Pos_InjectionID", prefix));
            Pos_Mother_McEntry = model->MakeField<int>(std::format("{}_Pos_Mother_McEntry", prefix));
            Pos_Mother_PdgCode = model->MakeField<int>(std::format("{}_Pos_Mother_PdgCode", prefix));
        }
    }

    // member variables //
    // -- (anti)lambda info
    std::shared_ptr<unsigned int> OnTheFlyEntry;
    std::shared_ptr<float> Decay_X;
    std::shared_ptr<float> Decay_Y;
    std::shared_ptr<float> Decay_Z;
    std::shared_ptr<float> Px;
    std::shared_ptr<float> Py;
    std::shared_ptr<float> Pz;
    std::shared_ptr<float> Energy;  // (new! compared to vector model)
    std::shared_ptr<float> DcaV0Daughters;
    // -- linked mc
    std::shared_ptr<int> McEntry;  // (can be dummy)
    std::shared_ptr<int> PdgCode;
    std::shared_ptr<float> MC_Px;
    std::shared_ptr<float> MC_Py;
    std::shared_ptr<float> MC_Pz;
    std::shared_ptr<float> MC_Energy;
    std::shared_ptr<float> Origin_X;
    std::shared_ptr<float> Origin_Y;
    std::shared_ptr<float> Origin_Z;
    std::shared_ptr<float> MC_Decay_X;
    std::shared_ptr<float> MC_Decay_Y;
    std::shared_ptr<float> MC_Decay_Z;
    std::shared_ptr<bool> IsTrue;
    std::shared_ptr<bool> IsSignal;
    std::shared_ptr<bool> IsHybrid;
    std::shared_ptr<int> InjectionID;  // (can be dummy)
    // -- negative daughter
    std::shared_ptr<unsigned int> Neg_EsdEntry;
    std::shared_ptr<float> Neg_PCAwrtV0_Px;
    std::shared_ptr<float> Neg_PCAwrtV0_Py;
    std::shared_ptr<float> Neg_PCAwrtV0_Pz;
    std::shared_ptr<float> Neg_PreDCAxy;
    std::shared_ptr<float> Neg_PreDCAz;
    std::shared_ptr<float> Neg_NSigmaProton;
    std::shared_ptr<float> Neg_NSigmaKaon;
    std::shared_ptr<float> Neg_NSigmaPion;
    // -- linked mc negative daughter
    std::shared_ptr<unsigned int> Neg_McEntry;
    std::shared_ptr<int> Neg_PdgCode;
    std::shared_ptr<float> Neg_Origin_X;
    std::shared_ptr<float> Neg_Origin_Y;
    std::shared_ptr<float> Neg_Origin_Z;
    std::shared_ptr<float> Neg_MC_Px;
    std::shared_ptr<float> Neg_MC_Py;
    std::shared_ptr<float> Neg_MC_Pz;
    std::shared_ptr<float> Neg_MC_Energy;
    std::shared_ptr<bool> Neg_IsTrue;
    std::shared_ptr<bool> Neg_IsSignal;
    std::shared_ptr<int> Neg_InjectionID;     // (can be dummy)
    std::shared_ptr<int> Neg_Mother_McEntry;  // (can be dummy)
    std::shared_ptr<int> Neg_Mother_PdgCode;
    // -- positive daughter
    std::shared_ptr<unsigned int> Pos_EsdEntry;
    std::shared_ptr<float> Pos_PCAwrtV0_Px;
    std::shared_ptr<float> Pos_PCAwrtV0_Py;
    std::shared_ptr<float> Pos_PCAwrtV0_Pz;
    std::shared_ptr<float> Pos_PreDCAxy;
    std::shared_ptr<float> Pos_PreDCAz;
    std::shared_ptr<float> Pos_NSigmaProton;
    std::shared_ptr<float> Pos_NSigmaKaon;
    std::shared_ptr<float> Pos_NSigmaPion;
    // -- linked mc positive daughter
    std::shared_ptr<unsigned int> Pos_McEntry;
    std::shared_ptr<int> Pos_PdgCode;
    std::shared_ptr<float> Pos_Origin_X;
    std::shared_ptr<float> Pos_Origin_Y;
    std::shared_ptr<float> Pos_Origin_Z;
    std::shared_ptr<float> Pos_MC_Px;
    std::shared_ptr<float> Pos_MC_Py;
    std::shared_ptr<float> Pos_MC_Pz;
    std::shared_ptr<float> Pos_MC_Energy;
    std::shared_ptr<bool> Pos_IsTrue;
    std::shared_ptr<bool> Pos_IsSignal;
    std::shared_ptr<int> Pos_InjectionID;     // (can be dummy)
    std::shared_ptr<int> Pos_Mother_McEntry;  // (can be dummy)
    std::shared_ptr<int> Pos_Mother_PdgCode;
};

}  // namespace Flat
