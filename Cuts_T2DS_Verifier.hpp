#pragma once

namespace T2DS::Cuts {

// -- Verifier Mode -- //

namespace PreFoundLambda {
// pre-seed cuts
constexpr double Max_TracksDeltaR = 0.02;     // PENDING: to tune
constexpr double Max_TracksRelDeltaP = 0.05;  // PENDING: to tune
// (anti)lambda candidates
constexpr double AbsMax_Pz = 3.;             // (GeV/c)
constexpr double Max_Pt = 4.;                // (GeV/c)
constexpr double Min_Pt = 0.75;              // (GeV/c)
constexpr double AbsMax_Rapidity = 0.8;      // signal region = 0.8; goal = 0.5
constexpr double Min_CPAwrtPV = 0.99;        //
constexpr double AbsMax_ArmRadiusDev = 0.1;  //
constexpr double Max_DCAwrtPV = 0.5;         // (cm)
constexpr double Max_DCAbtwDaughters = 0.5;  // (cm)
constexpr double Max_Chi2NDF = 5.;           //
// (anti)protons
constexpr double AbsMax_Pz_Proton = 2.5;  // (GeV/c)
constexpr double Max_Pt_Proton = 3.5;     // (GeV/c)
constexpr double Min_Pt_Proton = 0.75;    // (GeV/c)
// pi(minus/plus)
constexpr double AbsMax_Pz_Pion = 0.6;  // (GeV/c)
constexpr double Max_Pt_Pion = 0.8;     // (GeV/c)
constexpr double Min_Pt_Pion = 0.2;     // (GeV/c)
// related with (anti)h-dibaryon decay vertex
constexpr double Max_DecayLength = 75.;  // (cm)
constexpr double Min_DecayLength = 7.5;  // (cm)
constexpr double Min_CPAwrtDV = 0.99;    //
}  // namespace PreFoundLambda

namespace LambdaPair {
// pre-seed cuts
constexpr double Min_DistBtwLambdaDVs = 0.01;  // distance between (anti)lambdas decay vertices; PENDING: to tune
// (anti)h-dibaryon candidates
constexpr double AbsMax_Pz = 6.;         // (GeV/c)
constexpr double Max_Pt = 8.;            // (GeV/c)
constexpr double Min_Pt = 2.5;           // (GeV/c)
constexpr double Min_Mass = 2.22;        // (GeV/c^2)
constexpr double Max_Mass = 2.26;        // (GeV/c^2)
constexpr double AbsMax_Rapidity = 0.8;  // signal region = 0.8; goal = 0.5
constexpr double Max_DecayLength = 1.;   // (cm)
constexpr double Min_CPAwrtPV = 0.99;    //
constexpr double Max_DCAbtwDau = 0.5;    // (cm)
constexpr double Max_Chi2NDF = 2.5;      //
}  // namespace LambdaPair

}  // namespace T2DS::Cuts
