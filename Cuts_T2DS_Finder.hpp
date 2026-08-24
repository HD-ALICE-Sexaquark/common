#pragma once

namespace T2DS::Cuts {

// -- Finder Mode -- //

namespace Track {
// tracking can split one physical particle into two ESD entries; these tell them apart
constexpr double Max_TracksDeltaR = 0.02;     // PENDING: to tune
constexpr double Max_TracksRelDeltaP = 0.05;  // PENDING: to tune
}  // namespace Track

namespace Proton {
constexpr double AbsMax_NSigmasProton = 3.;  // mandatory for classification
// constexpr double AbsMin_DCAxy = 30.;  // PENDING
}  // namespace Proton

namespace Kaon {
constexpr double AbsMax_NSigmasKaon = 3.;  // mandatory for classification
}  // namespace Kaon

namespace Pion {
constexpr double AbsMax_NSigmasPion = 3.;  // mandatory for classification
// constexpr double AbsMin_DCAxy = 30.;  // PENDING
}  // namespace Pion

namespace Lambda {
// kinematics //
constexpr double Min_Pt = 0.75;    // (GeV/c)
constexpr double Min_Mass = 1.09;  // (GeV/c^2)
constexpr double Max_Mass = 1.14;  // (GeV/c^2)
constexpr double AbsMax_Rapidity = 1.;
constexpr double Min_CPAwrtPV = -1.;
constexpr double Max_CPAwrtPV = 0.95;  // PENDING: I want 0.95!
constexpr double Min_DCAwrtPV = 5.;    // (cm) // PENDING: I want 45!
constexpr double AbsMax_ArmQtOverAlpha = 0.2;
// geometric //
constexpr double Min_Decay_Radius2D = 25.;  // (cm)
constexpr double Max_DCAnegV0 = 2.;         // (cm)
constexpr double Max_DCAposV0 = 2.;         // (cm)
constexpr double Max_DCAbtwDau = 2.;        // (cm)
}  // namespace Lambda

namespace KaonZeroShort {
// kinematics //
constexpr double Min_Pt = 0.75;    // (GeV/c)
constexpr double Min_Mass = 0.44;  // (GeV/c^2)
constexpr double Max_Mass = 0.56;  // (GeV/c^2)
constexpr double AbsMax_Rapidity = 1.;
constexpr double Min_CPAwrtPV = -1.;
constexpr double Max_CPAwrtPV = 0.95;  // PENDING: I want 0.95!
constexpr double Min_DCAwrtPV = 5.;    // (cm) // PENDING: I want 45!
// geometric //
constexpr double Min_Decay_Radius2D = 25.;  // (cm)
constexpr double Max_DCAnegV0 = 1.;         // (cm)
constexpr double Max_DCAposV0 = 1.;         // (cm)
constexpr double Max_DCAbtwDau = 1.;        // (cm)
}  // namespace KaonZeroShort

namespace ChannelA {
// pre-seed cuts
constexpr double Min_DistBtwV0sDVs = 0.01;  // distance between V0s decay vertices; PENDING: to tune
// kinematics //
// constexpr double Min_CPAwrtPV{0.9};
// constexpr double Max_DCAwrtPV = 15.;  // (cm)
// geometry //
constexpr double Min_Radius2D = 40.;   // (cm)
constexpr double Max_DCALaSV = 10.;    // (cm)
constexpr double Max_DCAK0SV = 10.;    // (cm)
constexpr double Max_DCAbtwV0s = 10.;  // (cm)
// constexpr double Min_La_CPAwrtSV = 0.5;   // PENDING
// constexpr double Min_K0S_CPAwrtSV = 0.5;  // PENDING
// constexpr double Max_DecayLengthLa = 100.; // PENDING
// constexpr double Max_DecayLengthK0 = 100.; // PENDING
}  // namespace ChannelA

namespace ChannelD {
// kinematics //
constexpr double AbsMax_Rapidity = 0.7;
constexpr double Min_CPAwrtPV = 0.9;
constexpr double Max_CPAwrtPV = 1.;
// geometry //
constexpr double Min_Radius2D = 20.;    // (cm)
constexpr double Max_Radius2D = 180.;   // (cm)
constexpr double Max_DCALaSV = 10.;     // (cm)
constexpr double Max_DCALaNegSV = 10.;  // (cm)
constexpr double Max_DCALaPosSV = 10.;  // (cm)
constexpr double Max_DCAKaSV = 10.;     // (cm)
constexpr double Max_DCAKaLa = 10.;     // (cm)
constexpr double Max_DCALaNegKa = 10.;  // (cm)
constexpr double Max_DCALaPosKa = 10.;  // (cm)
}  // namespace ChannelD

namespace ChannelH {
// kinematics //

// geometry //

}  // namespace ChannelH

}  // namespace T2DS::Cuts
