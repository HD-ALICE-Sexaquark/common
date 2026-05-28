#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include <ROOT/RNTupleModel.hxx>

#include "Constants.hpp"

namespace Vector {

struct Track {

    Track() = default;
    Track(ROOT::RNTupleModel *model, bool is_mc, bool include_linked_mc, std::string_view prefix) {
        AddFieldsTo(model, is_mc, include_linked_mc, prefix);
    }

    void AddFieldsTo(ROOT::RNTupleModel *model, bool is_mc, bool include_linked_mc, std::string_view prefix) {
        EsdEntry = model->MakeField<std::vector<unsigned int>>(std::format("{}_EsdEntry", prefix));
        X = model->MakeField<std::vector<float>>(std::format("{}_X", prefix));
        Y = model->MakeField<std::vector<float>>(std::format("{}_Y", prefix));
        Z = model->MakeField<std::vector<float>>(std::format("{}_Z", prefix));
        Px = model->MakeField<std::vector<float>>(std::format("{}_Px", prefix));
        Py = model->MakeField<std::vector<float>>(std::format("{}_Py", prefix));
        Pz = model->MakeField<std::vector<float>>(std::format("{}_Pz", prefix));
        Charge = model->MakeField<std::vector<int>>(std::format("{}_Charge", prefix));
        PreDCAxy = model->MakeField<std::vector<float>>(std::format("{}_PreDCAxy", prefix));
        PreDCAz = model->MakeField<std::vector<float>>(std::format("{}_PreDCAz", prefix));
        TPC_Signal = model->MakeField<std::vector<float>>(std::format("{}_TPC_Signal", prefix));
        ITS_FirstLayer = model->MakeField<std::vector<int>>(std::format("{}_ITS_FirstLayer", prefix));
        NSigmaPion = model->MakeField<std::vector<float>>(std::format("{}_NSigmaPion", prefix));
        NSigmaKaon = model->MakeField<std::vector<float>>(std::format("{}_NSigmaKaon", prefix));
        NSigmaProton = model->MakeField<std::vector<float>>(std::format("{}_NSigmaProton", prefix));
        CovMatrix = model->MakeField<std::vector<std::array<float, Common::NCovMatrixComponents_State6>>>(std::format("{}_CovMatrix", prefix));
#if E2R_TPC_EXTRA
        TPC_DCAxy = model->MakeField<std::vector<float>>(std::format("{}_TPC_DCAxy", prefix));
        TPC_DCAz = model->MakeField<std::vector<float>>(std::format("{}_TPC_DCAz", prefix));
        TPC_NCrossedRows = model->MakeField<std::vector<float>>(std::format("{}_TPC_NCrossedRows", prefix));
        TPC_NClusters = model->MakeField<std::vector<unsigned int>>(std::format("{}_TPC_NClusters", prefix));
        TPC_NClustersLC = model->MakeField<std::vector<unsigned int>>(std::format("{}_TPC_NClustersLC", prefix));
        TPC_NClustersFound = model->MakeField<std::vector<unsigned int>>(std::format("{}_TPC_NClustersFound", prefix));
        TPC_NClustersShared = model->MakeField<std::vector<unsigned int>>(std::format("{}_TPC_NClustersShared", prefix));
        TPC_Chi2 = model->MakeField<std::vector<float>>(std::format("{}_TPC_Chi2", prefix));
        TPC_Chi2Constrained = model->MakeField<std::vector<float>>(std::format("{}_TPC_Chi2Constrained", prefix));
        TPC_Chi2TCVG = model->MakeField<std::vector<float>>(std::format("{}_TPC_Chi2TCVG", prefix));
        if (is_mc) TPC_SignalTunedOnData = model->MakeField<std::vector<float>>(std::format("{}_TPC_SignalTunedOnData", prefix));
        TPC_SignalSigma = model->MakeField<std::vector<float>>(std::format("{}_TPC_SignalSigma", prefix));
        TPC_SignalCorrected = model->MakeField<std::vector<float>>(std::format("{}_TPC_SignalCorrected", prefix));
        TPC_ESignalPion = model->MakeField<std::vector<float>>(std::format("{}_TPC_ESignalPion", prefix));
        TPC_ESigmaPion = model->MakeField<std::vector<float>>(std::format("{}_TPC_ESigmaPion", prefix));
        TPC_ESignalKaon = model->MakeField<std::vector<float>>(std::format("{}_TPC_ESignalKaon", prefix));
        TPC_ESigmaKaon = model->MakeField<std::vector<float>>(std::format("{}_TPC_ESigmaKaon", prefix));
        TPC_ESignalProton = model->MakeField<std::vector<float>>(std::format("{}_TPC_ESignalProton", prefix));
        TPC_ESigmaProton = model->MakeField<std::vector<float>>(std::format("{}_TPC_ESigmaProton", prefix));
        TPC_SignalN = model->MakeField<std::vector<unsigned int>>(std::format("{}_TPC_SignalN", prefix));
        TPC_PointsFirst = model->MakeField<std::vector<float>>(std::format("{}_TPC_PointsFirst", prefix));
        TPC_PointsIndexMax = model->MakeField<std::vector<float>>(std::format("{}_TPC_PointsIndexMax", prefix));
        TPC_PointsLast = model->MakeField<std::vector<float>>(std::format("{}_TPC_PointsLast", prefix));
        TPC_PointsMaxDens = model->MakeField<std::vector<float>>(std::format("{}_TPC_PointsMaxDens", prefix));
        TPC_FirstRow = model->MakeField<std::vector<int>>(std::format("{}_TPC_FirstRow", prefix));
#endif
        if (is_mc) McEntry = model->MakeField<std::vector<unsigned int>>(std::format("{}_McEntry", prefix));
        if (include_linked_mc) {
            PdgCode = model->MakeField<std::vector<int>>(std::format("MC_{}_PdgCode", prefix));
            Origin_X = model->MakeField<std::vector<float>>(std::format("MC_{}_Origin_X", prefix));
            Origin_Y = model->MakeField<std::vector<float>>(std::format("MC_{}_Origin_Y", prefix));
            Origin_Z = model->MakeField<std::vector<float>>(std::format("MC_{}_Origin_Z", prefix));
            MC_Px = model->MakeField<std::vector<float>>(std::format("MC_{}_Px", prefix));
            MC_Py = model->MakeField<std::vector<float>>(std::format("MC_{}_Py", prefix));
            MC_Pz = model->MakeField<std::vector<float>>(std::format("MC_{}_Pz", prefix));
            MC_Energy = model->MakeField<std::vector<float>>(std::format("MC_{}_Energy", prefix));
            IsTrue = model->MakeField<std::vector<char>>(std::format("MC_{}_IsTrue", prefix));
            IsSecondary = model->MakeField<std::vector<char>>(std::format("MC_{}_IsSecondary", prefix));
            IsSignal = model->MakeField<std::vector<char>>(std::format("MC_{}_IsSignal", prefix));
            ReactionID = model->MakeField<std::vector<int>>(std::format("MC_{}_ReactionID", prefix));
            // -- mother
            Mother_McEntry = model->MakeField<std::vector<int>>(std::format("MC_{}_Mother_McEntry", prefix));
            Mother_PdgCode = model->MakeField<std::vector<int>>(std::format("MC_{}_Mother_PdgCode", prefix));
            // -- grandmother
            GM_McEntry = model->MakeField<std::vector<int>>(std::format("MC_{}_GM_McEntry", prefix));
            GM_PdgCode = model->MakeField<std::vector<int>>(std::format("MC_{}_GM_PdgCode", prefix));
        }
    }

    void Clear(bool is_mc, bool include_linked_mc) {
        EsdEntry->clear();
        X->clear();
        Y->clear();
        Z->clear();
        Px->clear();
        Py->clear();
        Pz->clear();
        Charge->clear();
        PreDCAxy->clear();
        PreDCAz->clear();
        TPC_Signal->clear();
        NSigmaPion->clear();
        NSigmaKaon->clear();
        NSigmaProton->clear();
        CovMatrix->clear();
#if E2R_TPC_EXTRA
        TPC_DCAxy->clear();
        TPC_DCAz->clear();
        TPC_NCrossedRows->clear();
        TPC_NClusters->clear();
        TPC_NClustersLC->clear();
        TPC_NClustersFound->clear();
        TPC_NClustersShared->clear();
        TPC_Chi2->clear();
        TPC_Chi2Constrained->clear();
        TPC_Chi2TCVG->clear();
        if (is_mc) TPC_SignalTunedOnData->clear();
        TPC_SignalSigma->clear();
        TPC_SignalCorrected->clear();
        TPC_ESignalPion->clear();
        TPC_ESigmaPion->clear();
        TPC_ESignalKaon->clear();
        TPC_ESigmaKaon->clear();
        TPC_ESignalProton->clear();
        TPC_ESigmaProton->clear();
        TPC_SignalN->clear();
        TPC_PointsFirst->clear();
        TPC_PointsIndexMax->clear();
        TPC_PointsLast->clear();
        TPC_PointsMaxDens->clear();
        TPC_FirstRow->clear();
#endif
        if (is_mc) McEntry->clear();
        if (include_linked_mc) {
            PdgCode->clear();
            Origin_X->clear();
            Origin_Y->clear();
            Origin_Z->clear();
            MC_Px->clear();
            MC_Py->clear();
            MC_Pz->clear();
            MC_Energy->clear();
            IsTrue->clear();
            IsSecondary->clear();
            IsSignal->clear();
            ReactionID->clear();
            // -- mother
            Mother_McEntry->clear();
            Mother_PdgCode->clear();
            // -- grandmother
            GM_McEntry->clear();
            GM_PdgCode->clear();
        }
    }

    // member variables //
    // -- track info
    std::shared_ptr<std::vector<unsigned int>> EsdEntry;
    std::shared_ptr<std::vector<float>> X;   // from inner parametrization
    std::shared_ptr<std::vector<float>> Y;   // from inner parametrization
    std::shared_ptr<std::vector<float>> Z;   // from inner parametrization
    std::shared_ptr<std::vector<float>> Px;  // from inner parametrization
    std::shared_ptr<std::vector<float>> Py;  // from inner parametrization
    std::shared_ptr<std::vector<float>> Pz;  // from inner parametrization
    std::shared_ptr<std::vector<int>> Charge;
    std::shared_ptr<std::vector<float>> PreDCAxy;  // pre-calculated DCA wrt PV
    std::shared_ptr<std::vector<float>> PreDCAz;   // pre-calculated DCA wrt PV
    std::shared_ptr<std::vector<float>> TPC_Signal;
    std::shared_ptr<std::vector<int>> ITS_FirstLayer;  // -1 if no ITS hit
    std::shared_ptr<std::vector<float>> NSigmaPion;
    std::shared_ptr<std::vector<float>> NSigmaKaon;
    std::shared_ptr<std::vector<float>> NSigmaProton;
    std::shared_ptr<std::vector<std::array<float, Common::NCovMatrixComponents_State6>>> CovMatrix;
#if E2R_TPC_EXTRA
    std::shared_ptr<std::vector<float>> TPC_DCAxy;
    std::shared_ptr<std::vector<float>> TPC_DCAz;
    std::shared_ptr<std::vector<float>> TPC_NCrossedRows;
    std::shared_ptr<std::vector<unsigned int>> TPC_NClusters;
    std::shared_ptr<std::vector<unsigned int>> TPC_NClustersLC;  // from lowercase method `GetTPCncls`
    std::shared_ptr<std::vector<unsigned int>> TPC_NClustersFound;
    std::shared_ptr<std::vector<unsigned int>> TPC_NClustersShared;
    std::shared_ptr<std::vector<float>> TPC_Chi2;
    std::shared_ptr<std::vector<float>> TPC_Chi2Constrained;    // TPC-only chi2 at the primary vertex
    std::shared_ptr<std::vector<float>> TPC_Chi2TCVG;           // chi2 TPC constrained vs global
    std::shared_ptr<std::vector<float>> TPC_SignalTunedOnData;  // (mc only)
    std::shared_ptr<std::vector<float>> TPC_SignalSigma;
    std::shared_ptr<std::vector<float>> TPC_SignalCorrected;
    std::shared_ptr<std::vector<float>> TPC_ESignalPion;  // `E` stands for `Expected`
    std::shared_ptr<std::vector<float>> TPC_ESigmaPion;
    std::shared_ptr<std::vector<float>> TPC_ESignalKaon;
    std::shared_ptr<std::vector<float>> TPC_ESigmaKaon;
    std::shared_ptr<std::vector<float>> TPC_ESignalProton;
    std::shared_ptr<std::vector<float>> TPC_ESigmaProton;
    std::shared_ptr<std::vector<unsigned int>> TPC_SignalN;
    std::shared_ptr<std::vector<float>> TPC_PointsFirst;
    std::shared_ptr<std::vector<float>> TPC_PointsIndexMax;
    std::shared_ptr<std::vector<float>> TPC_PointsLast;
    std::shared_ptr<std::vector<float>> TPC_PointsMaxDens;
    std::shared_ptr<std::vector<int>> TPC_FirstRow;
#endif
    // -- linked mc
    std::shared_ptr<std::vector<unsigned int>> McEntry;
    std::shared_ptr<std::vector<int>> PdgCode;
    std::shared_ptr<std::vector<float>> Origin_X;
    std::shared_ptr<std::vector<float>> Origin_Y;
    std::shared_ptr<std::vector<float>> Origin_Z;
    std::shared_ptr<std::vector<float>> MC_Px;
    std::shared_ptr<std::vector<float>> MC_Py;
    std::shared_ptr<std::vector<float>> MC_Pz;
    std::shared_ptr<std::vector<float>> MC_Energy;
    std::shared_ptr<std::vector<char>> IsTrue;       // (yes, `vector<char>` instead of `vector<bool>`)
    std::shared_ptr<std::vector<char>> IsSecondary;  // (yes, `vector<char>` instead of `vector<bool>`)
    std::shared_ptr<std::vector<char>> IsSignal;     // (yes, `vector<char>` instead of `vector<bool>`)
    std::shared_ptr<std::vector<int>> ReactionID;
    // -- mc mother
    std::shared_ptr<std::vector<int>> Mother_McEntry;  // (can be dummy)
    std::shared_ptr<std::vector<int>> Mother_PdgCode;
    // -- mc grandmother
    std::shared_ptr<std::vector<int>> GM_McEntry;  // (can be dummy)
    std::shared_ptr<std::vector<int>> GM_PdgCode;
};

}  // namespace Vector
