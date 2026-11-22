#pragma once

#include <cstdint>

#define E2T_VERBOSE 0

namespace E2T {

inline constexpr const char* Name_OutputTree = "Events";

// the stage bytes `Schema::Events::McParticle_{Track,PreFoundLambda}Stage`: the furthest bookkeeping bin + 1 (0 for none),
// and a flag set when only fake tracks, or v0s with a fake daughter (label < 0), reached it
inline constexpr std::uint8_t StageByte_Bins = 0x7F;
inline constexpr std::uint8_t StageByte_OnlyFakes = 0x80;

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

// the dedicated MC's injections, as recorded in the mc stack (`docs/MC_LABELS.md`)
// -- both productions, as AliStack records them: E2T rewrites it to the injection's id (`McTruth::RelabelInjections`)
inline constexpr unsigned int GeneratorDecay_StatusCode = 1;  // the generator's own decay products; GEANT3's progeny carries 0
// -- antisexaquark (LHC23l1ab4)
inline constexpr unsigned int ReactionID_Offset = 600;      // antisexaquark+nucleon reactions
inline constexpr unsigned int NSexaReactionsPerEvent = 50;  // reaction ids 600-649
inline constexpr int PdgCode_AntiSexaquark = -900000020;
// -- (anti)h-dibaryon (LHC26h2)
inline constexpr unsigned int InjectionID_Offset = 400;          // (anti)h-dibaryon injections
inline constexpr unsigned int NInjectedHdibaryonsPerEvent = 50;  // injection ids 400-449 (400-424 H, 425-449 anti-H)
inline constexpr int PdgCode_Hdibaryon = 1020000020;
inline constexpr int PdgCode_AntiHdibaryon = -1020000020;

inline constexpr int DummyInt = -1;
inline constexpr char DummyChar = 0;
inline constexpr int DummyNNN = -999;
inline constexpr float DummyFloat = -999.;
inline constexpr double DummyDouble = -999.;

}  // namespace Common
