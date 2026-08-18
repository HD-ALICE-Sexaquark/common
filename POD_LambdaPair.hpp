#pragma once

namespace POD {

struct LambdaPair {
    // candidate info
    float Decay_X;
    float Decay_Y;
    float Decay_Z;
    float Px;       // = L1_Fit_Px + L2_Fit_Px
    float Py;       // = L1_Fit_Py + L2_Fit_Py
    float Pz;       // = L1_Fit_Pz + L2_Fit_Pz
    float Energy;   // = L1_Fit_Energy + L2_Fit_Energy
    float Chi2NDF;  // NDF = 1
    // available with PV constraint
    float CV_X;
    float CV_Y;
    float CV_Z;
    float CV_Px;
    float CV_Py;
    float CV_Pz;
    float CV_Energy;
    float CV_DecayLength;
    float CV_DecayLengthErr;
    float Chi2CV;  // chi2 w.r.t pv; 2 d.o.f.
    // (anti)lambda 1
    float Lambda1_PCAwrtDV_X;
    float Lambda1_PCAwrtDV_Y;
    float Lambda1_PCAwrtDV_Z;
    float Lambda1_Fit_Px;
    float Lambda1_Fit_Py;
    float Lambda1_Fit_Pz;
    float Lambda1_Fit_Energy;
    // (anti)lambda 2
    float Lambda2_PCAwrtDV_X;
    float Lambda2_PCAwrtDV_Y;
    float Lambda2_PCAwrtDV_Z;
    float Lambda2_Fit_Px;
    float Lambda2_Fit_Py;
    float Lambda2_Fit_Pz;
    float Lambda2_Fit_Energy;
};

}  // namespace POD
