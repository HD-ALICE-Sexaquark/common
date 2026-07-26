#pragma once

#include <array>

#include "Constants.hpp"

namespace POD {

struct Track {
    unsigned int EsdEntry;
    float X;         // from inner parametrization
    float Y;         // from inner parametrization
    float Z;         // from inner parametrization
    float Px;        // from inner parametrization
    float Py;        // from inner parametrization
    float Pz;        // from inner parametrization
    float PreDCAxy;  // abs. value of pre-calculated DCA wrt PV
    float PreDCAz;   // abs. value of pre-calculated DCA wrt PV
    float NSigmasPion;
    float NSigmasKaon;
    float NSigmasProton;
    std::array<float, Common::NCovMatrixComponents_State6> CovMatrix;
    float TPC_Signal;
    int TPC_FirstRow;  // in the cluster map
    float TPC_NCrossedRows;
    unsigned short TPC_NClusters;
    unsigned short TPC_NClustersFindable;
    float TPC_Chi2;
    short Charge;
};

}  // namespace POD
