#pragma once

#include <cstddef>
#include <vector>

#include "Constants.hpp"
#include "POD_McParticle.hpp"

// Where an mc particle sits within the injection of a dedicated MC production it descends from, if any: one rule for
// E2T and T2DS. Needs `POD::McParticle` only. See `docs/MC_LABELS.md`.
namespace Common::McTruth {

// == Signal Generation == //

namespace Generation {
inline constexpr int kNone = Common::DummyInt;  // no injected ancestor
inline constexpr int kInjected = 0;             // the injected (anti)h-dibaryon itself / the reaction's initial state (antisexaquark, nucleon)
inline constexpr int kFirstGen = 1;             // reaction products / the (anti)h-dibaryon's (anti)lambdas
inline constexpr int kSecondGen = 2;            // their charged daughters, from the generator's weak decays
inline constexpr int kGeantProgeny = 3;         // anything GEANT made, at any depth: a transported product's daughters included
}  // namespace Generation

// PID-agnostic.
struct Provenance {
    int signal_id{Common::DummyInt};  // NOTE: stays dummy when unreachable, `generation` alone tells if there was an injected ancestor or not
    int generation{Generation::kNone};
};

inline bool IsValidSignalID(int id) { return id > Common::DummyInt; }

inline bool IsRelevantGeneration(int generation) { return generation >= Generation::kInjected && generation < Generation::kGeantProgeny; }

// A signal particle, whatever its pdg code: `IsTrueSignal` before asking for a PID hypothesis.
inline bool IsSignal(const Provenance &prov) { return IsValidSignalID(prov.signal_id) && IsRelevantGeneration(prov.generation); }

// For a descendant of an injection: whether the generator made it, decaying its mother. GEANT3 writes status code 0.
// NOTE: the status code, since AliStack drops `IsPhysPrimary` of a generator daughter whose own first daughter is a photon
inline bool IsGeneratorDecayProduct(const POD::McParticle &mc) { return mc.StatusCode == Common::GeneratorDecay_StatusCode; }

// The mc particle at `entry` of `mc_collection`, or none when `entry` is dummy or out of range.
inline const POD::McParticle *At(int entry, const std::vector<POD::McParticle> &mc_collection) {
    if (entry <= Common::DummyInt || static_cast<std::size_t>(entry) >= mc_collection.size()) return nullptr;
    return &mc_collection[static_cast<std::size_t>(entry)];
}

// == AntiSexaquark Rules == //

namespace SexaquarkRules {

// Does it descend from an injected antisexaquark-nucleon reaction?
// Valid because generator index is passed downstream.
inline bool IsFromSignalGenerator(const POD::McParticle &mc) {  //
    return mc.Generator == Common::ECustomGeneratorIdx::kInjectedAntiSexaquarkReaction;
}

// Quickly check if `status_code` is within [600,650)
inline bool IsReactionID(unsigned int status_code) {
    return status_code >= Common::ReactionID_Offset && status_code < Common::ReactionID_Offset + Common::NSexaReactionsPerEvent;
}

// (1) cannot have a mother; (2) valid reaction ID; (3) pdg code matches the antisexaquark.
// `Generator`-agnostic.
inline bool IsInjectedSexaquark(const POD::McParticle &mc) {
    if (mc.Mother_McEntry > Common::DummyInt) return false;
    if (!IsReactionID(mc.StatusCode)) return false;
    return mc.PdgCode == Common::PdgCode_AntiSexaquark;
}

// The other half of the reaction's initial state: no mother either, and the same reaction ID as its antisexaquark, which
// is the only link between them.
inline bool IsStruckNucleon(const POD::McParticle &mc) {
    if (!IsFromSignalGenerator(mc)) return false;
    if (mc.Mother_McEntry > Common::DummyInt) return false;
    if (!IsReactionID(mc.StatusCode)) return false;
    return mc.PdgCode != Common::PdgCode_AntiSexaquark;
}

// NOTE: shaped like `HdibaryonRules::Classify`; LHC23l1ab4 records the antisexaquark as the mother of the reaction's products.
inline Provenance Classify(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection) {
    // -- it's the reaction's initial state: the antisexaquark, or its struck nucleon
    if (IsInjectedSexaquark(mc) || IsStruckNucleon(mc)) {
        return {static_cast<int>(mc.StatusCode), Generation::kInjected};
    }
    // -- check if particle has a mother
    if (const auto *mother = At(mc.Mother_McEntry, mc_collection); mother != nullptr) {
        // -- the reaction's products; their mother is the antisexaquark
        if (IsInjectedSexaquark(*mother)) {
            return {static_cast<int>(mother->StatusCode), Generation::kFirstGen};
        }
        // -- if not, check if particle has a grandmother
        if (const auto *grandmother = At(mother->Mother_McEntry, mc_collection); grandmother != nullptr) {
            // -- the products' daughters from the generator's decays (e.g. the V0s' charged legs); their grandmother is the
            //    antisexaquark. GEANT3's progeny of a transported product (the K+ and anti-proton of D, X and Y) falls through
            if (IsInjectedSexaquark(*grandmother) && IsGeneratorDecayProduct(mc)) {
                return {static_cast<int>(grandmother->StatusCode), Generation::kSecondGen};
            }
        }
    }
    // -- GEANT progeny; its reaction id would take a walk up the mother chain, not worth it for now
    return {Common::DummyInt, Generation::kGeantProgeny};
}

}  // namespace SexaquarkRules

// == (Anti)H-Dibaryon rules == //

namespace HdibaryonRules {

// Does it descend from an injected (anti)H?
// Valid because generator index is passed downstream.
inline bool IsFromSignalGenerator(const POD::McParticle &mc) {  //
    return mc.Generator == Common::ECustomGeneratorIdx::kInjectedHdibaryon;
}

// Quickly check if `status_code` is within [400,450)
inline bool IsInjectionID(unsigned int status_code) {
    return status_code >= Common::InjectionID_Offset && status_code < Common::InjectionID_Offset + Common::NInjectedHdibaryonsPerEvent;
}

// (1) cannot have a mother; (2) valid injection ID; (3) pdg code matches (anti)h-dib.
// `Generator`-agnostic.
inline bool IsInjectedHdibaryon(const POD::McParticle &mc) {
    if (mc.Mother_McEntry > Common::DummyInt) return false;
    if (!IsInjectionID(mc.StatusCode)) return false;
    return mc.PdgCode == Common::PdgCode_Hdibaryon || mc.PdgCode == Common::PdgCode_AntiHdibaryon;
}

inline Provenance Classify(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection) {
    // -- it's an injected (anti)h-dibaryon
    if (IsInjectedHdibaryon(mc)) {
        return {static_cast<int>(mc.StatusCode), Generation::kInjected};
    }
    // -- check if particle has a mother
    if (const auto *mother = At(mc.Mother_McEntry, mc_collection); mother != nullptr) {
        // -- (anti)lambdas; their mother is an (anti)h-dibaryon
        if (IsInjectedHdibaryon(*mother)) {
            return {static_cast<int>(mother->StatusCode), Generation::kFirstGen};
        }
        // -- if not, check if particle has a grandmother
        if (const auto *grandmother = At(mother->Mother_McEntry, mc_collection); grandmother != nullptr) {
            // -- (anti)protons and pions from the generator's decays; their grandmother is an (anti)h-dibaryon
            if (IsInjectedHdibaryon(*grandmother) && IsGeneratorDecayProduct(mc)) {
                return {static_cast<int>(grandmother->StatusCode), Generation::kSecondGen};
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

}  // namespace Common::McTruth
