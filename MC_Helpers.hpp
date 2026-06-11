#pragma once

#include <algorithm>
#include <cstddef>
#include <optional>
#include <tuple>
#include <vector>

#include "Constants.hpp"
#include "DB_Particles.hpp"
#include "DB_ReactionChannels.hpp"
#include "HD_Library.hpp"
#include "POD_McParticle.hpp"

namespace MC {

struct Cache {
    bool IsTrue{false};
    bool IsGen1Signal{false};
    bool IsGen2Signal{false};
    bool IsTrueSignal{false};
    bool IsSecondary{false};
    int SignalID{Common::DummyInt};
    // -- mother info
    int Mother_PdgCode{Common::DummyInt};
    // -- grandmother info
    int GM_McEntry{Common::DummyInt};
    int GM_PdgCode{Common::DummyInt};
    // -- daughters' info
    float Decay_X{Common::DummyFloat};
    float Decay_Y{Common::DummyFloat};
    float Decay_Z{Common::DummyFloat};
};

inline void Apply(POD::Extended::McParticle &mc, const Cache &c) {
    mc.IsTrue = c.IsTrue;
    mc.IsGen1Signal = c.IsGen1Signal;
    mc.IsGen2Signal = c.IsGen2Signal;
    mc.IsTrueSignal = c.IsTrueSignal;
    mc.IsSecondary = c.IsSecondary;
    mc.SignalID = c.SignalID;
    mc.Mother_PdgCode = c.Mother_PdgCode;
    mc.GM_McEntry = c.GM_McEntry;
    mc.GM_PdgCode = c.GM_PdgCode;
    mc.Decay_X = c.Decay_X;
    mc.Decay_Y = c.Decay_Y;
    mc.Decay_Z = c.Decay_Z;
}

inline std::optional<std::size_t> FindCommonMotherMcEntry(const POD::McParticle &daughter1, const POD::McParticle &daughter2) {
    if (daughter1.Mother_McEntry < 0) return std::nullopt;
    if (daughter2.Mother_McEntry < 0) return std::nullopt;
    if (daughter1.Mother_McEntry != daughter2.Mother_McEntry) return std::nullopt;
    return daughter1.Mother_McEntry;
}

// clang-format off
inline std::tuple<float, float, float>
GetDecayVertex(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection) {
    if (mc.FirstDau_McEntry < 0) return {Common::DummyFloat, Common::DummyFloat, Common::DummyFloat};
    const auto &first_dau = mc_collection[static_cast<std::size_t>(mc.FirstDau_McEntry)];
    return {first_dau.Origin_X, first_dau.Origin_Y, first_dau.Origin_Z};
}
// clang-format on

// Find first daughter with an specific PdgCode and return its McEntry.
inline std::optional<std::size_t> FindDaughterMcEntry(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection,
                                                      int target_pdg_code) {
    if (mc.N_Daughters == 0) return std::nullopt;
    if (mc.FirstDau_McEntry < 0) return std::nullopt;
    for (int entry_dau = mc.FirstDau_McEntry; entry_dau <= mc.LastDau_McEntry; ++entry_dau) {
        const auto &mc_dau = mc_collection[static_cast<std::size_t>(entry_dau)];
        if (mc_dau.PdgCode == target_pdg_code) return static_cast<std::size_t>(entry_dau);
    }
    return std::nullopt;
}

namespace SexaquarkRules {

// NOTE: no true hypothesis here.
inline bool IsGen1Signal(const POD::McParticle &mc, const DB::ReactionChannels::Definition &r_channel) {
    // (1)) logical primary (= no mother)
    if (mc.Mother_McEntry > Common::DummyInt) return false;
    // (2) should come from the anti-sexaquark reaction generator
    if (mc.Generator != E2R::EGenerator::kInjectedAntiSexaquarkReaction) return false;
    // (3) pdg is found in reaction products?
    if (std::ranges::find(r_channel.products_pdg, mc.PdgCode) == r_channel.products_pdg.end()) return false;
    // (4) mc status has to be [600,620[
    if (mc.StatusCode < E2R::ReactionID_Offset || mc.StatusCode >= E2R::ReactionID_Offset + E2R::NSexaReactionsPerEvent) return false;
    return true;
}

inline Cache ClassifyDownstream(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection,
                                const DB::ReactionChannels::Definition &r_channel, int pdg_code_hypothesis, bool include_gm, bool include_dv) {
    Cache c;
    c.IsTrue = mc.PdgCode == pdg_code_hypothesis;

    if (mc.Mother_McEntry < 0) {
        c.IsGen1Signal = SexaquarkRules::IsGen1Signal(mc, r_channel);
        c.IsGen2Signal = false;
        if (c.IsGen1Signal) c.SignalID = static_cast<int>(mc.StatusCode);
    } else {
        const auto &mother = mc_collection[static_cast<std::size_t>(mc.Mother_McEntry)];
        c.IsGen1Signal = false;
        c.IsGen2Signal = SexaquarkRules::IsGen1Signal(mother, r_channel);
        if (c.IsGen2Signal) c.SignalID = static_cast<int>(mother.StatusCode);
        c.Mother_PdgCode = mother.PdgCode;
        if (include_gm && mother.Mother_McEntry > Common::DummyInt) {
            c.GM_McEntry = mother.Mother_McEntry;
            c.GM_PdgCode = mc_collection[static_cast<std::size_t>(mother.Mother_McEntry)].PdgCode;
        }
    }

    c.IsTrueSignal = (c.IsGen1Signal || c.IsGen2Signal) && c.IsTrue;
    c.IsSecondary = mc.IsSecFromMat || mc.IsSecFromWeak || c.IsGen1Signal || c.IsGen2Signal;

    if (include_dv && mc.FirstDau_McEntry > Common::DummyInt) {
        const auto &first_dau = mc_collection[static_cast<std::size_t>(mc.FirstDau_McEntry)];
        c.Decay_X = first_dau.Origin_X;
        c.Decay_Y = first_dau.Origin_Y;
        c.Decay_Z = first_dau.Origin_Z;
    }

    return c;
}

inline std::optional<std::size_t> FindCommonReactionID(const POD::Extended::McParticle &mc_dau1, const POD::Extended::McParticle &mc_dau2) {
    if (mc_dau1.SignalID < 0) return std::nullopt;
    if (mc_dau2.SignalID < 0) return std::nullopt;
    if (mc_dau1.SignalID != mc_dau2.SignalID) return std::nullopt;
    return static_cast<std::size_t>(mc_dau1.SignalID) - E2R::ReactionID_Offset;
}

}  // namespace SexaquarkRules

namespace HdibaryonRules {

inline Cache ClassifyDownstream(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection, const HD::DecayTree &decay_pid,
                                int pdg_code_hypothesis, bool include_dv) {

    Cache c;
    c.IsTrue = mc.PdgCode == pdg_code_hypothesis;
    c.IsGen1Signal = false;
    c.IsGen2Signal = false;

    // -- check if it's first-gen decay product
    if (mc.Mother_McEntry > Common::DummyInt) {
        const auto &mother = mc_collection[static_cast<std::size_t>(mc.Mother_McEntry)];
        c.Mother_PdgCode = mother.PdgCode;
        c.IsGen1Signal = mother.PdgCode == decay_pid.hdibaryon.pdg_code;
        if (c.IsGen1Signal) c.SignalID = static_cast<int>(mother.StatusCode);
        // -- check if it's second-gen decay product
        if (mother.Mother_McEntry > Common::DummyInt) {
            const auto &gm = mc_collection[static_cast<std::size_t>(mother.Mother_McEntry)];
            c.GM_McEntry = mother.Mother_McEntry;
            c.GM_PdgCode = gm.PdgCode;
            c.IsGen2Signal = gm.PdgCode == decay_pid.hdibaryon.pdg_code;
            if (c.IsGen2Signal) c.SignalID = static_cast<int>(gm.StatusCode);
        }
    }

    c.IsTrueSignal = (c.IsGen1Signal || c.IsGen2Signal) && c.IsTrue;
    c.IsSecondary = mc.IsSecFromMat || mc.IsSecFromWeak || c.IsGen1Signal || c.IsGen2Signal;

    if (include_dv && mc.FirstDau_McEntry > Common::DummyInt) {
        const auto &first_dau = mc_collection[static_cast<std::size_t>(mc.FirstDau_McEntry)];
        c.Decay_X = first_dau.Origin_X;
        c.Decay_Y = first_dau.Origin_Y;
        c.Decay_Z = first_dau.Origin_Z;
    }

    return c;
}

}  // namespace HdibaryonRules

}  // namespace MC
