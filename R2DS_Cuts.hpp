#pragma once

namespace R2DS::Cuts {

namespace Proton {
constexpr double AbsMax_NSigmasProton = 3.;
// constexpr double AbsMin_DCAxy = 30.;  // PENDING
}  // namespace Proton

namespace Kaon {
constexpr double AbsMax_NSigmasKaon = 3.;
}  // namespace Kaon

namespace Pion {
constexpr double AbsMax_NSigmasPion = 3.;
// constexpr double AbsMin_DCAxy = 30.;  // PENDING
}  // namespace Pion

namespace Lambda {
// kinematics //
// constexpr double Min_Pt = 1.25;    // (GeV/c)
constexpr double Min_Mass = 1.09;  // (GeV/c^2)
constexpr double Max_Mass = 1.14;  // (GeV/c^2)
constexpr double AbsMax_Rapidity = 1.;
constexpr double Min_CPAwrtPV = 0.3;
constexpr double Max_CPAwrtPV = 0.95;  // PENDING: I want 0.95!
constexpr double Min_DCAwrtPV = 40.;   // (cm) // PENDING: I want 45!
constexpr double AbsMax_ArmQtOverAlpha = 0.2;
// geometric //
constexpr double Min_Radius2D = 65.;   // (cm)
constexpr double Max_DCAnegV0 = 0.2;   // (cm)
constexpr double Max_DCAposV0 = 0.2;   // (cm)
constexpr double Max_DCAbtwDau = 0.4;  // (cm)
}  // namespace Lambda

namespace KaonZeroShort {
// kinematics //
// constexpr double Min_Pt = 0.75;    // (GeV/c)
constexpr double Min_Mass = 0.47;  // (GeV/c^2)
constexpr double Max_Mass = 0.53;  // (GeV/c^2)
constexpr double AbsMax_Rapidity = 1.;
constexpr double Min_CPAwrtPV = 0.1;
constexpr double Max_CPAwrtPV = 0.95;  // PENDING: I want 0.95!
constexpr double Min_DCAwrtPV = 40.;   // (cm) // PENDING: I want 45!
// geometric //
constexpr double Min_Radius2D = 50.;    // (cm)
constexpr double Max_DCAnegV0 = 0.2;    // (cm)
constexpr double Max_DCAposV0 = 0.2;    // (cm)
constexpr double Max_DCAbtwDau = 0.25;  // (cm)
}  // namespace KaonZeroShort

namespace ChannelA {
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

namespace LambdaPair {
// kinematics //
constexpr double Min_Mass = 0.;  // PENDING
constexpr double Max_Mass = 0.;  // PENDING
// geometry //
constexpr double Max_DCAbtwDau = 0.;  // PENDING

}  // namespace LambdaPair

}  // namespace R2DS::Cuts
