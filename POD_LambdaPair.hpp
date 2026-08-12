#pragma once

namespace POD {

struct LambdaPair {
    // candidate info
    float Decay_X;
    float Decay_Y;
    float Decay_Z;
    float Px;
    float Py;
    float Pz;
    float Energy;
    float Chi2NDF;
    bool IsAntiChannel;
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
