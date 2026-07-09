#pragma once

#include <cmath>

#include <Math/Point3D.h>
#include <Math/Vector4D.h>

#include "HD_Library.hpp"
#include "POD_InjectedHdib.hpp"

namespace Cached {

struct InjectedHdib : POD::InjectedHdib {

    InjectedHdib(const POD::InjectedHdib& hdib, const ROOT::Math::XYZPoint& ref)
        : POD::InjectedHdib(hdib),  //
          lv{hdib.Px, hdib.Py, hdib.Pz, hdib.Energy},
          lv_l1{Lambda1_Px, Lambda1_Py, Lambda1_Pz, Lambda1_Energy},
          lv_p1{Proton1_Px, Proton1_Py, Proton1_Pz, Proton1_Energy},
          //   lv_pi1{Pion1_Px, Pion1_Py, Pion1_Pz, Pion1_Energy}, // unused, for now
          lv_l2{Lambda2_Px, Lambda2_Py, Lambda2_Pz, Lambda2_Energy},
          lv_p2{Proton2_Px, Proton2_Py, Proton2_Pz, Proton2_Energy},
          //   lv_pi2{Pion2_Px, Pion2_Py, Pion2_Pz, Pion2_Energy}, // unused, for now
          dv{hdib.Decay_X, hdib.Decay_Y, hdib.Decay_Z},
          //   dv_l1{Lambda1_Decay_X, Lambda1_Decay_Y, Lambda1_Decay_Z}, // unused, for now
          //   dv_l2{Lambda2_Decay_X, Lambda2_Decay_Y, Lambda2_Decay_Z}, // unused, for now
          pv{ref} {
        HD::InfoCorrelation c_corr = HD::GetAngles(lv, lv_l1, lv_l2, lv_p1, lv_p2);
        cos_theta_pp = c_corr.cos_theta_pp;
        theta_pp = c_corr.theta_pp;
        cos_theta_star_l1 = c_corr.cos_theta_star_l1;
        cos_theta_star_l2 = c_corr.cos_theta_star_l2;
        cos_theta_star_p1 = c_corr.cos_theta_star_p1;
        cos_theta_star_p2 = c_corr.cos_theta_star_p2;
        q_rel = c_corr.q_rel;
    }

    // kinematics
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    // geometry
    [[nodiscard]] double Decay_Radius2D() const { return dv.Rho(); }
    [[nodiscard]] double Decay_Radius3D() const { return dv.R(); }
    [[nodiscard]] double DecayLength() const { return (dv - pv).R(); }
    // correlations
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
    ROOT::Math::PxPyPzEVector lv_p1;
    // ROOT::Math::PxPyPzEVector lv_pi1; // unused, for now
    ROOT::Math::PxPyPzEVector lv_l2;
    ROOT::Math::PxPyPzEVector lv_p2;
    // ROOT::Math::PxPyPzEVector lv_pi2; // unused, for now
    ROOT::Math::XYZPoint dv;  // decay vertex
    // ROOT::Math::XYZPoint dv_l1;  // decay vertex of lambda 1 // unused, for now
    // ROOT::Math::XYZPoint dv_l2;  // decay vertex of lambda 2 // unused, for now
    ROOT::Math::XYZPoint pv;  // primary vertex = origin, by construction
    double cos_theta_pp;
    double theta_pp;
    double cos_theta_star_l1;
    double cos_theta_star_l2;
    double cos_theta_star_p1;
    double cos_theta_star_p2;
    double q_rel;
};

}  // namespace Cached
