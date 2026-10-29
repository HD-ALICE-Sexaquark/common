#pragma once

#include <cstdint>

#include "Constants.hpp"

namespace POD {

struct McParticle {
    int PdgCode{Common::DummyInt};
    int Charge{Common::DummyInt};
    int Mother_McEntry{Common::DummyInt};
    unsigned int N_Daughters{0};
    int FirstDau_McEntry{Common::DummyInt};
    int LastDau_McEntry{Common::DummyInt};
    float Origin_X{Common::DummyFloat};
    float Origin_Y{Common::DummyFloat};
    float Origin_Z{Common::DummyFloat};
    float Px{Common::DummyFloat};
    float Py{Common::DummyFloat};
    float Pz{Common::DummyFloat};
    float Energy{Common::DummyFloat};
    unsigned int StatusCode{0};
    std::uint8_t Generator{0};
    bool IsPhysPrimary{false};  // is physical primary according to the ALICE definition
    bool IsSecFromMat{false};
    bool IsSecFromWeak{false};
    bool IsOOBPileup{false};
};

}  // namespace POD
