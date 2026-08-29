#pragma once

namespace POD {

struct Sexaquark {
    // antisexaquark candidate info
    float SV_X;
    float SV_Y;
    float SV_Z;
    float Px;      // = Dau1_Fit_Px + Dau2_Fit_Px
    float Py;      // = Dau1_Fit_Py + Dau2_Fit_Py
    float Pz;      // = Dau1_Fit_Pz + Dau2_Fit_Pz
    float Energy;  // = Dau1_Fit_Energy + Dau2_Fit_Energy
    float Chi2NDF;
    float E_MinusNucleon;
    bool IsWrongSignChannel;
    // daughter 1
    float Dau1_PCAwrtSV_X;  // the PCA is the seeder's
    float Dau1_PCAwrtSV_Y;
    float Dau1_PCAwrtSV_Z;
    float Dau1_Fit_Px;
    float Dau1_Fit_Py;
    float Dau1_Fit_Pz;
    float Dau1_Fit_Energy;
    // daughter 2
    float Dau2_PCAwrtSV_X;
    float Dau2_PCAwrtSV_Y;
    float Dau2_PCAwrtSV_Z;
    float Dau2_Fit_Px;
    float Dau2_Fit_Py;
    float Dau2_Fit_Pz;
    float Dau2_Fit_Energy;
};

}  // namespace POD
