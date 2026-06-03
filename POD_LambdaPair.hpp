#pragma once

namespace POD {

struct LambdaPair {
    // -- candidate info
    float Decay_X;
    float Decay_Y;
    float Decay_Z;
    float Px;
    float Py;
    float Pz;
    float Energy;
    bool AntiChannel;
    // -- lambda1
    float Lambda1_PCAwrtDV_X;
    float Lambda1_PCAwrtDV_Y;
    float Lambda1_PCAwrtDV_Z;
    float Lambda1_PCAwrtDV_Px;
    float Lambda1_PCAwrtDV_Py;
    float Lambda1_PCAwrtDV_Pz;
    // -- lambda2
    float Lambda2_PCAwrtDV_X;
    float Lambda2_PCAwrtDV_Y;
    float Lambda2_PCAwrtDV_Z;
    float Lambda2_PCAwrtDV_Px;
    float Lambda2_PCAwrtDV_Py;
    float Lambda2_PCAwrtDV_Pz;
};

}  // namespace POD
