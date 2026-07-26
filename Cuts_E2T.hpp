#pragma once

namespace E2T::Cuts {

namespace Track {
static constexpr double AbsMax_NSigmas_PID = 3.;
static constexpr double Min_Pt = 0.15;   // (GeV/c)
static constexpr double Max_Pt = 5.;     // (GeV/c)
static constexpr double AbsMax_Pz = 4.;  // (GeV/c)
static constexpr double AbsMax_Eta = 0.9;
static constexpr double Min_PreDCAxy = 10.;   // (cm)
static constexpr double Max_PreDCAxy = 100.;  // (cm)
static constexpr double Min_PreDCAz = 1.5;    // (cm)
static constexpr double Max_PreDCAz = 120.;   // (cm)
static constexpr double Min_TPC_NClusters = 0.;
static constexpr double Max_TPC_Chi2_NClusters = 4.;
}  // namespace Track

namespace PreFoundLambda {
static constexpr double AbsMax_DeltaInvariantMass = 0.025;  // (GeV/c^2)
static constexpr double AbsSlope_ArmQtOverArmAlpha = 0.2;
static constexpr double AbsMax_NSigmas_PID = 3.;
static constexpr double Max_DcaV0Daughters = 1.;  // (cm)
static constexpr double Min_Pt = 0.5;             // (GeV/c)
static constexpr double Max_Pt = 5.;              // (GeV/c)
static constexpr double Max_DCA_wrt_PV = 0.5;     // (cm)
static constexpr double Min_CPA_wrt_PV = 0.;
static constexpr double Min_Decay_Radius2D = 3.;  // (cm)
static constexpr double Min_Pt_Daughters = 0.1;   // (GeV/c)
static constexpr double Max_Pt_Daughters = 4.;    // (GeV/c)
static constexpr double AbsMax_Eta_Daughters = 1.;
}  // namespace PreFoundLambda

}  // namespace E2T::Cuts
