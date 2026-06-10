#pragma once

namespace POD {

struct Sexaquark {
    // -- candidate info
    float SV_X;
    float SV_Y;
    float SV_Z;
    float Px;
    float Py;
    float Pz;
    float Energy;
    float Chi2NDF;
    float E_MinusNucleon;
    bool IsBkgChannel;
    // -- daughter 1
    float Dau1_PCAwrtSV_X;
    float Dau1_PCAwrtSV_Y;
    float Dau1_PCAwrtSV_Z;
    float Dau1_PCAwrtSV_Px;
    float Dau1_PCAwrtSV_Py;
    float Dau1_PCAwrtSV_Pz;
    // -- daughter 2
    float Dau2_PCAwrtSV_X;
    float Dau2_PCAwrtSV_Y;
    float Dau2_PCAwrtSV_Z;
    float Dau2_PCAwrtSV_Px;
    float Dau2_PCAwrtSV_Py;
    float Dau2_PCAwrtSV_Pz;
};

}  // namespace POD
