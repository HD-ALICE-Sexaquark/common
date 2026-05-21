#pragma once

#include <memory>
#include <vector>

namespace E2R {

struct Lambda {
    std::shared_ptr<std::vector<unsigned int>> EsdEntry;
    std::shared_ptr<std::vector<float>> Decay_X;
    std::shared_ptr<std::vector<float>> Decay_Y;
    std::shared_ptr<std::vector<float>> Decay_Z;
    std::shared_ptr<std::vector<float>> Px;
    std::shared_ptr<std::vector<float>> Py;
    std::shared_ptr<std::vector<float>> Pz;
    std::shared_ptr<std::vector<float>> DcaV0Daughters;
    // neg. daughter
    std::shared_ptr<std::vector<unsigned int>> Neg_EsdEntry;
    std::shared_ptr<std::vector<float>> Neg_PCAwrtV0_Px;
    std::shared_ptr<std::vector<float>> Neg_PCAwrtV0_Py;
    std::shared_ptr<std::vector<float>> Neg_PCAwrtV0_Pz;
    std::shared_ptr<std::vector<float>> Neg_PreDCAxy;
    std::shared_ptr<std::vector<float>> Neg_PreDCAz;
    std::shared_ptr<std::vector<float>> Neg_NSigmaProton;
    std::shared_ptr<std::vector<float>> Neg_NSigmaKaon;
    std::shared_ptr<std::vector<float>> Neg_NSigmaPion;
    // pos. daughter
    std::shared_ptr<std::vector<unsigned int>> Pos_EsdEntry;
    std::shared_ptr<std::vector<float>> Pos_PCAwrtV0_Px;
    std::shared_ptr<std::vector<float>> Pos_PCAwrtV0_Py;
    std::shared_ptr<std::vector<float>> Pos_PCAwrtV0_Pz;
    std::shared_ptr<std::vector<float>> Pos_PreDCAxy;
    std::shared_ptr<std::vector<float>> Pos_PreDCAz;
    std::shared_ptr<std::vector<float>> Pos_NSigmaProton;
    std::shared_ptr<std::vector<float>> Pos_NSigmaKaon;
    std::shared_ptr<std::vector<float>> Pos_NSigmaPion;
    // mc link
    std::shared_ptr<std::vector<int>> Neg_McEntry;
    std::shared_ptr<std::vector<int>> Pos_McEntry;
};

}  // namespace E2R
