#pragma once

#include <cstdint>
#include <string_view>

#define E2T_VERBOSE 0
#define E2T_MC_EXTRA 1

namespace E2T {

inline constexpr const char* Name_OutputTree = "Events";

// (both dedicated MC)
inline constexpr unsigned int NEventsInDedicatedMCFile = 10;

// (antisexaquark-dedicated MC)
inline constexpr unsigned int NSexaReactionsPerEvent = 20;
inline constexpr std::string_view SimLog_EventHeader = "I-AliGenCocktail::Generate: Generator 3: AliGenSexaquarkReaction";
inline constexpr std::string_view SimLog_ReactionMarker = "I-AliGenSexaquarkReaction::GenerateN:";

}  // namespace E2T

namespace Common {

enum ECustomGeneratorIdx : std::uint8_t {
    kHijing,
    kInjectedAntiNeutron,
    kInjectedAntiSexaquarkReaction,
    kInjectedAuxCharged,
    kInjectedHdibaryon,
};

inline constexpr int NCovMatrixComponents_State3 = 6;
inline constexpr int NCovMatrixComponents_State6 = 21;
inline constexpr int NCovMatrixComponents_State7 = 28;
inline constexpr int NCovMatrixComponents_State8 = 36;

inline constexpr double PdgMass_Deuteron = 1.875612945;    // PDG mass of deuteron (GeV/c^2)
inline constexpr double PdgMass_Electron = 0.000510999;    // PDG mass of electron (GeV/c^2)
inline constexpr double PdgMass_Lambda = 1.115683;         // PDG mass of lambda (GeV/c^2)
inline constexpr double PdgMass_Neutron = 0.93956542;      // PDG mass of neutron (GeV/c^2)
inline constexpr double PdgMass_Proton = 0.93827208816;    // PDG mass of proton (GeV/c^2)
inline constexpr double PdgMass_Kaon = 0.493677;           // PDG mass of charged kaon (GeV/c^2)
inline constexpr double PdgMass_Pion = 0.13957039;         // PDG mass of charged pion (GeV/c^2)
inline constexpr double PdgMass_KaonZeroShort = 0.497611;  // PDG mass of kaon-zero-short (GeV/c^2)

inline constexpr int DummyInt = -1;
inline constexpr char DummyChar = 0;
inline constexpr int DummyNNN = -999;
inline constexpr float DummyFloat = -999.;
inline constexpr double DummyDouble = -999.;

}  // namespace Common
