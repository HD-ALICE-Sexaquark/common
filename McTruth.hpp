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

inline bool SameSignalID(int a, int b) { return IsValidSignalID(a) && a == b; }

inline bool IsRelevantGeneration(int generation) { return generation >= Generation::kInjected && generation < Generation::kGeantProgeny; }

// A signal particle, whatever its pdg code: `IsTrueSignal` before asking for a PID hypothesis.
inline bool IsSignal(const Provenance &prov) { return IsValidSignalID(prov.signal_id) && IsRelevantGeneration(prov.generation); }

// == Labels == //

// Whose labels the injections' members carry; they differ at generation 2 only:
// - AliStack's, as E2T reads them from the stack: the generator's decay products carry status code 1
// - E2T's, as it writes them (`RelabelInjections`) and as everything downstream reads them: every member of an injection's
//   own decay tree carries the injection's id as its status code
// GEANT3's progeny carries status code 0 under both.
enum class ELabels { kAliStack, kE2T };

// For a grandchild of `injected`: whether it is a member of the injection's own decay tree -- a product of the generator's
// decay of a first gen. particle --, and not GEANT3's progeny.
inline bool IsSecondGenMember(const POD::McParticle &mc, const POD::McParticle &injected, ELabels labels) {
    if (labels == ELabels::kAliStack) return mc.StatusCode == Common::GeneratorDecay_StatusCode;
    return mc.StatusCode == injected.StatusCode;
}

// The mc particle at `entry` of `mc_collection`, or none when `entry` is dummy or out of range.
inline const POD::McParticle *At(int entry, const std::vector<POD::McParticle> &mc_collection) {
    if (entry <= Common::DummyInt || static_cast<std::size_t>(entry) >= mc_collection.size()) return nullptr;
    return &mc_collection[static_cast<std::size_t>(entry)];
}

// The entry of the common mother of the mc particles at `entry_1` and `entry_2`, or dummy when there is none.
// NOTE: they must be two different mc particles: split images of a single one do share a mother, but that mother would
//       misdescribe the pair
inline int FindMcEntry_CommonMother(int entry_1, int entry_2, const std::vector<POD::McParticle> &mc_collection) {
    if (entry_1 == entry_2) return Common::DummyInt;
    const auto *mc_1 = At(entry_1, mc_collection);
    const auto *mc_2 = At(entry_2, mc_collection);
    if (mc_1 == nullptr || mc_2 == nullptr) return Common::DummyInt;
    if (mc_1->Mother_McEntry != mc_2->Mother_McEntry) return Common::DummyInt;
    if (At(mc_1->Mother_McEntry, mc_collection) == nullptr) return Common::DummyInt;
    return mc_1->Mother_McEntry;
}

namespace Detail {

// Below the injections that `is_injected` tells apart: their first and second generations, or GEANT progeny. Generation 0
// is each production's own rule.
inline Provenance ClassifyDescendant(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection,
                                     bool (*is_injected)(const POD::McParticle &), ELabels labels) {
    // -- check if particle has a mother
    if (const auto *mother = At(mc.Mother_McEntry, mc_collection); mother != nullptr) {
        // -- the reaction's products / the (anti)h-dibaryon's (anti)lambdas; their mother is the injection
        if (is_injected(*mother)) {
            return {static_cast<int>(mother->StatusCode), Generation::kFirstGen};
        }
        // -- if not, check if particle has a grandmother
        if (const auto *grandmother = At(mother->Mother_McEntry, mc_collection); grandmother != nullptr) {
            // -- the first gen.'s daughters from the generator's decays (the V0s' charged legs, the (anti)lambdas' (anti)protons
            //    and pions); GEANT3's progeny of a transported product (the K+ and anti-proton of D, X and Y) falls through
            if (is_injected(*grandmother) && IsSecondGenMember(mc, *grandmother, labels)) {
                return {static_cast<int>(grandmother->StatusCode), Generation::kSecondGen};
            }
        }
    }
    // -- GEANT progeny; its signal id would take a walk up the mother chain, not worth it for now
    return {Common::DummyInt, Generation::kGeantProgeny};
}

// The flags of ALICE's definitions, mutually exclusive.
inline void SetPhysPrimary(POD::McParticle &mc) {
    mc.IsPhysPrimary = true;
    mc.IsSecFromMat = false;
    mc.IsSecFromWeak = false;
}
inline void SetSecFromWeak(POD::McParticle &mc) {
    mc.IsPhysPrimary = false;
    mc.IsSecFromMat = false;
    mc.IsSecFromWeak = true;
}

}  // namespace Detail

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

// NOTE: LHC23l1ab4 records the antisexaquark as the mother of the reaction's products.
inline Provenance Classify(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection, ELabels labels = ELabels::kE2T) {
    // -- it's the reaction's initial state: the antisexaquark, or its struck nucleon
    if (IsInjectedSexaquark(mc) || IsStruckNucleon(mc)) {
        return {static_cast<int>(mc.StatusCode), Generation::kInjected};
    }
    return Detail::ClassifyDescendant(mc, mc_collection, IsInjectedSexaquark, labels);
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

inline Provenance Classify(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection, ELabels labels = ELabels::kE2T) {
    // -- it's an injected (anti)h-dibaryon
    if (IsInjectedHdibaryon(mc)) {
        return {static_cast<int>(mc.StatusCode), Generation::kInjected};
    }
    return Detail::ClassifyDescendant(mc, mc_collection, IsInjectedHdibaryon, labels);
}

}  // namespace HdibaryonRules

// == Classification == //

// Both dedicated productions are told apart by the generator index; see `docs/MC_PRODUCTIONS.md`.
inline Provenance Classify(const POD::McParticle &mc, const std::vector<POD::McParticle> &mc_collection, ELabels labels = ELabels::kE2T) {
    if (SexaquarkRules::IsFromSignalGenerator(mc)) return SexaquarkRules::Classify(mc, mc_collection, labels);
    if (HdibaryonRules::IsFromSignalGenerator(mc)) return HdibaryonRules::Classify(mc, mc_collection, labels);
    // -- HIJING, injected antineutrons, auxiliary primaries, and all of their progeny
    return {};
}

// == E2T's Labels == //

// Rewrite in place AliStack's labels of the dedicated MC's injections, so that every member of
// an injection's own decay tree carries the injection's id as its status code, and the flags of ALICE's definitions:
// - in sexa MCs:
//   + the struck nucleon keeps AliStack's labels
//   + the injected antisexaquark: a physical primary
//   + the reaction's products keep AliStack's labels: secondaries from material
//   + the decay products of the first gen.: the injection's id as status code and marked as secondaries from weak decay
// - in hdib MCs:
//   + injected (anti)h-dibaryon: physical primary
//   + the (anti)h-dibaryon's (anti)lambdas, from its strong decay: physical primaries
//   + the decay products of the first gen.: the injection's id as status code and marked as secondaries from weak decay
// GEANT3's progeny, and every other generator, keep AliStack's labels.
inline void RelabelInjections(std::vector<POD::McParticle> &mc_collection) {
    // -- every particle classified under AliStack's labels first, so that none is classified after another one was rewritten
    std::vector<Provenance> provenances;
    provenances.reserve(mc_collection.size());
    for (const POD::McParticle &mc : mc_collection) provenances.push_back(Classify(mc, mc_collection, ELabels::kAliStack));

    for (std::size_t entry = 0; entry < mc_collection.size(); ++entry) {
        POD::McParticle &mc = mc_collection[entry];
        const Provenance &prov = provenances[entry];
        switch (prov.generation) {
            case Generation::kInjected:
                if (!SexaquarkRules::IsStruckNucleon(mc)) Detail::SetPhysPrimary(mc);
                break;
            case Generation::kFirstGen:
                if (HdibaryonRules::IsFromSignalGenerator(mc)) Detail::SetPhysPrimary(mc);
                break;
            case Generation::kSecondGen:
                mc.StatusCode = static_cast<unsigned int>(prov.signal_id);
                Detail::SetSecFromWeak(mc);
                break;
            default:  // no injected ancestor, or GEANT3's progeny
                break;
        }
    }
}

}  // namespace Common::McTruth
