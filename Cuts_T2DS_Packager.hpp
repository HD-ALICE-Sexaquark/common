#pragma once

namespace T2DS::Cuts {

// -- Packager -- //

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

}  // namespace T2DS::Cuts
