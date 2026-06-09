#pragma once

#include <cmath>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Constants.hpp"
#include "Math.hpp"
#include "POD_PreFoundLambda.hpp"

namespace Cached {

struct PreFoundLambda : POD::PreFoundLambda {

    PreFoundLambda(const POD::PreFoundLambda& lambda, double mass_neg, double mass_pos, const ROOT::Math::XYZPoint& ref)  //
        : POD::PreFoundLambda(lambda),
          lv_neg{Neg_PCAwrtV0_Px, Neg_PCAwrtV0_Py, Neg_PCAwrtV0_Pz,
                 Common::Math::Hypot4(lambda.Neg_PCAwrtV0_Px, lambda.Neg_PCAwrtV0_Py, lambda.Neg_PCAwrtV0_Pz, mass_neg)},
          lv_pos{Pos_PCAwrtV0_Px, Pos_PCAwrtV0_Py, Pos_PCAwrtV0_Pz,
                 Common::Math::Hypot4(Pos_PCAwrtV0_Px, Pos_PCAwrtV0_Py, Pos_PCAwrtV0_Pz, mass_pos)},
          lv{lv_neg + lv_pos},
          dv{lambda.Decay_X, lambda.Decay_Y, lambda.Decay_Z},
          pv{ref},
          pca_wrt_pv{Common::Math::FastPCA_LineVertex(lv.Vect(), dv, pv)},
          cpa_wrt_pv{Common::Math::CosinePointingAngle(lv.Vect(), dv, pv)},
          arm_alpha{Common::Math::ArmenterosAlpha(lv.Vect(), lv_neg.Vect(), lv_pos.Vect()).value_or(Common::DummyFloat)},
          arm_qt{Common::Math::ArmenterosQt(lv.Vect(), lv_neg.Vect())} {}

    // kinematics
    [[nodiscard]] double Px() const { return lv.Px(); }
    [[nodiscard]] double Py() const { return lv.Py(); }
    [[nodiscard]] double Pz() const { return lv.Pz(); }
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    [[nodiscard]] double Energy() const { return lv.E(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    // -- correlations
    [[nodiscard]] double Proton_CosThetaStar() const {
        // angle between proton and their respective lambdas in the lambda r.f.
        auto boost_to_l1_rf = ROOT::Math::Boost(lv.BoostToCM());
        auto p1_in_l1_rf = boost_to_l1_rf(lv_neg.M() > lv_pos.M() ? lv_neg : lv_pos);  // PENDING: maybe could polish
        auto u_l1 = lv.Vect().Unit();
        auto u_p1 = p1_in_l1_rf.Vect().Unit();
        return u_p1.Dot(u_l1);
    }
    // -- negative daughter
    [[nodiscard]] double Neg_Pt() const { return lv_neg.Pt(); }
    [[nodiscard]] double Neg_P() const { return lv_neg.P(); }
    // -- positive daughter
    [[nodiscard]] double Pos_Pt() const { return lv_pos.Pt(); }
    [[nodiscard]] double Pos_P() const { return lv_pos.P(); }
    // -- armenteros-podolanski
    [[nodiscard]] double ArmAlpha() const { return arm_alpha; }
    [[nodiscard]] double ArmQt() const { return arm_qt; }
    // geometry
    [[nodiscard]] double Decay_Radius2D() const { return dv.Rho(); }
    [[nodiscard]] double Decay_Radius3D() const { return dv.R(); }
    [[nodiscard]] double DCAxy_wrt_PV() const { return (pca_wrt_pv - pv).Rho(); }
    [[nodiscard]] double DCAz_wrt_PV() const { return std::abs((pca_wrt_pv - pv).Z()); }
    [[nodiscard]] double DCA_wrt_PV() const { return (pca_wrt_pv - pv).R(); }
    [[nodiscard]] double CPA_wrt_PV() const { return cpa_wrt_pv; }

   private:
    ROOT::Math::PxPyPzEVector lv_neg;  // lorentz vector of negative daughter
    ROOT::Math::PxPyPzEVector lv_pos;  // lorentz vector of positive daughter
    ROOT::Math::PxPyPzEVector lv;      // lorentz vector of v0
    ROOT::Math::XYZPoint dv;           // decay vertex
    ROOT::Math::XYZPoint pv;           // primary vertex
    ROOT::Math::XYZPoint pca_wrt_pv;   // point of closest approach w.r.t. primary vertex
    double cpa_wrt_pv;                 // cosine pointing angle w.r.t. primary vertex
    double arm_alpha;                  // armenteros-podolanki's variable alpha
    double arm_qt;                     // armenteros-podolanki's variable q_t
};

}  // namespace Cached
