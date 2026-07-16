#pragma once

namespace T2DS::Cuts {

// -- Verifier Mode -- //

namespace PreFoundLambda {
static constexpr double AbsMax_NSigmasProton = 3.;
static constexpr double AbsMax_NSigmasPion = 3.;
static constexpr double AbsMax_Rapidity = 0.8;
static constexpr double Min_Pt_Proton = 0.5;  // (GeV/c)
}  // namespace PreFoundLambda

namespace LambdaPair {
// kinematics //
constexpr double Min_Mass = 0.;  // PENDING
constexpr double Max_Mass = 0.;  // PENDING
// geometry //
constexpr double Max_DCAbtwDau = 0.;  // PENDING

}  // namespace LambdaPair

}  // namespace T2DS::Cuts
