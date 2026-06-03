#pragma once

#include <array>

#include "Constants.hpp"

namespace POD {

struct Track {
    unsigned int EsdEntry;
    float X;   // from inner parametrization
    float Y;   // from inner parametrization
    float Z;   // from inner parametrization
    float Px;  // from inner parametrization
    float Py;  // from inner parametrization
    float Pz;  // from inner parametrization
    int Charge;
    float PreDCAxy;  // pre-calculated DCA wrt PV
    float PreDCAz;   // pre-calculated DCA wrt PV
    float TPC_Signal;
    int ITS_FirstLayer;  // -1 if no ITS hit
    float NSigmaPion;
    float NSigmaKaon;
    float NSigmaProton;
    std::array<float, Common::NCovMatrixComponents_State6> CovMatrix;
#if E2R_TPC_EXTRA
    float TPC_DCAxy;
    float TPC_DCAz;
    float TPC_NCrossedRows;
    unsigned int TPC_NClusters;
    unsigned int TPC_NClustersLC;  // from lowercase method `GetTPCncls`
    unsigned int TPC_NClustersFound;
    unsigned int TPC_NClustersShared;
    float TPC_Chi2;
    float TPC_Chi2Constrained;    // TPC-only chi2 at the primary vertex
    float TPC_Chi2TCVG;           // chi2 TPC constrained vs global
    float TPC_SignalTunedOnData;  // (mc only)
    float TPC_SignalSigma;
    float TPC_SignalCorrected;
    float TPC_ESignalPion;  // `E` stands for `Expected`
    float TPC_ESigmaPion;
    float TPC_ESignalKaon;
    float TPC_ESigmaKaon;
    float TPC_ESignalProton;
    float TPC_ESigmaProton;
    unsigned int TPC_SignalN;
    float TPC_PointsFirst;
    float TPC_PointsIndexMax;
    float TPC_PointsLast;
    float TPC_PointsMaxDens;
    int TPC_FirstRow;
#endif
};

}  // namespace POD
