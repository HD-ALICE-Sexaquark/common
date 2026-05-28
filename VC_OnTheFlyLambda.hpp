#pragma once

#include <memory>
#include <vector>

#include <ROOT/RNTupleModel.hxx>

namespace Vector {

struct OnTheFlyLambda {

    OnTheFlyLambda() = default;
    OnTheFlyLambda(ROOT::RNTupleModel *model, bool is_mc) { AddFieldsTo(model, is_mc); }

    void AddFieldsTo(ROOT::RNTupleModel *model, bool is_mc) {
        // -- (anti)lambda info
        OnTheFlyEntry = model->MakeField<std::vector<unsigned int>>("Lambda_OnTheFlyEntry");
        Decay_X = model->MakeField<std::vector<float>>("Lambda_Decay_X");
        Decay_Y = model->MakeField<std::vector<float>>("Lambda_Decay_Y");
        Decay_Z = model->MakeField<std::vector<float>>("Lambda_Decay_Z");
        Px = model->MakeField<std::vector<float>>("Lambda_Px");
        Py = model->MakeField<std::vector<float>>("Lambda_Py");
        Pz = model->MakeField<std::vector<float>>("Lambda_Pz");
        DcaV0Daughters = model->MakeField<std::vector<float>>("Lambda_DcaV0Daughters");
        // -- negative daughter
        Neg_EsdEntry = model->MakeField<std::vector<unsigned int>>("Lambda_Neg_EsdEntry");
        Neg_PCAwrtV0_Px = model->MakeField<std::vector<float>>("Lambda_Neg_PCAwrtV0_Px");
        Neg_PCAwrtV0_Py = model->MakeField<std::vector<float>>("Lambda_Neg_PCAwrtV0_Py");
        Neg_PCAwrtV0_Pz = model->MakeField<std::vector<float>>("Lambda_Neg_PCAwrtV0_Pz");
        Neg_PreDCAxy = model->MakeField<std::vector<float>>("Lambda_Neg_PreDCAxy");
        Neg_PreDCAz = model->MakeField<std::vector<float>>("Lambda_Neg_PreDCAz");
        Neg_NSigmaProton = model->MakeField<std::vector<float>>("Lambda_Neg_NSigmaProton");
        Neg_NSigmaKaon = model->MakeField<std::vector<float>>("Lambda_Neg_NSigmaKaon");
        Neg_NSigmaPion = model->MakeField<std::vector<float>>("Lambda_Neg_NSigmaPion");
        // -- positive daughter
        Pos_EsdEntry = model->MakeField<std::vector<unsigned int>>("Lambda_Pos_EsdEntry");
        Pos_PCAwrtV0_Px = model->MakeField<std::vector<float>>("Lambda_Pos_PCAwrtV0_Px");
        Pos_PCAwrtV0_Py = model->MakeField<std::vector<float>>("Lambda_Pos_PCAwrtV0_Py");
        Pos_PCAwrtV0_Pz = model->MakeField<std::vector<float>>("Lambda_Pos_PCAwrtV0_Pz");
        Pos_PreDCAxy = model->MakeField<std::vector<float>>("Lambda_Pos_PreDCAxy");
        Pos_PreDCAz = model->MakeField<std::vector<float>>("Lambda_Pos_PreDCAz");
        Pos_NSigmaProton = model->MakeField<std::vector<float>>("Lambda_Pos_NSigmaProton");
        Pos_NSigmaKaon = model->MakeField<std::vector<float>>("Lambda_Pos_NSigmaKaon");
        Pos_NSigmaPion = model->MakeField<std::vector<float>>("Lambda_Pos_NSigmaPion");
        // --linked mc
        if (!is_mc) return;
        Neg_McEntry = model->MakeField<std::vector<unsigned int>>("Lambda_Neg_McEntry");
        Pos_McEntry = model->MakeField<std::vector<unsigned int>>("Lambda_Pos_McEntry");
    }

    // member variables //
    // -- (anti)lambda info
    std::shared_ptr<std::vector<unsigned int>> OnTheFlyEntry;
    std::shared_ptr<std::vector<float>> Decay_X;
    std::shared_ptr<std::vector<float>> Decay_Y;
    std::shared_ptr<std::vector<float>> Decay_Z;
    std::shared_ptr<std::vector<float>> Px;
    std::shared_ptr<std::vector<float>> Py;
    std::shared_ptr<std::vector<float>> Pz;
    std::shared_ptr<std::vector<float>> DcaV0Daughters;
    // -- negative daughter
    std::shared_ptr<std::vector<unsigned int>> Neg_EsdEntry;
    std::shared_ptr<std::vector<float>> Neg_PCAwrtV0_Px;
    std::shared_ptr<std::vector<float>> Neg_PCAwrtV0_Py;
    std::shared_ptr<std::vector<float>> Neg_PCAwrtV0_Pz;
    std::shared_ptr<std::vector<float>> Neg_PreDCAxy;
    std::shared_ptr<std::vector<float>> Neg_PreDCAz;
    std::shared_ptr<std::vector<float>> Neg_NSigmaProton;
    std::shared_ptr<std::vector<float>> Neg_NSigmaKaon;
    std::shared_ptr<std::vector<float>> Neg_NSigmaPion;
    // -- positive daughter
    std::shared_ptr<std::vector<unsigned int>> Pos_EsdEntry;
    std::shared_ptr<std::vector<float>> Pos_PCAwrtV0_Px;
    std::shared_ptr<std::vector<float>> Pos_PCAwrtV0_Py;
    std::shared_ptr<std::vector<float>> Pos_PCAwrtV0_Pz;
    std::shared_ptr<std::vector<float>> Pos_PreDCAxy;
    std::shared_ptr<std::vector<float>> Pos_PreDCAz;
    std::shared_ptr<std::vector<float>> Pos_NSigmaProton;
    std::shared_ptr<std::vector<float>> Pos_NSigmaKaon;
    std::shared_ptr<std::vector<float>> Pos_NSigmaPion;
    // -- linked mc
    std::shared_ptr<std::vector<unsigned int>> Neg_McEntry;  // (mc only)
    std::shared_ptr<std::vector<unsigned int>> Pos_McEntry;  // (mc only)
};

}  // namespace Vector
