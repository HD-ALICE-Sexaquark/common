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
    short Charge;
    float PreDCAxy;  // pre-calculated DCA wrt PV
    float PreDCAz;   // pre-calculated DCA wrt PV
    float TPC_Signal;
    float NSigmasPion;
    float NSigmasKaon;
    float NSigmasProton;
    std::array<float, Common::NCovMatrixComponents_State6> CovMatrix;
    int TPC_FirstRow;  // in the cluster map
#if E2R_TPC_EXTRA
    float TPC_Chi2;
    float TPC_Chi2Constrained;  // TPC-only chi2 at the primary vertex
    float TPC_Chi2TCVG;         // chi2 TPC constrained vs global
    float TPC_ESignalPion;
    float TPC_ESignalKaon;
    float TPC_ESignalProton;
    float TPC_ESigmaPion;
    float TPC_ESigmaKaon;
    float TPC_ESigmaProton;
    float TPC_NCrossedRows;
    unsigned short TPC_NClusters;
    unsigned short TPC_NClustersLC;  // from lowercase method `GetTPCncls`
    unsigned short TPC_NClustersFindable;
    unsigned short TPC_NClustersShared;
#endif
};

}  // namespace POD
