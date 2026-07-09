#pragma once

#include <cstdint>
#include <string_view>

#define E2T_VERBOSE 0
#define E2T_MC_EXTRA 1
#define E2T_TPC_EXTRA 1

namespace E2T {
static constexpr const char* Name_OutputTree = "Events";

// number of events per file from dedicated MC (both), by construction
static constexpr unsigned int NEventsInDedicatedMC = 10;

// (anti-sexaquark-dedicated MC only)
static constexpr unsigned int ReactionID_Offset = 600;   // (anti)sexaquark-nucleon reactions
static constexpr unsigned int InjectionID_Offset = 400;  // h-dibaryon injections
static constexpr unsigned int NSexaReactionsPerEvent = 20;
static constexpr std::string_view SimLog_EventHeader = "I-AliGenCocktail::Generate: Generator 3: AliGenSexaquarkReaction";
static constexpr std::string_view SimLog_ReactionMarker = "I-AliGenSexaquarkReaction::GenerateN:";
enum EGenerator : std::uint8_t { kHijing, kInjectedAntiNeutron, kInjectedAntiSexaquarkReaction };

// (hdibaryon-dedicated MC only)
static constexpr std::uint8_t GeneratorIdx_InjectedHdibaryon = 6;
}  // namespace E2T

namespace T2DS {
static constexpr const char* Name_PackedRNT = "PackedEvents";
static constexpr const char* Name_FoundHdibaryonRNT = "FoundHdibaryon";
}  // namespace T2DS

namespace Common {
static constexpr int NCovMatrixComponents_State3 = 6;
static constexpr int NCovMatrixComponents_State6 = 21;
static constexpr int NCovMatrixComponents_State7 = 28;
static constexpr int NCovMatrixComponents_State8 = 36;
static constexpr double PdgMass_Lambda = 1.115683;  // PDG mass of lambda (GeV/c^2)
static constexpr double PdgMass_Proton = 0.938272;  // PDG mass of proton (GeV/c^2)
static constexpr double PdgMass_Kaon = 0.493677;    // PDG mass of kaon (GeV/c^2)
static constexpr double PdgMass_Pion = 0.139570;    // PDG mass of pion (GeV/c^2)
static constexpr double Kappa = 0.000299792458;     // (GeV/c) / (kG/cm)
static constexpr double AbsAlmostZero = 1.E-8;
static constexpr double BigNumber = 1.E8;
static constexpr int DummyInt = -1;
static constexpr char DummyChar = static_cast<char>(false);
static constexpr int DummyNNN = -999;
static constexpr float DummyFloat = -999.;
static constexpr double DummyDouble = -999.;
static constexpr double SpeedOfLight = 2.99792458E10;  // (cm/s)
static constexpr double HBarC = 197.3269804;           // (eV * nm) = (MeV * fm) = (GeV * pm)
static constexpr double Convert_eV2GeV = 1.E-9;
static constexpr double Convert_nm2cm = 1.E-7;

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
