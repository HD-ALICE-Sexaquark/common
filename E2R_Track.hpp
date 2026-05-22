#pragma once

#include <memory>
#include <vector>

#include <ROOT/RNTupleModel.hxx>

#include "Constants.hpp"

namespace E2R {

struct Track {
    void AddToModel(ROOT::RNTupleModel *model, bool is_mc) {
        EsdEntry = model->MakeField<std::vector<unsigned int>>("Track_EsdEntry");
        X = model->MakeField<std::vector<float>>("Track_X");
        Y = model->MakeField<std::vector<float>>("Track_Y");
        Z = model->MakeField<std::vector<float>>("Track_Z");
        Px = model->MakeField<std::vector<float>>("Track_Px");
        Py = model->MakeField<std::vector<float>>("Track_Py");
        Pz = model->MakeField<std::vector<float>>("Track_Pz");
        Charge = model->MakeField<std::vector<char>>("Track_Charge");
        PreDCAxy = model->MakeField<std::vector<float>>("Track_PreDCAxy");
        PreDCAz = model->MakeField<std::vector<float>>("Track_PreDCAz");
        TPC_Signal = model->MakeField<std::vector<float>>("Track_TPC_Signal");
        ITS_FirstLayer = model->MakeField<std::vector<int>>("Track_ITS_FirstLayer");
        NSigmaPion = model->MakeField<std::vector<float>>("Track_NSigmaPion");
        NSigmaKaon = model->MakeField<std::vector<float>>("Track_NSigmaKaon");
        NSigmaProton = model->MakeField<std::vector<float>>("Track_NSigmaProton");
        CovMatrix = model->MakeField<std::vector<std::array<float, 21>>>("Track_CovMatrix");
#if E2R_TPC_EXTRA
        TPC_DCAxy = model->MakeField<std::vector<float>>("Track_TPC_DCAxy");
        TPC_DCAz = model->MakeField<std::vector<float>>("Track_TPC_DCAz");
        TPC_NCrossedRows = model->MakeField<std::vector<float>>("Track_TPC_NCrossedRows");
        TPC_NClusters = model->MakeField<std::vector<unsigned int>>("Track_TPC_NClusters");
        TPC_NClustersLC = model->MakeField<std::vector<unsigned int>>("Track_TPC_NClustersLC");
        TPC_NClustersFound = model->MakeField<std::vector<unsigned int>>("Track_TPC_NClustersFound");
        TPC_NClustersShared = model->MakeField<std::vector<unsigned int>>("Track_TPC_NClustersShared");
        TPC_Chi2 = model->MakeField<std::vector<float>>("Track_TPC_Chi2");
        TPC_Chi2Constrained = model->MakeField<std::vector<float>>("Track_TPC_Chi2Constrained");
        TPC_Chi2TCVG = model->MakeField<std::vector<float>>("Track_TPC_Chi2TCVG");
        if (is_mc) TPC_SignalTunedOnData = model->MakeField<std::vector<float>>("Track_TPC_SignalTunedOnData");
        TPC_SignalSigma = model->MakeField<std::vector<float>>("Track_TPC_SignalSigma");
        TPC_SignalCorrected = model->MakeField<std::vector<float>>("Track_TPC_SignalCorrected");
        TPC_ESignalPion = model->MakeField<std::vector<float>>("Track_TPC_ESignalPion");
        TPC_ESigmaPion = model->MakeField<std::vector<float>>("Track_TPC_ESigmaPion");
        TPC_ESignalKaon = model->MakeField<std::vector<float>>("Track_TPC_ESignalKaon");
        TPC_ESigmaKaon = model->MakeField<std::vector<float>>("Track_TPC_ESigmaKaon");
        TPC_ESignalProton = model->MakeField<std::vector<float>>("Track_TPC_ESignalProton");
        TPC_ESigmaProton = model->MakeField<std::vector<float>>("Track_TPC_ESigmaProton");
        TPC_SignalN = model->MakeField<std::vector<unsigned int>>("Track_TPC_SignalN");
        TPC_PointsFirst = model->MakeField<std::vector<float>>("Track_TPC_PointsFirst");
        TPC_PointsIndexMax = model->MakeField<std::vector<float>>("Track_TPC_PointsIndexMax");
        TPC_PointsLast = model->MakeField<std::vector<float>>("Track_TPC_PointsLast");
        TPC_PointsMaxDens = model->MakeField<std::vector<float>>("Track_TPC_PointsMaxDens");
        TPC_FirstRow = model->MakeField<std::vector<int>>("Track_TPC_FirstRow");
#endif
        if (is_mc) McEntry = model->MakeField<std::vector<int>>("Track_McEntry");
    }

    // member variables //
    std::shared_ptr<std::vector<unsigned int>> EsdEntry;
    std::shared_ptr<std::vector<float>> X;                          // from inner parametrization
    std::shared_ptr<std::vector<float>> Y;                          // from inner parametrization
    std::shared_ptr<std::vector<float>> Z;                          // from inner parametrization
    std::shared_ptr<std::vector<float>> Px;                         // from inner parametrization
    std::shared_ptr<std::vector<float>> Py;                         // from inner parametrization
    std::shared_ptr<std::vector<float>> Pz;                         // from inner parametrization
    std::shared_ptr<std::vector<char>> Charge;                      //
    std::shared_ptr<std::vector<float>> PreDCAxy;                   // pre-calculated DCA wrt PV
    std::shared_ptr<std::vector<float>> PreDCAz;                    // pre-calculated DCA wrt PV
    std::shared_ptr<std::vector<float>> TPC_Signal;                 //
    std::shared_ptr<std::vector<int>> ITS_FirstLayer;               // -1 if no ITS hit
    std::shared_ptr<std::vector<float>> NSigmaPion;                 //
    std::shared_ptr<std::vector<float>> NSigmaKaon;                 //
    std::shared_ptr<std::vector<float>> NSigmaProton;               //
    std::shared_ptr<std::vector<std::array<float, 21>>> CovMatrix;  //
#if E2R_TPC_EXTRA
    std::shared_ptr<std::vector<float>> TPC_DCAxy;                   //
    std::shared_ptr<std::vector<float>> TPC_DCAz;                    //
    std::shared_ptr<std::vector<float>> TPC_NCrossedRows;            //
    std::shared_ptr<std::vector<unsigned int>> TPC_NClusters;        //
    std::shared_ptr<std::vector<unsigned int>> TPC_NClustersLC;      // from lowercase method `GetTPCncls`
    std::shared_ptr<std::vector<unsigned int>> TPC_NClustersFound;   //
    std::shared_ptr<std::vector<unsigned int>> TPC_NClustersShared;  //
    std::shared_ptr<std::vector<float>> TPC_Chi2;                    //
    std::shared_ptr<std::vector<float>> TPC_Chi2Constrained;         // TPC-only chi2 at the primary vertex
    std::shared_ptr<std::vector<float>> TPC_Chi2TCVG;                // chi2 TPC constrained vs global
    std::shared_ptr<std::vector<float>> TPC_SignalTunedOnData;       // (mc only)
    std::shared_ptr<std::vector<float>> TPC_SignalSigma;             //
    std::shared_ptr<std::vector<float>> TPC_SignalCorrected;         //
    std::shared_ptr<std::vector<float>> TPC_ESignalPion;             // `E` stands for `Expected`
    std::shared_ptr<std::vector<float>> TPC_ESigmaPion;              //
    std::shared_ptr<std::vector<float>> TPC_ESignalKaon;             //
    std::shared_ptr<std::vector<float>> TPC_ESigmaKaon;              //
    std::shared_ptr<std::vector<float>> TPC_ESignalProton;           //
    std::shared_ptr<std::vector<float>> TPC_ESigmaProton;            //
    std::shared_ptr<std::vector<unsigned int>> TPC_SignalN;          //
    std::shared_ptr<std::vector<float>> TPC_PointsFirst;             //
    std::shared_ptr<std::vector<float>> TPC_PointsIndexMax;          //
    std::shared_ptr<std::vector<float>> TPC_PointsLast;              //
    std::shared_ptr<std::vector<float>> TPC_PointsMaxDens;           //
    std::shared_ptr<std::vector<int>> TPC_FirstRow;                  //
#endif
    // mc link
    std::shared_ptr<std::vector<int>> McEntry;  // (mc only)
};

}  // namespace E2R
