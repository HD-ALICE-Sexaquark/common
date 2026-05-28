#pragma once

#include <memory>
#include <vector>

#include <ROOT/RNTupleModel.hxx>

#include "Constants.hpp"

namespace Vector {

struct McParticle {

    McParticle() = default;
    explicit McParticle(ROOT::RNTupleModel *model) { AddFieldsTo(model); }

    void AddFieldsTo(ROOT::RNTupleModel *model) {
        PdgCode = model->MakeField<std::vector<int>>("MC_PdgCode");
        Charge = model->MakeField<std::vector<char>>("MC_Charge");
        Mother_McEntry = model->MakeField<std::vector<int>>("MC_Mother_McEntry");
        N_Daughters = model->MakeField<std::vector<unsigned int>>("MC_N_Daughters");
        FirstDau_McEntry = model->MakeField<std::vector<int>>("MC_FirstDau_McEntry");
        LastDau_McEntry = model->MakeField<std::vector<int>>("MC_LastDau_McEntry");
        Origin_X = model->MakeField<std::vector<float>>("MC_Origin_X");
        Origin_Y = model->MakeField<std::vector<float>>("MC_Origin_Y");
        Origin_Z = model->MakeField<std::vector<float>>("MC_Origin_Z");
        Px = model->MakeField<std::vector<float>>("MC_Px");
        Py = model->MakeField<std::vector<float>>("MC_Py");
        Pz = model->MakeField<std::vector<float>>("MC_Pz");
        Energy = model->MakeField<std::vector<float>>("MC_Energy");
        Status = model->MakeField<std::vector<unsigned int>>("MC_Status");
        Generator = model->MakeField<std::vector<char>>("MC_Generator");
        IsPhysPrimary = model->MakeField<std::vector<char>>("MC_IsPhysPrimary");
        IsSecFromMat = model->MakeField<std::vector<char>>("MC_IsSecFromMat");
        IsSecFromWeak = model->MakeField<std::vector<char>>("MC_IsSecFromWeak");
#if E2R_MC_EXTRA
        IsOOBPileup = model->MakeField<std::vector<char>>("MC_IsOOBPileup");
#endif
    }

    void Clear() {
        PdgCode->clear();
        Charge->clear();
        Mother_McEntry->clear();
        N_Daughters->clear();
        FirstDau_McEntry->clear();
        LastDau_McEntry->clear();
        Origin_X->clear();
        Origin_Y->clear();
        Origin_Z->clear();
        Px->clear();
        Py->clear();
        Pz->clear();
        Energy->clear();
        Status->clear();
        Generator->clear();
        IsPhysPrimary->clear();
        IsSecFromMat->clear();
        IsSecFromWeak->clear();
#if E2R_MC_EXTRA
        IsOOBPileup->clear();
#endif
    }

    // member variables //
    std::shared_ptr<std::vector<int>> PdgCode;
    std::shared_ptr<std::vector<char>> Charge;
    std::shared_ptr<std::vector<int>> Mother_McEntry;
    std::shared_ptr<std::vector<unsigned int>> N_Daughters;
    std::shared_ptr<std::vector<int>> FirstDau_McEntry;
    std::shared_ptr<std::vector<int>> LastDau_McEntry;
    std::shared_ptr<std::vector<float>> Origin_X;
    std::shared_ptr<std::vector<float>> Origin_Y;
    std::shared_ptr<std::vector<float>> Origin_Z;
    std::shared_ptr<std::vector<float>> Px;
    std::shared_ptr<std::vector<float>> Py;
    std::shared_ptr<std::vector<float>> Pz;
    std::shared_ptr<std::vector<float>> Energy;
    std::shared_ptr<std::vector<unsigned int>> Status;
    std::shared_ptr<std::vector<char>> Generator;      // 0: HIJING, 1: anti-neutron injector, 2: anti-sexaquark reaction
    std::shared_ptr<std::vector<char>> IsPhysPrimary;  // is physical primary according to the ALICE definition
    std::shared_ptr<std::vector<char>> IsSecFromMat;
    std::shared_ptr<std::vector<char>> IsSecFromWeak;
#if E2R_MC_EXTRA
    std::shared_ptr<std::vector<char>> IsOOBPileup;
#endif
};

}  // namespace Vector
