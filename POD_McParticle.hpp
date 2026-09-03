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
#if E2T_MC_EXTRA
    bool IsOOBPileup{false};
#endif
};

namespace Extended {

struct McParticle : POD::McParticle {
    int McEntry{Common::DummyInt};                         // own entry in the event's mc collection
    float Decay_X{Common::DummyFloat};                     // (makes sense for decaying particles only)
    float Decay_Y{Common::DummyFloat};                     // (makes sense for decaying particles only)
    float Decay_Z{Common::DummyFloat};                     // (makes sense for decaying particles only)
    int SignalID{Common::DummyInt};                        // = "ReactionID" / "InjectionID" = status code of the injected ancestor
    int SignalGeneration{Common::DummyInt};                // tier below the injected ancestor; see `MC::Generation` in `MC_Helpers.hpp`
    int Mother_PdgCode{Common::DummyInt};                  //
    int GM_McEntry{Common::DummyInt};                      //
    int GM_PdgCode{Common::DummyInt};                      //
    std::uint8_t GeneratorMask{Common::OriginGen::kNone};  // bitmask of generators of origin, OR-ed over the charged constituents
    bool IsTrue{false};                                    // particle satisfies current PID hypothesis
    bool IsTrueSignal{false};                              // true member of an injection's own decay tree, under current PID hypothesis
    bool IsSecondary{false};                               // = IsSecFromMat || IsSecFromWeak || SignalGeneration >= 1
    bool IsRealBkg{false};                                 // carries no signal at all: neither it nor any constituent descends from an injection
};

}  // namespace Extended

}  // namespace POD
