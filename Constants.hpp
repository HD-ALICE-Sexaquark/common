#pragma once

#include <cstddef>
#include <string_view>

#define E2R_VERBOSE 1
#define E2R_MC_EXTRA 1
#define E2R_TPC_EXTRA 1

namespace E2R {
static constexpr int ReactionID_Offset = 600;
static constexpr int NEventsInDedicatedMC = 10;  // number of events per file from dedicated MC
static constexpr int NReactionsPerEvent = 20;    // number of injected reactions per event in dedicated MC
static constexpr std::string_view SimLog_EventHeader = "I-AliGenCocktail::Generate: Generator 3: AliGenSexaquarkReaction";
static constexpr std::string_view SimLog_ReactionMarker = "I-AliGenSexaquarkReaction::GenerateN:";

// (sexaquark-dedicated MC only)
enum EGenerator { kHijing, kInjectedAntiNeutron, kInjectedAntiSexaquarkReaction };

static constexpr std::string_view Name_OutputRNT = "Events";
}  // namespace E2R

namespace R2DS {
static constexpr const char* Name_PackedRNT = "PackedEvents";
static constexpr const char* Name_InjectedSexaRNT = "InjectedSexa";
static constexpr const char* Name_LambdaPairRNT = "LambdaPair";
static constexpr const char* Name_InjectedHdibRNT = "InjectedHdib";

}  // namespace R2DS

namespace Common {
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
enum ESigma : std::size_t {
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
