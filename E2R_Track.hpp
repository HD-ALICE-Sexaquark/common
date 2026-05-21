#pragma once

#include <memory>
#include <vector>

namespace E2R {

struct Track {
    std::shared_ptr<std::vector<unsigned int>> EsdEntry;            //
    std::shared_ptr<std::vector<float>> X;                          // inner parametrization
    std::shared_ptr<std::vector<float>> Y;                          // inner parametrization
    std::shared_ptr<std::vector<float>> Z;                          // inner parametrization
    std::shared_ptr<std::vector<float>> Px;                         // inner parametrization
    std::shared_ptr<std::vector<float>> Py;                         // inner parametrization
    std::shared_ptr<std::vector<float>> Pz;                         // inner parametrization
    std::shared_ptr<std::vector<char>> Charge;                      //
    std::shared_ptr<std::vector<float>> PreDCAxy;                   // pre-calculated DCA wrt PV
    std::shared_ptr<std::vector<float>> PreDCAz;                    // pre-calculated DCA wrt PV
    std::shared_ptr<std::vector<float>> TPC_Signal;                 //
    std::shared_ptr<std::vector<int>> ITS_FirstLayer;               // -1 if no ITS hit
    std::shared_ptr<std::vector<float>> NSigmaPion;                 //
    std::shared_ptr<std::vector<float>> NSigmaKaon;                 //
    std::shared_ptr<std::vector<float>> NSigmaProton;               //
    std::shared_ptr<std::vector<std::array<float, 21>>> CovMatrix;  //
    // extra tpc info
    std::shared_ptr<std::vector<float>> TPC_DCAxy;                   //
    std::shared_ptr<std::vector<float>> TPC_DCAz;                    //
    std::shared_ptr<std::vector<float>> TPC_NCrossedRows;            //
    std::shared_ptr<std::vector<unsigned int>> TPC_NClusters;        //
    std::shared_ptr<std::vector<unsigned int>> TPC_NClustersLC;      // `LC` stands for the lowercase method `GetTPCncls`
    std::shared_ptr<std::vector<unsigned int>> TPC_NClustersFound;   //
    std::shared_ptr<std::vector<unsigned int>> TPC_NClustersShared;  //
    std::shared_ptr<std::vector<float>> TPC_Chi2;                    //
    std::shared_ptr<std::vector<float>> TPC_Chi2Constrained;         // TPC-only chi2 at the primary vertex
    std::shared_ptr<std::vector<float>> TPC_Chi2TCVG;                // chi2 TPC constrained vs global
    std::shared_ptr<std::vector<float>> TPC_SignalTunedOnData;       // NOTE: MC only
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
    // mc link
    std::shared_ptr<std::vector<int>> McEntry;  // only available for MC
};

}  // namespace E2R
