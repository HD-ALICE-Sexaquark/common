#pragma once

namespace T2DS::Cuts {

// -- Verifier Mode -- //

namespace PreFoundLambda {
static constexpr double AbsMax_Pz = 3.;             // (GeV/c)
static constexpr double Max_Pt = 4.;                // (GeV/c)
static constexpr double Min_Pt = 0.75;              // (GeV/c)
static constexpr double AbsMax_Rapidity = 0.5;      //
static constexpr double Min_Mass = 1.09;            // (GeV/c^2)
static constexpr double Max_Mass = 1.14;            // (GeV/c^2)
static constexpr double Min_CPAwrtPV = 0.99;        //
static constexpr double AbsMin_ArmRadiusDev = 0.1;  //
static constexpr double Max_DCAwrtPV = 0.5;         // (cm)
static constexpr double Max_DCAbtwDaughters = 0.5;  // (cm)
static constexpr double Max_Chi2NDF = 5.;           //
// -- depend on (anti)protons
static constexpr double AbsMax_Pz_Proton = 2.5;  // (GeV/c)
static constexpr double Max_Pt_Proton = 3.5;     // (GeV/c)
static constexpr double Min_Pt_Proton = 0.75;    // (GeV/c)
// -- depend on pi(minus/plus)
static constexpr double AbsMax_Pz_Pion = 0.6;  // (GeV/c)
static constexpr double Max_Pt_Pion = 0.8;     // (GeV/c)
static constexpr double Min_Pt_Pion = 0.2;     // (GeV/c)
// -- depend on (anti)h-dibaryon decay vertex
static constexpr double Max_DecayLength = 75.;  // (cm)
static constexpr double Min_DecayLength = 7.5;  // (cm)
static constexpr double Min_CPAwrtDV = 0.99;    //
}  // namespace PreFoundLambda

namespace LambdaPair {
constexpr double AbsMax_Pz = 6.;         // (GeV/c)
constexpr double Max_Pt = 8.;            // (GeV/c)
constexpr double Min_Pt = 2.5;           // (GeV/c)
constexpr double Min_Mass = 2.22;        // (GeV/c^2)
constexpr double Max_Mass = 2.26;        // (GeV/c^2)
constexpr double AbsMax_Rapidity = 0.5;  //
constexpr double Max_DecayLength = 1.;   // (cm)
constexpr double Min_CPAwrtPV = 0.99;    //
constexpr double Max_DCAbtwDau = 0.5;    // (cm)
constexpr double Max_Chi2NDF = 2.5;      //
}  // namespace LambdaPair

}  // namespace T2DS::Cuts
