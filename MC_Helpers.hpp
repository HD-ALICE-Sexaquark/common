#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>
#include <tuple>
#include <vector>

#include "Constants.hpp"
#include "DB_Particles.hpp"
#include "DB_ReactionChannels.hpp"
#include "POD_McParticle.hpp"

namespace MC {

// == Signal Generation == //

namespace Generation {
inline constexpr int kNone = Common::DummyInt;  // no injected ancestor
inline constexpr int kInjected = 0;      // the injected (anti)h-dibaryon itself (unreachable in sexaquark mc: the reaction leaves no mc record)
inline constexpr int kFirstGen = 1;      // reaction products / the (anti)h-dibaryon's (anti)lambdas
inline constexpr int kSecondGen = 2;     // their charged daughters, from weak decays
inline constexpr int kGeantProgeny = 3;  // anything deeper, transported by GEANT
}  // namespace Generation

// Where a particle sits within the injection it descends from, if any.
// PID-agnostic.
struct Provenance {
    int signal_id{Common::DummyInt};  // NOTE: stays dummy when unreachable, `generation` alone tells if there was an injected ancestor or not
    int generation{Generation::kNone};
};

inline bool IsValidSignalID(int id) { return id > Common::DummyInt; }

inline bool SameSignalID(int a, int b) { return IsValidSignalID(a) && a == b; }

inline bool IsRelevantGeneration(int generation) { return generation >= Generation::kInjected && generation < Generation::kGeantProgeny; }

// The three sets are `IsTrueSignal`, `IsRealBkg` and whatever is left; see `docs/MC_LABELS.md`.
inline bool CarriesSignal(const POD::Extended::McParticle &mc) { return !mc.IsRealBkg; }

inline bool IsHybrid(const POD::Extended::McParticle &mc) { return !mc.IsTrueSignal && !mc.IsRealBkg; }

inline bool SameMcParticle(const POD::Extended::McParticle &a, const POD::Extended::McParticle &b) {
    return a.McEntry > Common::DummyInt && a.McEntry == b.McEntry;
}

// == Generator of Origin == //

namespace Origin {

// The compact H|N|S composition code. Its seven non-empty values are the seven possible class mixtures.
enum class EClass : std::uint8_t {
    kNone = 0,
    kH = 1,
    kN = 2,
    kHN = 3,
    kS = 4,
    kHS = 5,
    kNS = 6,
    kHNS = 7,
};
inline constexpr auto Name_Class = std::to_array<std::string_view>({"None", "H", "N", "HN", "S", "HS", "NS", "HNS"});
[[nodiscard]] constexpr std::string_view Name(EClass origin_class) { return Name_Class[static_cast<std::size_t>(origin_class)]; }

[[nodiscard]] constexpr bool CarriesHIJING(std::uint8_t mask) { return (mask & Common::OriginGen::kHIJING) != 0; }
[[nodiscard]] constexpr bool CarriesInjectedBkg(std::uint8_t mask) { return (mask & Common::OriginGen::kInjectedBkg) != 0; }
[[nodiscard]] constexpr bool CarriesSignal(std::uint8_t mask) { return (mask & Common::OriginGen::kSignal) != 0; }

[[nodiscard]] constexpr bool OnlyHIJING(std::uint8_t mask) { return mask != Common::OriginGen::kNone && (mask & ~Common::OriginGen::kHIJING) == 0; }
[[nodiscard]] constexpr bool OnlyInjectedBkg(std::uint8_t mask) {
    return mask != Common::OriginGen::kNone && (mask & ~Common::OriginGen::kInjectedBkg) == 0;
}
[[nodiscard]] constexpr bool OnlySignal(std::uint8_t mask) { return mask != Common::OriginGen::kNone && (mask & ~Common::OriginGen::kSignal) == 0; }

[[nodiscard]] constexpr EClass Classes(std::uint8_t mask) {
    unsigned int code = 0;
    if (CarriesHIJING(mask)) code |= 1U << 0U;
    if (CarriesInjectedBkg(mask)) code |= 1U << 1U;
    if (CarriesSignal(mask)) code |= 1U << 2U;
    return static_cast<EClass>(code);
}

}  // namespace Origin

inline std::optional<std::size_t> FindMcEntry_CommonMother(const POD::McParticle &daughter1, const POD::McParticle &daughter2) {
    if (daughter1.Mother_McEntry < 0) return std::nullopt;
    if (daughter1.Mother_McEntry != daughter2.Mother_McEntry) return std::nullopt;
    return static_cast<std::size_t>(daughter1.Mother_McEntry);
}

inline std::tuple<float, float, float> GetDecayVertex(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection) {
    if (mc.FirstDau_McEntry < 0) return {Common::DummyFloat, Common::DummyFloat, Common::DummyFloat};
    const auto &first_dau = mc_collection[static_cast<std::size_t>(mc.FirstDau_McEntry)];
    return {first_dau.Origin_X, first_dau.Origin_Y, first_dau.Origin_Z};
}

// Find first daughter with an specific `PdgCode` and return its `McEntry`.
inline std::optional<std::size_t> FindMcEntry_Daughter(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection,
                                                       int target_pdg_code) {
    if (mc.N_Daughters == 0) return std::nullopt;
    if (mc.FirstDau_McEntry < 0) return std::nullopt;
    for (int entry_dau = mc.FirstDau_McEntry; entry_dau <= mc.LastDau_McEntry; ++entry_dau) {
        const auto &mc_dau = mc_collection[static_cast<std::size_t>(entry_dau)];
        if (mc_dau.PdgCode == target_pdg_code) return static_cast<std::size_t>(entry_dau);
    }
    return std::nullopt;
}

// == AntiSexaquark Rules == //

namespace SexaquarkRules {

// Does it descend from an injected antisexaquark-nucleon reaction?
// Valid because generator index is passed downstream.
inline bool IsFromSignalGenerator(const POD::McParticle &mc) {  //
    return mc.Generator == Common::ECustomGeneratorIdx::kInjectedAntiSexaquarkReaction;
}

// Quickly check if `status_code` is within [600,619]
inline bool IsReactionID(unsigned int status_code) {
    return status_code >= E2T::ReactionID_Offset && status_code < E2T::ReactionID_Offset + E2T::NSexaReactionsPerEvent;
}

// Subtract the reaction id offset (600) to `status code` to get index/position in injected collection.
inline std::size_t InjectionIndex(unsigned int status_code) {  //
    return static_cast<std::size_t>(status_code - E2T::ReactionID_Offset);
}

// The first generation of a reaction: logical primaries carrying their own reaction id.
// NOTE: channel- and hypothesis-independent, on purpose; provenance must not depend on either.
inline bool IsGen1Signal(const POD::McParticle &mc) {
    return IsFromSignalGenerator(mc) && mc.Mother_McEntry <= Common::DummyInt && IsReactionID(mc.StatusCode);
}

// The first generation of a reaction: logical primaries carrying their own reaction id;
// also demanding the pdg code to be among the channel's first gen. products.
inline bool IsGen1Signal(const POD::McParticle &mc, const DB::ReactionChannels::Definition &r_channel) {
    if (!IsGen1Signal(mc)) return false;
    return std::ranges::find(r_channel.products_pdg, mc.PdgCode) != r_channel.products_pdg.end();
}

inline Provenance Classify(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection) {
    // -- particle is first gen. signal
    if (IsGen1Signal(mc)) {
        return {static_cast<int>(mc.StatusCode), Generation::kFirstGen};
    }
    // -- check if particle has a mother
    if (mc.Mother_McEntry > Common::DummyInt) {
        const auto &mother = mc_collection[static_cast<std::size_t>(mc.Mother_McEntry)];
        // -- particle is second gen. signal
        if (IsGen1Signal(mother)) {
            return {static_cast<int>(mother.StatusCode), Generation::kSecondGen};
        }
    }
    // -- GEANT progeny; its reaction id would take a walk up the mother chain, not worth it for now
    return {Common::DummyInt, Generation::kGeantProgeny};
}

// Return the index/position in injected collection.
// Non-null when there both particles have the same valid reaction id (non-dummy, within range).
inline std::optional<std::size_t> FindCommonReactionID(const POD::Extended::McParticle &mc_dau1, const POD::Extended::McParticle &mc_dau2) {
    if (!SameSignalID(mc_dau1.SignalID, mc_dau2.SignalID)) return std::nullopt;
    if (!IsReactionID(static_cast<unsigned int>(mc_dau1.SignalID))) return std::nullopt;
    return InjectionIndex(static_cast<unsigned int>(mc_dau1.SignalID));
}

// Identify the injected reaction channel of a dedicated sexaquark MC production,
// by matching the first-gen products of a single reaction against `DB::ReactionChannels`.
// Every reaction of a production shares the same channel, so one of them is enough;
// This function is carried only in the first event and it's not tried again, because dedicated sexa MC have all events with injected signal.
// NOTE: could be generalized for more channels, but this crafty profile-based version is efficient enough.
inline DB::ReactionChannels::Definition DetectMcSignalChannel(const std::vector<POD::McParticle> &mc_particles) {

    DB::ReactionChannels::Definition default_reaction_channel = DB::ReactionChannels::ReactionChannel('0');

    int n_antilambda = 0;
    int n_k0s = 0;
    int n_kplus = 0;
    int n_antiproton = 0;
    int n_pi0 = 0;
    unsigned int n_total = 0;

    // collect all possible gen1 signal particles; grab their pdg codes
    for (const auto &mc : mc_particles) {
        if (!IsGen1Signal(mc)) continue;
        n_antilambda += mc.PdgCode == -3122;
        n_k0s += mc.PdgCode == 310;
        n_kplus += mc.PdgCode == 321;
        n_antiproton += mc.PdgCode == -2212;
        n_pi0 += mc.PdgCode == 111;
        n_total++;
    }
    if (n_total == 0) return default_reaction_channel;  // no signal found

    constexpr int n_reactions = static_cast<int>(E2T::NSexaReactionsPerEvent);

    DB::ReactionChannels::Definition test_reaction_channel = DB::ReactionChannels::ReactionChannel('A');
    if (n_total == test_reaction_channel.products_pdg.size() * E2T::NSexaReactionsPerEvent) {
        if (n_antilambda == n_reactions && n_k0s == n_reactions) return test_reaction_channel;
    }

    test_reaction_channel = DB::ReactionChannels::ReactionChannel('D');
    if (n_total == test_reaction_channel.products_pdg.size() * E2T::NSexaReactionsPerEvent) {
        if (n_antilambda == n_reactions && n_kplus == n_reactions) return test_reaction_channel;
    }

    test_reaction_channel = DB::ReactionChannels::ReactionChannel('H');
    if (n_total == test_reaction_channel.products_pdg.size() * E2T::NSexaReactionsPerEvent) {
        if (n_antiproton == n_reactions && n_pi0 == n_reactions && n_kplus == 2 * n_reactions) return test_reaction_channel;
    }

    return default_reaction_channel;
}

}  // namespace SexaquarkRules

// == (Anti)H-Dibaryon rules == //

namespace HdibaryonRules {

// Does it descend from an injected (anti)H?
// Valid because generator index is passed downstream.
inline bool IsFromSignalGenerator(const POD::McParticle &mc) {  //
    return mc.Generator == Common::ECustomGeneratorIdx::kInjectedHdibaryon;
}

// Quickly check if `status_code` is within [400,499]
inline bool IsInjectionID(unsigned int status_code) {
    return status_code >= E2T::InjectionID_Offset && status_code < E2T::InjectionID_Offset + E2T::NInjectedHdibaryonsPerEvent;
}

// (1) cannot have a mother; (2) valid injection ID; (3) pdg code matches (anti)h-dib.
// `Generator`-agnostic.
inline bool IsInjectedHdibaryon(const POD::McParticle &mc) {
    if (mc.Mother_McEntry > Common::DummyInt) return false;
    if (!IsInjectionID(mc.StatusCode)) return false;
    return mc.PdgCode == DB::Particles::Particle("Hdibaryon").pdg_code || mc.PdgCode == DB::Particles::Particle("AntiHdibaryon").pdg_code;
}

inline Provenance Classify(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection) {
    // -- it's an injected (anti)h-dibaryon
    if (IsInjectedHdibaryon(mc)) {
        return {static_cast<int>(mc.StatusCode), Generation::kInjected};
    }
    // -- check if particle has a mother
    if (mc.Mother_McEntry > Common::DummyInt) {
        const auto &mother = mc_collection[static_cast<std::size_t>(mc.Mother_McEntry)];
        // -- (anti)lambdas; their mother is an (anti)h-dibaryon
        if (IsInjectedHdibaryon(mother)) {
            return {static_cast<int>(mother.StatusCode), Generation::kFirstGen};
        }
        // -- if not, check if particle has a grandmother
        if (mother.Mother_McEntry > Common::DummyInt) {
            const auto &grandmother = mc_collection[static_cast<std::size_t>(mother.Mother_McEntry)];
            // -- (anti)protons and pions; their grandmother is an (anti)h-dibaryon
            if (IsInjectedHdibaryon(grandmother)) {
                return {static_cast<int>(grandmother.StatusCode), Generation::kSecondGen};
            }
        }
    }
    // -- GEANT progeny; its injection id would take a walk up the mother chain, not worth it for now
    return {Common::DummyInt, Generation::kGeantProgeny};
}

}  // namespace HdibaryonRules

// == Classification == //

// Both dedicated productions are told apart by the generator index; see `docs/MC_PRODUCTIONS.md`.
inline Provenance Classify(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection) {
    if (SexaquarkRules::IsFromSignalGenerator(mc)) return SexaquarkRules::Classify(mc, mc_collection);
    if (HdibaryonRules::IsFromSignalGenerator(mc)) return HdibaryonRules::Classify(mc, mc_collection);
    // -- HIJING, injected antineutrons, auxiliary primaries, and all of their progeny
    return {};
}

// Extend a single MC particle with everything reachable about it, under a PID hypothesis.
inline POD::Extended::McParticle Extend(std::size_t mc_entry, const std::vector<POD::McParticle> &mc_collection, int pdg_code_hypothesis) {

    const POD::McParticle &mc = mc_collection[mc_entry];
    POD::Extended::McParticle ext_mc{mc};
    ext_mc.McEntry = static_cast<int>(mc_entry);

    std::tie(ext_mc.Decay_X, ext_mc.Decay_Y, ext_mc.Decay_Z) = GetDecayVertex(mc, mc_collection);

    if (mc.Mother_McEntry > Common::DummyInt) {
        const auto &mother = mc_collection[static_cast<std::size_t>(mc.Mother_McEntry)];
        ext_mc.Mother_PdgCode = mother.PdgCode;
        if (mother.Mother_McEntry > Common::DummyInt) {
            ext_mc.GM_McEntry = mother.Mother_McEntry;
            ext_mc.GM_PdgCode = mc_collection[static_cast<std::size_t>(mother.Mother_McEntry)].PdgCode;
        }
    }

    const Provenance prov = Classify(mc, mc_collection);
    ext_mc.SignalID = prov.signal_id;
    ext_mc.SignalGeneration = prov.generation;

    ext_mc.IsTrue = mc.PdgCode == pdg_code_hypothesis;
    ext_mc.IsSecondary = mc.IsSecFromMat || mc.IsSecFromWeak || prov.generation >= Generation::kFirstGen;
    ext_mc.IsTrueSignal = ext_mc.IsTrue && IsValidSignalID(prov.signal_id) && IsRelevantGeneration(prov.generation);
    ext_mc.IsRealBkg = prov.generation == Generation::kNone;

    // -- a single particle is its own only leaf, so its mask is just its own bit
    assert(mc.Generator < Common::NCustomGenerators);
    ext_mc.GeneratorMask = static_cast<std::uint8_t>(1U << mc.Generator);

    return ext_mc;
}

// == Composites == //

// Link two already-extended constituents into the mc record of the composite they were assumed to come from.
inline POD::Extended::McParticle LinkComposite(const std::vector<POD::McParticle> &mc_collection, const POD::Extended::McParticle &c1,
                                               const POD::Extended::McParticle &c2, int pdg_code_hypothesis) {

    POD::Extended::McParticle composite;  // by default, initialized to dummy values: no mc particle describes this candidate

    // -- a common mother only describes the candidate if the constituents really are two different mc particles;
    //    split images of a single one do share a mother, but that mother would misdescribe the pair
    if (!SameMcParticle(c1, c2)) {
        if (auto entry_mother = FindMcEntry_CommonMother(c1, c2); entry_mother.has_value()) {
            composite = Extend(entry_mother.value(), mc_collection, pdg_code_hypothesis);
        }
    }

    // -- true signal: both constituents are true daughters of one same injection, and the mother is what the hypothesis assumed
    composite.IsTrueSignal = composite.IsTrue && c1.IsTrueSignal && c2.IsTrueSignal && SameSignalID(c1.SignalID, c2.SignalID);
    // -- real background: no constituent carries signal, at any depth
    composite.IsRealBkg = c1.IsRealBkg && c2.IsRealBkg;

    // -- `Extend` already set the mother's own bit when there was one, and left the mask empty when there wasn't
    composite.GeneratorMask = static_cast<std::uint8_t>(composite.GeneratorMask | c1.GeneratorMask | c2.GeneratorMask);

    return composite;
}

struct LinkedV0 {
    POD::Extended::McParticle v0;
    POD::Extended::McParticle neg;
    POD::Extended::McParticle pos;
};

// Extend both daughters of a V0 candidate under the hypothesis' own charged decay mode, then link them into the V0.
inline LinkedV0 LinkV0(const std::vector<POD::McParticle> &mc_collection, std::size_t entry_neg, std::size_t entry_pos,
                       const DB::Particles::Definition &pid_v0) {

    const int pdg_neg = pid_v0.idx_neg_dau.has_value() ? pid_v0.daughters_pdg[pid_v0.idx_neg_dau.value()] : Common::DummyNNN;
    const int pdg_pos = pid_v0.idx_pos_dau.has_value() ? pid_v0.daughters_pdg[pid_v0.idx_pos_dau.value()] : Common::DummyNNN;

    LinkedV0 linked;
    linked.neg = Extend(entry_neg, mc_collection, pdg_neg);
    linked.pos = Extend(entry_pos, mc_collection, pdg_pos);
    linked.v0 = LinkComposite(mc_collection, linked.neg, linked.pos, pid_v0.pdg_code);

    return linked;
}

}  // namespace MC
