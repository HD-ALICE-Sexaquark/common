#pragma once

#include <cmath>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Constants.hpp"
#include "Math.hpp"
#include "POD_Track.hpp"
#include "POD_V0.hpp"

namespace Cached {

struct V0 : POD::V0 {

    V0(const POD::V0& v0, const POD::Track& neg, const POD::Track& pos, const ROOT::Math::XYZPoint& ref)  //
        : POD::V0(v0),
          Neg{&neg},
          Pos{&pos},
          lv{Px, Py, Pz, Energy},
          neg_mom{v0.Neg_PCAwrtV0_Px, v0.Neg_PCAwrtV0_Py, v0.Neg_PCAwrtV0_Pz},
          pos_mom{v0.Pos_PCAwrtV0_Px, v0.Pos_PCAwrtV0_Py, v0.Pos_PCAwrtV0_Pz},
          neg_pca_wrt_dv{v0.Neg_PCAwrtV0_X, v0.Neg_PCAwrtV0_Y, v0.Neg_PCAwrtV0_Z},
          pos_pca_wrt_dv{v0.Pos_PCAwrtV0_X, v0.Pos_PCAwrtV0_Y, v0.Pos_PCAwrtV0_Z},
          pv{ref},
          dv{Decay_X, Decay_Y, Decay_Z},
          pca_wrt_pv{Common::Math::FastPCA_LineVertex(lv.Vect(), dv, pv)},
          cpa_wrt_pv{Common::Math::CosinePointingAngle(lv.Vect(), dv, pv)},
          arm_alpha{Common::Math::ArmenterosAlpha(lv.Vect(), neg_mom, pos_mom).value_or(Common::DummyDouble)},
          arm_qt{Common::Math::ArmenterosQt(lv.Vect(), neg_mom)} {}

    // kinematics
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double ArmAlpha() const { return arm_alpha; }
    [[nodiscard]] double ArmQt() const { return arm_qt; }
    // geometry
    [[nodiscard]] double Decay_SquaredRadius2D() const { return dv.Perp2(); }
    [[nodiscard]] double Decay_SquaredRadius3D() const { return dv.Mag2(); }
    [[nodiscard]] double Decay_Radius2D() const { return dv.Rho(); }
    [[nodiscard]] double Decay_Radius3D() const { return dv.R(); }
    [[nodiscard]] double DCA_wrt_PV() const { return (pca_wrt_pv - pv).R(); }
    [[nodiscard]] double SquaredDCA_wrt_PV() const { return (pca_wrt_pv - pv).Mag2(); }
    [[nodiscard]] double DCA_btw_Daughters() const { return (neg_pca_wrt_dv - pos_pca_wrt_dv).R(); }
    // kinematics+geometry
    [[nodiscard]] double CPA_wrt_PV() const { return cpa_wrt_pv; }
    // negative daughter
    // -- kinematics
    [[nodiscard]] double Neg_PCAwrtV0_Pz() const { return neg_mom.Z(); }
    [[nodiscard]] double Neg_PCAwrtV0_Pt() const { return neg_mom.Rho(); }
    [[nodiscard]] double Neg_PCAwrtV0_P() const { return neg_mom.R(); }
    [[nodiscard]] double Neg_PCAwrtV0_Eta() const { return neg_mom.Eta(); }
    // -- geometry
    [[nodiscard]] double Neg_SquaredDCA_wrt_V0() const { return (neg_pca_wrt_dv - dv).Mag2(); }
    [[nodiscard]] double Neg_DCA_wrt_V0() const { return (neg_pca_wrt_dv - dv).R(); }
    // positive daughter
    // -- kinematics
    [[nodiscard]] double Pos_PCAwrtV0_Pz() const { return pos_mom.Z(); }
    [[nodiscard]] double Pos_PCAwrtV0_Pt() const { return pos_mom.Rho(); }
    [[nodiscard]] double Pos_PCAwrtV0_P() const { return pos_mom.R(); }
    [[nodiscard]] double Pos_PCAwrtV0_Eta() const { return pos_mom.Eta(); }
    // -- geometry
    [[nodiscard]] double Pos_SquaredDCA_wrt_V0() const { return (pos_pca_wrt_dv - dv).Mag2(); }
    [[nodiscard]] double Pos_DCA_wrt_V0() const { return (pos_pca_wrt_dv - dv).R(); }

    const POD::Track* Neg;
    const POD::Track* Pos;

   private:
    ROOT::Math::PxPyPzEVector lv;         // lorentz vector
    ROOT::Math::XYZVector neg_mom;        // momentum
    ROOT::Math::XYZVector pos_mom;        // momentum
    ROOT::Math::XYZPoint neg_pca_wrt_dv;  // PCA of neg. daughter w.r.t. v0's decay vertex
    ROOT::Math::XYZPoint pos_pca_wrt_dv;  // PCA of pos. daughter w.r.t. v0's decay vertex
    ROOT::Math::XYZPoint pv;              // primary vertex
    ROOT::Math::XYZPoint dv;              // decay vertex
    ROOT::Math::XYZPoint pca_wrt_pv;      // point of closest approach w.r.t. primary vertex
    double cpa_wrt_pv;
    double arm_alpha;
    double arm_qt;
};

}  // namespace Cached
