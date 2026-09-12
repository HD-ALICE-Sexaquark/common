#pragma once

// # Signal Region
// ## PreFoundLambda
//    |pz| < 3 GeV/c
//    pt < 4 GeV/c
//    |y| < 0.8
// ## (Anti)Protons
//    |pz| < 2.5 GeV/c
//    pt < 3.5 GeV
// ## Pi(Minus/Plus)
//    |pz| < 0.6 GeV/c
//    pt < 0.8 GeV
// ## (Anti)H-dibaryon
//    |pz| < 6 GeV/c
//    pt < 8 GeV
//    |y| < 0.8

namespace T2DS::Cuts {

// -- Verifier Mode -- //

namespace PreFoundLambda {
// pre-seed cuts
// constexpr double Max_TracksDeltaR = 0.02;     // PENDING: to tune
// constexpr double Max_TracksRelDeltaP = 0.05;  // PENDING: to tune
// post-seed/pre-fit cuts
constexpr double Max_DCAbtwDaughters = 10.;  //
constexpr double Min_PreFit_Mass = 1.09;     //
constexpr double Max_PreFit_Mass = 1.12;     //
constexpr double Min_PreFit_Radius2D = 0.;   //
constexpr double Max_PreFit_Radius2D = 75;   //
// the rest...
constexpr double AbsMax_Pz = 3.;  // (GeV/c)
constexpr double Max_Pt = 4.;     // (GeV/c)
// constexpr double Min_Pt = 0.75;          // (GeV/c)
constexpr double AbsMax_Rapidity = 0.8;  //  = 0.8; goal = 0.5
// constexpr double Min_CPAwrtPV = 0.99;        //
// constexpr double AbsMax_ArmRadiusDev = 0.1;  //
// constexpr double Max_DCAwrtPV = 0.5;         // (cm)
// constexpr double Max_DCAbtwDaughters = 0.5;  // (cm)
// constexpr double Max_Chi2NDF = 5.;           //
// (anti)protons //
constexpr double AbsMax_Pz_Proton = 2.5;  // (GeV/c)
constexpr double Max_Pt_Proton = 3.5;     // (GeV/c)
// constexpr double Min_Pt_Proton = 0.75;  // (GeV/c)
// pi(minus/plus) //
constexpr double AbsMax_Pz_Pion = 0.6;  // (GeV/c)
constexpr double Max_Pt_Pion = 0.8;     // (GeV/c)
// constexpr double Min_Pt_Pion = 0.2;     // (GeV/c)
// related with (anti)h-dibaryon decay vertex
// constexpr double Max_DecayLength = 75.;  // (cm)
// constexpr double Min_DecayLength = 7.5;  // (cm)
// constexpr double Min_CPAwrtDV = 0.99;    //
}  // namespace PreFoundLambda

namespace LambdaPair {
// pre-seed cuts
// constexpr double Min_DistBtwLambdaDVs = 0.01;  // distance between (anti)lambdas decay vertices; PENDING: to tune
constexpr double Min_PreSeed_Mass = 2.2;  // (GeV/c^2)
constexpr double Max_PreSeed_Mass = 2.4;  // (GeV/c^2)
// post-seed/pre-fit cuts
constexpr double Max_DCAbtwLambdas = 10.;    // (cm)
constexpr double Max_PreFit_Radius2D = 10.;  // (cm)
// the rest...
constexpr double AbsMax_Pz = 6.;  // (GeV/c)
constexpr double Max_Pt = 8.;     // (GeV/c)
// constexpr double Min_Pt = 2.5;           // (GeV/c)
// constexpr double Min_Mass = 2.22;        // (GeV/c^2)
constexpr double Max_Mass = 2.24;  // (GeV/c^2)
// constexpr double AbsMax_Rapidity = 0.8;  // signal region = 0.8; goal = 0.5
// constexpr double Max_DecayLength = 1.;   // (cm)
// constexpr double Min_CPAwrtPV = 0.99;    //
// constexpr double Max_DCAbtwDau = 0.5;    // (cm)
// constexpr double Max_Chi2NDF = 2.5;      //
}  // namespace LambdaPair

}  // namespace T2DS::Cuts
