#pragma once

#include <cmath>

#include <Math/Point3D.h>
#include <Math/Vector4D.h>

#include "HD_Library.hpp"
#include "Math.hpp"
#include "POD_InjectedHdib.hpp"

namespace Cached {

struct InjectedHdib : POD::InjectedHdib {

    InjectedHdib(const POD::InjectedHdib& hdib, const ROOT::Math::XYZPoint& ref)
        : POD::InjectedHdib(hdib),  //
          lv{hdib.Px, hdib.Py, hdib.Pz, hdib.Energy},
          lv_l1{Lambda1_Px, Lambda1_Py, Lambda1_Pz, Lambda1_Energy},
          lv_pr1{Proton1_Px, Proton1_Py, Proton1_Pz, Proton1_Energy},
          lv_pi1{Pion1_Px, Pion1_Py, Pion1_Pz, Pion1_Energy},
          lv_l2{Lambda2_Px, Lambda2_Py, Lambda2_Pz, Lambda2_Energy},
          lv_pr2{Proton2_Px, Proton2_Py, Proton2_Pz, Proton2_Energy},
          lv_pi2{Pion2_Px, Pion2_Py, Pion2_Pz, Pion2_Energy},
          dv{hdib.Decay_X, hdib.Decay_Y, hdib.Decay_Z},
          dv_l1{Lambda1_Decay_X, Lambda1_Decay_Y, Lambda1_Decay_Z},
          dv_l2{Lambda2_Decay_X, Lambda2_Decay_Y, Lambda2_Decay_Z},
          pv{ref},
          cpa_wrt_pv{Common::Math::CosinePointingAngle(lv.Vect(), dv, pv)} {
        // (anti)lambda 1 //
        const auto& lv_neg1 = IsAntiChannel ? lv_pr1 : lv_pi1;
        const auto& lv_pos1 = IsAntiChannel ? lv_pi1 : lv_pr1;
        cpa_wrt_pv_l1 = Common::Math::CosinePointingAngle(lv_l1.Vect(), dv_l1, pv);
        cpa_wrt_dv_l1 = Common::Math::CosinePointingAngle(lv_l1.Vect(), dv_l1, dv);
        arm_qt_l1 = Common::Math::ArmenterosQt(lv_l1.Vect(), lv_neg1.Vect());
        arm_alpha_l1 = Common::Math::ArmenterosAlpha(lv_l1.Vect(), lv_neg1.Vect(), lv_pos1.Vect()).value_or(Common::DummyDouble);
        // (anti)lambda 2 //
        const auto& lv_neg2 = IsAntiChannel ? lv_pr2 : lv_pi2;
        const auto& lv_pos2 = IsAntiChannel ? lv_pi2 : lv_pr2;
        cpa_wrt_pv_l2 = Common::Math::CosinePointingAngle(lv_l2.Vect(), dv_l2, pv);
        cpa_wrt_dv_l2 = Common::Math::CosinePointingAngle(lv_l2.Vect(), dv_l2, dv);
        arm_qt_l2 = Common::Math::ArmenterosQt(lv_l2.Vect(), lv_neg2.Vect());
        arm_alpha_l2 = Common::Math::ArmenterosAlpha(lv_l2.Vect(), lv_neg2.Vect(), lv_pos2.Vect()).value_or(Common::DummyDouble);
        // correlations //
        HD::InfoCorrelation c_corr = HD::GetAngles(lv, lv_l1, lv_l2, lv_pr1, lv_pr2);
        cos_theta_pp = c_corr.cos_theta_pp;
        theta_pp = c_corr.theta_pp;
        cos_theta_star_l1 = c_corr.cos_theta_star_l1;
        cos_theta_star_l2 = c_corr.cos_theta_star_l2;
        cos_theta_star_p1 = c_corr.cos_theta_star_p1;
        cos_theta_star_p2 = c_corr.cos_theta_star_p2;
        q_rel = c_corr.q_rel;
    }

    // (anti)h-dibaryon //
    // -- kinematics
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Energy() const { return lv.E(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    // -- geometry
    [[nodiscard]] double Origin_Radius2D() const { return pv.Rho(); }
    [[nodiscard]] double Origin_Radius3D() const { return pv.R(); }
    [[nodiscard]] double Decay_Radius2D() const { return dv.Rho(); }
    [[nodiscard]] double Decay_Radius3D() const { return dv.R(); }
    [[nodiscard]] double DecayLength() const { return (dv - pv).R(); }
    // -- kinematics + geometry
    [[nodiscard]] double CPA_wrt_PV() const { return cpa_wrt_pv; }

    // (anti)lambda 1 //
    [[nodiscard]] double Lambda1_Pt() const { return lv_l1.Pt(); }
    [[nodiscard]] double Lambda1_P() const { return lv_l1.P(); }
    [[nodiscard]] double Lambda1_Mass() const { return lv_l1.M(); }
    [[nodiscard]] double Lambda1_Rapidity() const { return lv_l1.Rapidity(); }
    [[nodiscard]] double Lambda1_CPA_wrt_PV() const { return cpa_wrt_pv_l1; }
    // -- related to (anti)h-dibaryon
    [[nodiscard]] double Lambda1_DecayLength() const { return (dv - dv_l1).R(); }
    [[nodiscard]] double Lambda1_CPA_wrt_DV() const { return cpa_wrt_dv_l1; }
    // -- related to daughters
    [[nodiscard]] double Lambda1_ArmQt() const { return arm_qt_l1; }
    [[nodiscard]] double Lambda1_ArmAlpha() const { return arm_alpha_l1; }

    // (anti)lambda 2 //
    [[nodiscard]] double Lambda2_Pt() const { return lv_l2.Pt(); }
    [[nodiscard]] double Lambda2_P() const { return lv_l2.P(); }
    [[nodiscard]] double Lambda2_Mass() const { return lv_l2.M(); }
    [[nodiscard]] double Lambda2_Rapidity() const { return lv_l2.Rapidity(); }
    [[nodiscard]] double Lambda2_CPA_wrt_PV() const { return cpa_wrt_pv_l2; }
    // -- related to (anti)h-dibaryon
    [[nodiscard]] double Lambda2_DecayLength() const { return (dv - dv_l2).R(); }
    [[nodiscard]] double Lambda2_CPA_wrt_DV() const { return cpa_wrt_dv_l2; }
    // -- related to daughters
    [[nodiscard]] double Lambda2_ArmQt() const { return arm_qt_l2; }
    [[nodiscard]] double Lambda2_ArmAlpha() const { return arm_alpha_l2; }

    // (anti)proton 1 //
    [[nodiscard]] double Proton1_Pt() const { return lv_pr1.Pt(); }
    [[nodiscard]] double Proton1_Eta() const { return lv_pr1.Eta(); }

    // pion (minus/plus) 1 //
    [[nodiscard]] double Pion1_Pt() const { return lv_pi1.Pt(); }
    [[nodiscard]] double Pion1_Eta() const { return lv_pi1.Eta(); }

    // (anti)proton 2 //
    [[nodiscard]] double Proton2_Pt() const { return lv_pr2.Pt(); }
    [[nodiscard]] double Proton2_Eta() const { return lv_pr2.Eta(); }

    // pion (minus/plus) 2 //
    [[nodiscard]] double Pion2_Pt() const { return lv_pi2.Pt(); }
    [[nodiscard]] double Pion2_Eta() const { return lv_pi2.Eta(); }

    // correlations //
    [[nodiscard]] double CosTheta_pp() const { return cos_theta_pp; }
    [[nodiscard]] double Theta_pp() const { return theta_pp; }
    [[nodiscard]] double CosThetaStar_L1() const { return cos_theta_star_l1; }
    [[nodiscard]] double CosThetaStar_L2() const { return cos_theta_star_l2; }
    [[nodiscard]] double CosThetaStar_Pr1() const { return cos_theta_star_p1; }
    [[nodiscard]] double CosThetaStar_Pr2() const { return cos_theta_star_p2; }
    [[nodiscard]] double Qrel() const { return q_rel; }

   private:
    ROOT::Math::PxPyPzEVector lv;
    ROOT::Math::PxPyPzEVector lv_l1;
    ROOT::Math::PxPyPzEVector lv_pr1;
    ROOT::Math::PxPyPzEVector lv_pi1;
    ROOT::Math::PxPyPzEVector lv_l2;
    ROOT::Math::PxPyPzEVector lv_pr2;
    ROOT::Math::PxPyPzEVector lv_pi2;
    ROOT::Math::XYZPoint dv;     // decay vertex
    ROOT::Math::XYZPoint dv_l1;  // decay vertex of lambda 1
    ROOT::Math::XYZPoint dv_l2;  // decay vertex of lambda 2
    ROOT::Math::XYZPoint pv;     // primary vertex = origin, by construction
    double cpa_wrt_pv;
    double cpa_wrt_pv_l1;
    double cpa_wrt_pv_l2;
    double cpa_wrt_dv_l1;
    double cpa_wrt_dv_l2;
    double arm_qt_l1;
    double arm_alpha_l1;
    double arm_qt_l2;
    double arm_alpha_l2;
    double cos_theta_pp;
    double theta_pp;
    double cos_theta_star_l1;
    double cos_theta_star_l2;
    double cos_theta_star_p1;
    double cos_theta_star_p2;
    double q_rel;
};

}  // namespace Cached
