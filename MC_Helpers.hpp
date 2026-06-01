#pragma once

#include <optional>
#include <tuple>
#include <vector>

#include "common/Constants.hpp"
#include "common/DB_Particles.hpp"
#include "common/DB_ReactionChannels.hpp"
#include "common/HD_Library.hpp"
#include "common/POD_InjectedSexa.hpp"
#include "common/POD_McParticle.hpp"

namespace MC {

struct Cache {
    bool IsTrue{false};
    bool IsGen1Signal{false};
    bool IsGen2Signal{false};
    bool IsTrueSignal{false};
    bool IsSecondary{false};
    std::optional<unsigned int> SignalID;
    // -- mother info
    std::optional<int> Mother_PdgCode;
    // -- grandmother info
    std::optional<unsigned int> GM_McEntry;
    std::optional<int> GM_PdgCode;
    // -- daughters' info
    std::optional<float> Decay_X;
    std::optional<float> Decay_Y;
    std::optional<float> Decay_Z;
};

inline void Apply(POD::McParticle &mc, const Cache &c) {
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
    if (!daughter1.Mother_McEntry.has_value()) return std::nullopt;
    if (!daughter2.Mother_McEntry.has_value()) return std::nullopt;
    if (daughter1.Mother_McEntry.value() != daughter2.Mother_McEntry.value()) return std::nullopt;
    return daughter1.Mother_McEntry;
}

inline std::tuple<std::optional<float>, std::optional<float>, std::optional<float>> GetDecayVertex(
    const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection) {
    if (!mc.FirstDau_McEntry.has_value()) return {std::nullopt, std::nullopt, std::nullopt};
    const auto &first_dau = mc_collection[mc.FirstDau_McEntry.value()];
    return {first_dau.Origin_X, first_dau.Origin_Y, first_dau.Origin_Z};
}

inline std::pair<bool, bool> IsSignalIsHybrid(const std::optional<POD::McParticle> &mc) {
    if (!mc.has_value()) return {false, false};
    return {mc->IsTrueSignal.value_or(false), mc->IsHybrid.value_or(false)};
}

namespace SexaquarkRules {

// NOTE: no true hypothesis here.
inline bool IsGen1Signal(const POD::McParticle &mc, const DB::ReactionChannels::Definition &r_channel) {
    // (1)) logical primary (= no mother)
    if (mc.Mother_McEntry.has_value()) return false;
    // (2) should come from the anti-sexaquark reaction generator
    if (mc.Generator != E2R::EGenerator::kInjectedAntiSexaquarkReaction) return false;
    // (3) pdg is found in reaction products?
    if (std::ranges::find(r_channel.products_pdg, mc.PdgCode) == r_channel.products_pdg.end()) return false;
    // (4) mc status has to be [600,620[
    if (mc.Status < E2R::ReactionID_Offset || mc.Status >= E2R::ReactionID_Offset + E2R::NSexaReactionsPerEvent) return false;
    return true;
}

inline Cache Classify(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection, const DB::ReactionChannels::Definition &r_channel,
                      int pdg_code_hypothesis, bool include_gm, bool include_dv) {
    Cache c;
    c.IsTrue = mc.PdgCode == pdg_code_hypothesis;

    if (!mc.Mother_McEntry.has_value()) {
        c.IsGen1Signal = SexaquarkRules::IsGen1Signal(mc, r_channel);
        c.IsGen2Signal = false;
        if (c.IsGen1Signal) c.SignalID = mc.Status;
    } else {
        const auto &mother = mc_collection[mc.Mother_McEntry.value()];
        c.IsGen1Signal = false;
        c.IsGen2Signal = SexaquarkRules::IsGen1Signal(mother, r_channel);
        if (c.IsGen2Signal) c.SignalID = mother.Status;
        c.Mother_PdgCode = mother.PdgCode;
        if (include_gm && mother.Mother_McEntry.has_value()) {
            c.GM_McEntry = mother.Mother_McEntry;
            c.GM_PdgCode = mc_collection[mother.Mother_McEntry.value()].PdgCode;
        }
    }

    c.IsTrueSignal = (c.IsGen1Signal || c.IsGen2Signal) && c.IsTrue;
    c.IsSecondary = mc.IsSecFromMat || mc.IsSecFromWeak || c.IsGen1Signal || c.IsGen2Signal;

    if (include_dv && mc.FirstDau_McEntry.has_value()) {
        const auto &first_dau = mc_collection[mc.FirstDau_McEntry.value()];
        c.Decay_X = first_dau.Origin_X;
        c.Decay_Y = first_dau.Origin_Y;
        c.Decay_Z = first_dau.Origin_Z;
    }

    return c;
}

inline std::optional<POD::InjectedSexa> FindInjected(const std::optional<POD::McParticle> &mc_product1,
                                                     const std::optional<POD::McParticle> &mc_product2,
                                                     const std::vector<POD::InjectedSexa> &inj_collection, bool control_channel) {
    // control channel has no linked injected mc //
    if (control_channel) return std::nullopt;
    if (!mc_product1.has_value()) return std::nullopt;
    const auto &mc_1 = mc_product1.value();
    if (!mc_product2.has_value()) return std::nullopt;
    const auto &mc_2 = mc_product2.value();
    auto rid_1 = mc_1.SignalID;
    if (!rid_1.has_value()) return std::nullopt;
    auto rid_2 = mc_2.SignalID;
    if (!rid_2.has_value()) return std::nullopt;
    if (rid_1.value() != rid_2.value()) return std::nullopt;
    auto entry_injected = rid_1.value() - E2R::ReactionID_Offset;
    return inj_collection[entry_injected];
}

}  // namespace SexaquarkRules

namespace HdibaryonRules {

inline Cache Classify(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection, const HD::DecayTree &decay_pid,
                      int pdg_code_hypothesis, bool include_dv) {

    Cache c;
    c.IsTrue = mc.PdgCode == pdg_code_hypothesis;
    c.IsGen1Signal = false;
    c.IsGen2Signal = false;

    if (mc.Mother_McEntry.has_value()) {
        const auto &mother = mc_collection[mc.Mother_McEntry.value()];
        c.Mother_PdgCode = mother.PdgCode;
        c.IsGen1Signal = c.Mother_PdgCode == decay_pid.hdibaryon.pdg_code && mc.PdgCode == decay_pid.lambda.pdg_code;
        if (mother.Mother_McEntry.has_value()) {
            c.GM_McEntry = mother.Mother_McEntry;
            c.GM_PdgCode = mc_collection[mother.Mother_McEntry.value()].PdgCode;
            c.IsGen2Signal =
                c.GM_PdgCode == decay_pid.hdibaryon.pdg_code && (mc.PdgCode == decay_pid.neg.pdg_code || mc.PdgCode == decay_pid.pos.pdg_code);
        }
    }

    c.IsTrueSignal = (c.IsGen1Signal || c.IsGen2Signal) && c.IsTrue;
    if (c.IsGen1Signal || c.IsGen2Signal) c.SignalID = mc.Status;
    c.IsSecondary = mc.IsSecFromMat || mc.IsSecFromWeak || c.IsGen1Signal || c.IsGen2Signal;

    if (include_dv && mc.FirstDau_McEntry.has_value()) {
        const auto &first_dau = mc_collection[mc.FirstDau_McEntry.value()];
        c.Decay_X = first_dau.Origin_X;
        c.Decay_Y = first_dau.Origin_Y;
        c.Decay_Z = first_dau.Origin_Z;
    }

    return c;
}

}  // namespace HdibaryonRules

}  // namespace MC
