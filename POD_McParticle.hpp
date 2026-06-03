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
#if E2R_MC_EXTRA
    bool IsOOBPileup{false};
#endif
};

namespace Extended {

struct McParticle : POD::McParticle {
    float Decay_X{Common::DummyFloat};     // (v0 only)
    float Decay_Y{Common::DummyFloat};     // (v0 only)
    float Decay_Z{Common::DummyFloat};     // (v0 only)
    int SignalID{Common::DummyInt};        // = "ReactionID" / "InjectionID"
    int Mother_PdgCode{Common::DummyInt};  //
    int GM_McEntry{Common::DummyInt};      // (track only)
    int GM_PdgCode{Common::DummyInt};      // (track only)
    bool IsTrue{false};
    bool IsGen1Signal{false};
    bool IsGen2Signal{false};
    bool IsTrueSignal{false};
    bool IsSecondary{false};
    bool IsHybrid{false};  // (composite only, e.g. secondary v0s + on-the-fly lambdas)
};

}  // namespace Extended

}  // namespace POD
