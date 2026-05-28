#pragma once

#include <memory>
#include <vector>

#include <ROOT/RNTupleModel.hxx>

#include "Constants.hpp"
#include "VC_Track.hpp"

namespace Vector {

struct V0 {

    V0() = default;
    explicit V0(ROOT::RNTupleModel *model, bool is_mc, std::string_view prefix) { AddFieldsTo(model, is_mc, prefix); }

    void AddFieldsTo(ROOT::RNTupleModel *model, bool is_mc, std::string_view prefix) {
        Px = model->MakeField<std::vector<float>>(std::format("{}_Px", prefix));
        Py = model->MakeField<std::vector<float>>(std::format("{}_Py", prefix));
        Pz = model->MakeField<std::vector<float>>(std::format("{}_Pz", prefix));
        Energy = model->MakeField<std::vector<float>>(std::format("{}_Energy", prefix));
        Decay_X = model->MakeField<std::vector<float>>(std::format("{}_Decay_X", prefix));
        Decay_Y = model->MakeField<std::vector<float>>(std::format("{}_Decay_Y", prefix));
        Decay_Z = model->MakeField<std::vector<float>>(std::format("{}_Decay_Z", prefix));
        CovMatrix = model->MakeField<std::vector<std::array<float, Common::NCovMatrixComponents_State7>>>(std::format("{}_CovMatrix", prefix));
        Chi2NDF = model->MakeField<std::vector<float>>(std::format("{}_Chi2NDF", prefix));
        if (is_mc) {
            McEntry = model->MakeField<std::vector<int>>(std::format("MC_{}_McEntry", prefix));
            PdgCode = model->MakeField<std::vector<int>>(std::format("MC_{}_PdgCode", prefix));
            MC_Px = model->MakeField<std::vector<float>>(std::format("MC_{}_Px", prefix));
            MC_Py = model->MakeField<std::vector<float>>(std::format("MC_{}_Py", prefix));
            MC_Pz = model->MakeField<std::vector<float>>(std::format("MC_{}_Pz", prefix));
            MC_Energy = model->MakeField<std::vector<float>>(std::format("MC_{}_Energy", prefix));
            Origin_X = model->MakeField<std::vector<float>>(std::format("MC_{}_Origin_X", prefix));
            Origin_Y = model->MakeField<std::vector<float>>(std::format("MC_{}_Origin_Y", prefix));
            Origin_Z = model->MakeField<std::vector<float>>(std::format("MC_{}_Origin_Z", prefix));
            MC_Decay_X = model->MakeField<std::vector<float>>(std::format("MC_{}_Decay_X", prefix));
            MC_Decay_Y = model->MakeField<std::vector<float>>(std::format("MC_{}_Decay_Y", prefix));
            MC_Decay_Z = model->MakeField<std::vector<float>>(std::format("MC_{}_Decay_Z", prefix));
            IsTrue = model->MakeField<std::vector<char>>(std::format("MC_{}_IsTrue", prefix));
            IsSignal = model->MakeField<std::vector<char>>(std::format("MC_{}_IsSignal", prefix));
            IsSecondary = model->MakeField<std::vector<char>>(std::format("MC_{}_IsSecondary", prefix));
            IsHybrid = model->MakeField<std::vector<char>>(std::format("MC_{}_IsHybrid", prefix));
            ReactionID = model->MakeField<std::vector<int>>(std::format("MC_{}_ReactionID", prefix));
            // mother
            Mother_McEntry = model->MakeField<std::vector<int>>(std::format("MC_{}_Mother_McEntry", prefix));
            Mother_PdgCode = model->MakeField<std::vector<int>>(std::format("MC_{}_Mother_PdgCode", prefix));
        }
        // negative daughter
        Neg.AddFieldsTo(model, is_mc, is_mc, std::format("{}_Neg", prefix));
        Neg_PCAwrtV0_X = model->MakeField<std::vector<float>>(std::format("{}_Neg_PCAwrtV0_X", prefix));
        Neg_PCAwrtV0_Y = model->MakeField<std::vector<float>>(std::format("{}_Neg_PCAwrtV0_Y", prefix));
        Neg_PCAwrtV0_Z = model->MakeField<std::vector<float>>(std::format("{}_Neg_PCAwrtV0_Z", prefix));
        Neg_PCAwrtV0_Px = model->MakeField<std::vector<float>>(std::format("{}_Neg_PCAwrtV0_Px", prefix));
        Neg_PCAwrtV0_Py = model->MakeField<std::vector<float>>(std::format("{}_Neg_PCAwrtV0_Py", prefix));
        Neg_PCAwrtV0_Pz = model->MakeField<std::vector<float>>(std::format("{}_Neg_PCAwrtV0_Pz", prefix));
        // positive daughter
        Pos.AddFieldsTo(model, is_mc, is_mc, std::format("{}_Pos", prefix));
        Pos_PCAwrtV0_X = model->MakeField<std::vector<float>>(std::format("{}_Pos_PCAwrtV0_X", prefix));
        Pos_PCAwrtV0_Y = model->MakeField<std::vector<float>>(std::format("{}_Pos_PCAwrtV0_Y", prefix));
        Pos_PCAwrtV0_Z = model->MakeField<std::vector<float>>(std::format("{}_Pos_PCAwrtV0_Z", prefix));
        Pos_PCAwrtV0_Px = model->MakeField<std::vector<float>>(std::format("{}_Pos_PCAwrtV0_Px", prefix));
        Pos_PCAwrtV0_Py = model->MakeField<std::vector<float>>(std::format("{}_Pos_PCAwrtV0_Py", prefix));
        Pos_PCAwrtV0_Pz = model->MakeField<std::vector<float>>(std::format("{}_Pos_PCAwrtV0_Pz", prefix));
    }

    void Clear(bool is_mc) {
        Decay_X->clear();
        Decay_Y->clear();
        Decay_Z->clear();
        Px->clear();
        Py->clear();
        Pz->clear();
        Energy->clear();
        CovMatrix->clear();
        Chi2NDF->clear();
        // linked mc
        if (is_mc) {
            McEntry->clear();
            PdgCode->clear();
            MC_Px->clear();
            MC_Py->clear();
            MC_Pz->clear();
            MC_Energy->clear();
            Origin_X->clear();
            Origin_Y->clear();
            Origin_Z->clear();
            MC_Decay_X->clear();
            MC_Decay_Y->clear();
            MC_Decay_Z->clear();
            IsTrue->clear();
            IsSignal->clear();
            IsSecondary->clear();
            IsHybrid->clear();
            ReactionID->clear();
            // mother
            Mother_McEntry->clear();
            Mother_PdgCode->clear();
        }
        // negative daughter
        Neg.Clear(is_mc, is_mc);
        Neg_PCAwrtV0_X->clear();
        Neg_PCAwrtV0_Y->clear();
        Neg_PCAwrtV0_Z->clear();
        Neg_PCAwrtV0_Px->clear();
        Neg_PCAwrtV0_Py->clear();
        Neg_PCAwrtV0_Pz->clear();
        // positive daughter
        Pos.Clear(is_mc, is_mc);
        Pos_PCAwrtV0_X->clear();
        Pos_PCAwrtV0_Y->clear();
        Pos_PCAwrtV0_Z->clear();
        Pos_PCAwrtV0_Px->clear();
        Pos_PCAwrtV0_Py->clear();
        Pos_PCAwrtV0_Pz->clear();
    }

    // member variables //
    std::shared_ptr<std::vector<float>> Decay_X;
    std::shared_ptr<std::vector<float>> Decay_Y;
    std::shared_ptr<std::vector<float>> Decay_Z;
    std::shared_ptr<std::vector<float>> Px;
    std::shared_ptr<std::vector<float>> Py;
    std::shared_ptr<std::vector<float>> Pz;
    std::shared_ptr<std::vector<float>> Energy;
    std::shared_ptr<std::vector<std::array<float, Common::NCovMatrixComponents_State7>>> CovMatrix;
    std::shared_ptr<std::vector<float>> Chi2NDF;
    // -- linked mc
    std::shared_ptr<std::vector<int>> McEntry;
    std::shared_ptr<std::vector<int>> PdgCode;
    std::shared_ptr<std::vector<float>> MC_Px;
    std::shared_ptr<std::vector<float>> MC_Py;
    std::shared_ptr<std::vector<float>> MC_Pz;
    std::shared_ptr<std::vector<float>> MC_Energy;
    std::shared_ptr<std::vector<float>> Origin_X;
    std::shared_ptr<std::vector<float>> Origin_Y;
    std::shared_ptr<std::vector<float>> Origin_Z;
    std::shared_ptr<std::vector<float>> MC_Decay_X;
    std::shared_ptr<std::vector<float>> MC_Decay_Y;
    std::shared_ptr<std::vector<float>> MC_Decay_Z;
    std::shared_ptr<std::vector<char>> IsTrue;
    std::shared_ptr<std::vector<char>> IsSignal;
    std::shared_ptr<std::vector<char>> IsSecondary;
    std::shared_ptr<std::vector<char>> IsHybrid;
    std::shared_ptr<std::vector<int>> ReactionID;
    // -- mother
    std::shared_ptr<std::vector<int>> Mother_McEntry;
    std::shared_ptr<std::vector<int>> Mother_PdgCode;
    // -- negative daughter
    Vector::Track Neg;
    std::shared_ptr<std::vector<float>> Neg_PCAwrtV0_X;
    std::shared_ptr<std::vector<float>> Neg_PCAwrtV0_Y;
    std::shared_ptr<std::vector<float>> Neg_PCAwrtV0_Z;
    std::shared_ptr<std::vector<float>> Neg_PCAwrtV0_Px;
    std::shared_ptr<std::vector<float>> Neg_PCAwrtV0_Py;
    std::shared_ptr<std::vector<float>> Neg_PCAwrtV0_Pz;
    // -- positive daughter
    Vector::Track Pos;
    std::shared_ptr<std::vector<float>> Pos_PCAwrtV0_X;
    std::shared_ptr<std::vector<float>> Pos_PCAwrtV0_Y;
    std::shared_ptr<std::vector<float>> Pos_PCAwrtV0_Z;
    std::shared_ptr<std::vector<float>> Pos_PCAwrtV0_Px;
    std::shared_ptr<std::vector<float>> Pos_PCAwrtV0_Py;
    std::shared_ptr<std::vector<float>> Pos_PCAwrtV0_Pz;
};

}  // namespace Vector
