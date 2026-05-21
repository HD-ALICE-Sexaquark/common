#pragma once

#include <memory>
#include <vector>

namespace E2R {

struct McParticle {
    std::shared_ptr<std::vector<int>> PdgCode;
    std::shared_ptr<std::vector<char>> Charge;
    std::shared_ptr<std::vector<int>> Mother_McEntry;
    std::shared_ptr<std::vector<unsigned int>> N_Daughters;
    std::shared_ptr<std::vector<int>> FirstDau_McEntry;
    std::shared_ptr<std::vector<int>> LastDau_McEntry;
    std::shared_ptr<std::vector<float>> Origin_X;
    std::shared_ptr<std::vector<float>> Origin_Y;
    std::shared_ptr<std::vector<float>> Origin_Z;
    std::shared_ptr<std::vector<float>> Px;
    std::shared_ptr<std::vector<float>> Py;
    std::shared_ptr<std::vector<float>> Pz;
    std::shared_ptr<std::vector<float>> E;
    std::shared_ptr<std::vector<unsigned int>> Status;
    std::shared_ptr<std::vector<char>> Generator;      // 0: HIJING, 1: anti-neutron injector, 2: anti-sexaquark reaction
    std::shared_ptr<std::vector<char>> IsPhysPrimary;  // is physical primary according to the ALICE definition
    std::shared_ptr<std::vector<char>> IsSecFromMat;
    std::shared_ptr<std::vector<char>> IsSecFromWeak;
    std::shared_ptr<std::vector<char>> IsOOBPileup;
};

}  // namespace E2R
