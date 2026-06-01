#pragma once

#include <cstdint>
#include <optional>

#include "Constants.hpp"

namespace POD {

struct McParticle {
    int PdgCode;
    int Charge;
    std::optional<unsigned int> Mother_McEntry;
    unsigned int N_Daughters;
    std::optional<unsigned int> FirstDau_McEntry;
    std::optional<unsigned int> LastDau_McEntry;
    float Origin_X;
    float Origin_Y;
    float Origin_Z;
    float Px;
    float Py;
    float Pz;
    float Energy;
    unsigned int Status;
    std::uint8_t Generator;
    bool IsPhysPrimary;  // is physical primary according to the ALICE definition
    bool IsSecFromMat;
    bool IsSecFromWeak;
#if E2R_MC_EXTRA
    bool IsOOBPileup;
#endif
    // filled at `R2DS`
    std::optional<float> Decay_X;            // (v0 only)
    std::optional<float> Decay_Y;            // (v0 only)
    std::optional<float> Decay_Z;            // (v0 only)
    std::optional<bool> IsTrue;              //
    std::optional<bool> IsGen1Signal;        //
    std::optional<bool> IsGen2Signal;        //
    std::optional<bool> IsTrueSignal;        //
    std::optional<bool> IsSecondary;         //
    std::optional<bool> IsHybrid;            // (v0 only)
    std::optional<unsigned int> SignalID;    // (= "ReactionID" / "InjectionID"
    std::optional<int> Mother_PdgCode;       //
    std::optional<unsigned int> GM_McEntry;  // (track only)
    std::optional<int> GM_PdgCode;           // (track only)
};

}  // namespace POD
