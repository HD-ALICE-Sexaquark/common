#pragma once

#include <cstdint>
#include <string_view>

#define E2T_VERBOSE 0
#define E2T_MC_EXTRA 1

namespace E2T {
inline constexpr const char* Name_OutputTree = "Events";

// number of events per file from dedicated MC (both), by construction
inline constexpr unsigned int NEventsInDedicatedMCFile = 10;
inline constexpr unsigned int NExpectedEventsInRealData = 400'000'000;

// (antisexaquark-dedicated MC only)
inline constexpr unsigned int ReactionID_Offset = 600;  // antisexaquark+nucleon reactions
inline constexpr unsigned int NSexaReactionsPerEvent = 20;
inline constexpr std::string_view SimLog_EventHeader = "I-AliGenCocktail::Generate: Generator 3: AliGenSexaquarkReaction";
inline constexpr std::string_view SimLog_ReactionMarker = "I-AliGenSexaquarkReaction::GenerateN:";
inline constexpr double InjectedAntiSexa_MinPt = 0.;        // (GeV/c)
inline constexpr double InjectedAntiSexa_MaxPt = 5.;        // (GeV/c)
inline constexpr double InjectedAntiSexa_MinRadius = 5.;    // (cm)
inline constexpr double InjectedAntiSexa_MaxRadius = 180.;  // (cm)

// (hdibaryon-dedicated MC only)
inline constexpr unsigned int InjectionID_Offset = 400;  // (anti)h-dibaryon injections
inline constexpr unsigned int NInjectedHdibaryonsPerEvent = 100;
}  // namespace E2T

namespace T2DS {
inline constexpr std::string_view Name_FoundSexaquarkRNT = "FoundSexaquark";
inline constexpr std::string_view Name_FoundHdibaryonRNT = "FoundHdibaryon";
}  // namespace T2DS

namespace Skimmer {
inline constexpr std::string_view Name_CachedSexaquarkRNT = "CachedSexaquark";
inline constexpr std::string_view Name_CachedHdibaryonRNT = "CachedHdibaryon";
}  // namespace Skimmer

namespace Common {

enum ECustomGeneratorIdx : std::uint8_t {
    kHijing,
    kInjectedAntiNeutron,
    kInjectedAntiSexaquarkReaction,
    kInjectedAuxCharged,
    kInjectedHdibaryon,
};
inline constexpr std::uint8_t NCustomGenerators = kInjectedHdibaryon + 1;

// Generator of origin, as a bitmask over `ECustomGeneratorIdx`: a candidate's mask is the OR of its charged leaves'.
// The three groups below are the ones a search cares about; `MC::Origin` (in `MC_Helpers.hpp`) is what asks about them,
// and its `EClass` is the separate, non-interchangeable type the groups collapse into. See `docs/MC_LABELS.md`.
namespace OriginGen {
inline constexpr std::uint8_t kNone = 0;
inline constexpr std::uint8_t kHijingBit = 1U << kHijing;
inline constexpr std::uint8_t kAntiNeutronBit = 1U << kInjectedAntiNeutron;
inline constexpr std::uint8_t kSexaReactionBit = 1U << kInjectedAntiSexaquarkReaction;
inline constexpr std::uint8_t kAuxChargedBit = 1U << kInjectedAuxCharged;
inline constexpr std::uint8_t kHdibaryonBit = 1U << kInjectedHdibaryon;
// -- the underlying event (H); empty in (anti)h-dibaryon mc, which carries no HIJING at all
inline constexpr std::uint8_t kHIJING = kHijingBit;
// -- the injected background (N)
inline constexpr std::uint8_t kInjectedBkg = kAntiNeutronBit | kAuxChargedBit;
// -- the injected signal (S)
inline constexpr std::uint8_t kSignal = kSexaReactionBit | kHdibaryonBit;
}  // namespace OriginGen

inline constexpr int NCovMatrixComponents_State3 = 6;
inline constexpr int NCovMatrixComponents_State6 = 21;
inline constexpr int NCovMatrixComponents_State7 = 28;
inline constexpr int NCovMatrixComponents_State8 = 36;
inline constexpr double PdgMass_Lambda = 1.115683;         // PDG mass of lambda (GeV/c^2)
inline constexpr double PdgMass_Neutron = 0.93956542;      // PDG mass of neutron (GeV/c^2)
inline constexpr double PdgMass_Proton = 0.93827208816;    // PDG mass of proton (GeV/c^2)
inline constexpr double PdgMass_Kaon = 0.493677;           // PDG mass of charged kaon (GeV/c^2)
inline constexpr double PdgMass_Pion = 0.13957039;         // PDG mass of charged pion (GeV/c^2)
inline constexpr double PdgMass_KaonZeroShort = 0.497611;  // PDG mass of kaon-zero-short (GeV/c^2)
inline constexpr double Kappa = 0.000299792458;            // (GeV/c) / (kG/cm)
inline constexpr double AbsAlmostZero = 1.E-8;
inline constexpr double BigNumber = 1.E8;
inline constexpr int DummyInt = -1;
inline constexpr char DummyChar = 0;
inline constexpr int DummyNNN = -999;
inline constexpr float DummyFloat = -999.;
inline constexpr double DummyDouble = -999.;
inline constexpr double SpeedOfLight = 2.99792458E10;  // (cm/s)
inline constexpr double HBarC = 197.3269804;           // (eV * nm) = (MeV * fm) = (GeV * pm)
inline constexpr double Convert_eV2GeV = 1.E-9;
inline constexpr double Convert_nm2cm = 1.E-7;

// clang-format off
enum ESigma : std::uint8_t {
    X2,
    XY,  Y2,
    XZ,  YZ,  Z2,
    XPx, YPx, ZPx, Px2,
    XPy, YPy, ZPy, PxPy, Py2,
    XPz, YPz, ZPz, PxPz, PyPz, Pz2,
    XE,  YE,  ZE,  PxE,  PyE,  PzE,  E2,
    XS,  YS,  ZS,  PxS,  PyS,  PzS,  SE,  S2
};
// clang-format on

}  // namespace Common
