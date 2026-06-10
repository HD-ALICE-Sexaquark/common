#pragma once

#include <cmath>

#include <Math/Point3Dfwd.h>
#include <Math/Vector4Dfwd.h>

#include "HD_Library.hpp"
#include "Math.hpp"
#include "POD_LambdaPair.hpp"
#include "POD_PreFoundLambda.hpp"

namespace Cached {

struct LambdaPair : POD::LambdaPair {

    LambdaPair(const POD::LambdaPair& hdib, const POD::Extended::PreFoundLambda& l1, const POD::Extended::PreFoundLambda& l2,
               const ROOT::Math::XYZPoint& ref)
        : POD::LambdaPair(hdib),  //
          lv{hdib.Px, hdib.Py, hdib.Pz, hdib.Energy},
          dv{hdib.Decay_X, hdib.Decay_Y, hdib.Decay_Z},
          pv{ref} {
        pca_wrt_pv = Common::Math::FastPCA_LineVertex(lv.Vect(), dv, pv);
        dca_btw_lambdas = Common::Math::Distance(ROOT::Math::XYZPoint{Lambda1_PCAwrtDV_X, Lambda1_PCAwrtDV_Y, Lambda1_PCAwrtDV_Z},
                                                 ROOT::Math::XYZPoint{Lambda2_PCAwrtDV_X, Lambda2_PCAwrtDV_Y, Lambda2_PCAwrtDV_Z});
        cpa_wrt_pv = Common::Math::CosinePointingAngle(lv.Vect(), dv, pv);
        // correlations
        ROOT::Math::PxPyPzEVector lv_l1(l1.Px, l1.Py, l1.Pz, l1.Energy);
        ROOT::Math::PxPyPzEVector lv_l2(l2.Px, l2.Py, l2.Pz, l2.Energy);
        ROOT::Math::PxPyPzEVector lv_p1;
        ROOT::Math::PxPyPzEVector lv_p2;
        if (hdib.AntiChannel) {
            lv_p1.SetCoordinates(l1.Neg_PCAwrtV0_Px, l1.Neg_PCAwrtV0_Py, l1.Neg_PCAwrtV0_Pz, l1.Neg_Energy);
            lv_p2.SetCoordinates(l2.Neg_PCAwrtV0_Px, l2.Neg_PCAwrtV0_Py, l2.Neg_PCAwrtV0_Pz, l2.Neg_Energy);
        } else {
            lv_p1.SetCoordinates(l1.Pos_PCAwrtV0_Px, l1.Pos_PCAwrtV0_Py, l1.Pos_PCAwrtV0_Pz, l1.Pos_Energy);
            lv_p2.SetCoordinates(l2.Pos_PCAwrtV0_Px, l2.Pos_PCAwrtV0_Py, l2.Pos_PCAwrtV0_Pz, l2.Pos_Energy);
        }
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
    [[nodiscard]] double Origin_X() const { return pca_wrt_pv.X(); }
    [[nodiscard]] double Origin_Y() const { return pca_wrt_pv.Y(); }
    [[nodiscard]] double Origin_Z() const { return pca_wrt_pv.Z(); }
    [[nodiscard]] double Origin_Radius2D() const { return pca_wrt_pv.Rho(); }
    [[nodiscard]] double Origin_Radius3D() const { return pca_wrt_pv.R(); }
    [[nodiscard]] double Decay_X() const { return dv.X(); }
    [[nodiscard]] double Decay_Y() const { return dv.Y(); }
    [[nodiscard]] double Decay_Z() const { return dv.Z(); }
    [[nodiscard]] double Decay_Radius2D() const { return dv.Rho(); }
    [[nodiscard]] double Decay_Radius3D() const { return dv.R(); }
    [[nodiscard]] double DecayLength() const { return (pca_wrt_pv - dv).R(); }
    [[nodiscard]] double DCAxy_wrt_PV() const { return (pca_wrt_pv - pv).Rho(); }
    [[nodiscard]] double DCAz_wrt_PV() const { return std::abs((pca_wrt_pv - pv).Z()); }
    [[nodiscard]] double DCA_btw_Lambdas() const { return dca_btw_lambdas; }
    [[nodiscard]] double CPA_wrt_PV() const { return cpa_wrt_pv; }
    // correlations
    [[nodiscard]] double CosTheta_pp() const { return cos_theta_pp; }
    [[nodiscard]] double Theta_pp() const { return theta_pp; }
    [[nodiscard]] double CosThetaStar_l1() const { return cos_theta_star_l1; }
    [[nodiscard]] double CosThetaStar_l2() const { return cos_theta_star_l2; }
    [[nodiscard]] double CosThetaStar_p1() const { return cos_theta_star_p1; }
    [[nodiscard]] double CosThetaStar_p2() const { return cos_theta_star_p2; }
    [[nodiscard]] double Qrel() const { return q_rel; }

   private:
    ROOT::Math::PxPyPzEVector lv;
    ROOT::Math::XYZPoint dv;  // decay vertex
    ROOT::Math::XYZPoint pv;
    ROOT::Math::XYZPoint pca_wrt_pv;  // ~ origin
    double dca_btw_lambdas;
    double cpa_wrt_pv;
    double cos_theta_pp;
    double theta_pp;
    double cos_theta_star_l1;
    double cos_theta_star_l2;
    double cos_theta_star_p1;
    double cos_theta_star_p2;
    double q_rel;
};

}  // namespace Cached
