#pragma once

#include <cmath>

#include <Math/Point3Dfwd.h>
#include <Math/Vector4Dfwd.h>

#include "HD_Library.hpp"
#include "Math.hpp"
#include "POD_LambdaPair.hpp"
#include "POD_PreFoundLambda.hpp"

namespace Cached {

struct Hdibaryon : POD::LambdaPair {

    Hdibaryon(const POD::LambdaPair& hdib, const POD::Extended::PreFoundLambda& l1, const POD::Extended::PreFoundLambda& l2,
              const ROOT::Math::XYZPoint& ref)
        : POD::LambdaPair(hdib),
          // 4-momentum
          lv{hdib.Px, hdib.Py, hdib.Pz, hdib.Energy},
          // -- (anti)lambda 1
          lv_l1{l1.Px, l1.Py, l1.Pz, l1.Energy},
          lv_l1_neg{l1.Neg_PCAwrtV0_Px, l1.Neg_PCAwrtV0_Py, l1.Neg_PCAwrtV0_Pz, l1.Neg_Energy},
          lv_l1_pos{l1.Pos_PCAwrtV0_Px, l1.Pos_PCAwrtV0_Py, l1.Pos_PCAwrtV0_Pz, l1.Pos_Energy},
          // -- (anti)lambda 2
          lv_l2{l2.Px, l2.Py, l2.Pz, l2.Energy},
          lv_l2_neg{l2.Neg_PCAwrtV0_Px, l2.Neg_PCAwrtV0_Py, l2.Neg_PCAwrtV0_Pz, l2.Neg_Energy},
          lv_l2_pos{l2.Pos_PCAwrtV0_Px, l2.Pos_PCAwrtV0_Py, l2.Pos_PCAwrtV0_Pz, l2.Pos_Energy},
          // coordinates
          dv{hdib.Decay_X, hdib.Decay_Y, hdib.Decay_Z},
          pv{ref},
          pca_wrt_pv{Common::Math::FastPCA_LineVertex(lv.Vect(), dv, pv)},
          // -- (anti)lambda 1
          dv_l1{l1.Decay_X, l1.Decay_Y, l1.Decay_Z},
          l1_pca_wrt_pv{Common::Math::FastPCA_LineVertex(lv_l1.Vect(), dv_l1, pv)},
          l1_pca_wrt_dv{Lambda1_PCAwrtDV_X, Lambda1_PCAwrtDV_Y, Lambda1_PCAwrtDV_Z},
          l1_neg_pca_wrt_l1{l1.Neg_PCAwrtV0_X, l1.Neg_PCAwrtV0_Y, l1.Neg_PCAwrtV0_Z},
          l1_pos_pca_wrt_l1{l1.Pos_PCAwrtV0_X, l1.Pos_PCAwrtV0_Y, l1.Pos_PCAwrtV0_Z},
          // -- (anti)lambda 2
          dv_l2{l2.Decay_X, l2.Decay_Y, l2.Decay_Z},
          l2_pca_wrt_pv{Common::Math::FastPCA_LineVertex(lv.Vect(), dv, pv)},
          l2_pca_wrt_dv{Lambda2_PCAwrtDV_X, Lambda2_PCAwrtDV_Y, Lambda2_PCAwrtDV_Z},
          l2_neg_pca_wrt_l2{l2.Neg_PCAwrtV0_X, l2.Neg_PCAwrtV0_Y, l2.Neg_PCAwrtV0_Z},
          l2_pos_pca_wrt_l2{l2.Pos_PCAwrtV0_X, l2.Pos_PCAwrtV0_Y, l2.Pos_PCAwrtV0_Z},
          // scalars
          cpa_wrt_pv{Common::Math::CosinePointingAngle(lv.Vect(), dv, pv)},
          // -- (anti)lambda 1
          l1_cpa_wrt_pv{Common::Math::CosinePointingAngle(lv_l1.Vect(), dv_l1, pv)},
          l1_cpa_wrt_dv{Common::Math::CosinePointingAngle(lv_l1.Vect(), dv_l1, dv)},
          // -- (anti)lambda 2
          l2_cpa_wrt_pv{Common::Math::CosinePointingAngle(lv_l2.Vect(), dv_l2, pv)},
          l2_cpa_wrt_dv{Common::Math::CosinePointingAngle(lv_l2.Vect(), dv_l2, dv)},
          L1{&l1},
          L2{&l2} {
        // correlations
        HD::InfoCorrelation c_corr = HD::GetAngles(lv, lv_l1, lv_l2,                            //
                                                   hdib.IsAntiChannel ? lv_l1_neg : lv_l1_pos,  //
                                                   hdib.IsAntiChannel ? lv_l2_neg : lv_l2_pos);
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
    // (anti)lambda 1
    [[nodiscard]] double L1_Decay_Radius2D() const { return dv_l1.Rho(); }
    [[nodiscard]] double L1_Decay_Z() const { return L1->Decay_Z; }
    [[nodiscard]] double L1_CPA_wrt_PV() const { return l1_cpa_wrt_pv; }
    [[nodiscard]] double L1_DCA_wrt_PV() const { return (l1_pca_wrt_pv - dv).R(); }
    [[nodiscard]] double L1_CPA_wrt_DV() const { return l1_cpa_wrt_dv; }
    [[nodiscard]] double L1_DCA_wrt_DV() const { return (l1_pca_wrt_dv - dv).R(); }
    [[nodiscard]] double L1_DCA_btw_Daughters() const { return L1->DcaV0Daughters; }
    [[nodiscard]] double L1_Mass() const { return lv_l1.M(); }
    [[nodiscard]] double L1_Rapidity() const { return lv_l1.Rapidity(); }
    [[nodiscard]] double L1_Chi2NDF() const { return Chi2NDF; }
    // -- (anti)proton 1
    [[nodiscard]] double Pr1_NSigmasPion() const { return IsAntiChannel ? L1->Neg_NSigmasPion : L1->Pos_NSigmasPion; }
    [[nodiscard]] double Pr1_NSigmasKaon() const { return IsAntiChannel ? L1->Neg_NSigmasKaon : L1->Pos_NSigmasKaon; }
    [[nodiscard]] double Pr1_NSigmasProton() const { return IsAntiChannel ? L1->Neg_NSigmasProton : L1->Pos_NSigmasProton; }
    [[nodiscard]] double Pr1_DCA_wrt_L1() const { return std::abs(((IsAntiChannel ? l1_neg_pca_wrt_l1 : l1_pos_pca_wrt_l1) - dv_l1).R()); }
    [[nodiscard]] double Pr1_PreDCAxy() const { return IsAntiChannel ? L1->Neg_PreDCAxy : L1->Pos_PreDCAxy; }
    [[nodiscard]] double Pr1_PreDCAz() const { return IsAntiChannel ? L1->Neg_PreDCAz : L1->Pos_PreDCAz; }
    [[nodiscard]] double Pr1_Eta() const { return IsAntiChannel ? lv_l1_neg.Eta() : lv_l1_pos.Eta(); }
    [[nodiscard]] double Pr1_Pt() const { return IsAntiChannel ? lv_l1_neg.Pt() : lv_l1_pos.Pt(); }
    [[nodiscard]] double Pr1_Pz() const { return IsAntiChannel ? lv_l1_neg.Pz() : lv_l1_pos.Pz(); }
    // -- pion (minus/plus) 1
    [[nodiscard]] double Pi1_NSigmasPion() const { return IsAntiChannel ? L1->Pos_NSigmasPion : L1->Neg_NSigmasPion; }
    [[nodiscard]] double Pi1_NSigmasKaon() const { return IsAntiChannel ? L1->Pos_NSigmasKaon : L1->Neg_NSigmasKaon; }
    [[nodiscard]] double Pi1_NSigmasProton() const { return IsAntiChannel ? L1->Pos_NSigmasProton : L1->Neg_NSigmasProton; }
    [[nodiscard]] double Pi1_DCA_wrt_L1() const { return std::abs(((IsAntiChannel ? l1_pos_pca_wrt_l1 : l1_neg_pca_wrt_l1) - dv_l1).R()); }
    [[nodiscard]] double Pi1_PreDCAxy() const { return IsAntiChannel ? L1->Pos_PreDCAxy : L1->Neg_PreDCAxy; }
    [[nodiscard]] double Pi1_PreDCAz() const { return IsAntiChannel ? L1->Pos_PreDCAz : L1->Neg_PreDCAz; }
    [[nodiscard]] double Pi1_Eta() const { return IsAntiChannel ? lv_l1_pos.Eta() : lv_l1_neg.Eta(); }
    [[nodiscard]] double Pi1_Pt() const { return IsAntiChannel ? lv_l1_pos.Pt() : lv_l1_neg.Pt(); }
    [[nodiscard]] double Pi1_Pz() const { return IsAntiChannel ? lv_l1_pos.Pz() : lv_l1_neg.Pz(); }
    // (anti)lambda 2
    [[nodiscard]] double L2_Decay_Radius2D() const { return dv_l2.Rho(); }
    [[nodiscard]] double L2_Decay_Z() const { return L2->Decay_Z; }
    [[nodiscard]] double L2_CPA_wrt_PV() const { return l2_cpa_wrt_pv; }
    [[nodiscard]] double L2_DCA_wrt_PV() const { return (l2_pca_wrt_pv - dv).R(); }
    [[nodiscard]] double L2_CPA_wrt_DV() const { return l2_cpa_wrt_dv; }
    [[nodiscard]] double L2_DCA_wrt_DV() const { return (l2_pca_wrt_dv - dv).R(); }
    [[nodiscard]] double L2_DCA_btw_Daughters() const { return L2->DcaV0Daughters; }
    [[nodiscard]] double L2_Mass() const { return lv_l2.M(); }
    [[nodiscard]] double L2_Rapidity() const { return lv_l2.Rapidity(); }
    [[nodiscard]] double L2_Chi2NDF() const { return Chi2NDF; }
    // -- (anti)proton 2
    [[nodiscard]] double Pr2_NSigmasPion() const { return IsAntiChannel ? L2->Neg_NSigmasPion : L2->Pos_NSigmasPion; }
    [[nodiscard]] double Pr2_NSigmasKaon() const { return IsAntiChannel ? L2->Neg_NSigmasKaon : L2->Pos_NSigmasKaon; }
    [[nodiscard]] double Pr2_NSigmasProton() const { return IsAntiChannel ? L2->Neg_NSigmasProton : L2->Pos_NSigmasProton; }
    [[nodiscard]] double Pr2_DCA_wrt_L2() const { return std::abs(((IsAntiChannel ? l2_neg_pca_wrt_l2 : l2_pos_pca_wrt_l2) - dv_l2).R()); }
    [[nodiscard]] double Pr2_PreDCAxy() const { return IsAntiChannel ? L2->Neg_PreDCAxy : L2->Pos_PreDCAxy; }
    [[nodiscard]] double Pr2_PreDCAz() const { return IsAntiChannel ? L2->Neg_PreDCAz : L2->Pos_PreDCAz; }
    [[nodiscard]] double Pr2_Eta() const { return IsAntiChannel ? lv_l2_neg.Eta() : lv_l2_pos.Eta(); }
    [[nodiscard]] double Pr2_Pt() const { return IsAntiChannel ? lv_l2_neg.Pt() : lv_l2_pos.Pt(); }
    [[nodiscard]] double Pr2_Pz() const { return IsAntiChannel ? lv_l2_neg.Pz() : lv_l2_pos.Pz(); }
    // -- pion (minus/plus) 2
    [[nodiscard]] double Pi2_NSigmasPion() const { return IsAntiChannel ? L2->Pos_NSigmasPion : L2->Neg_NSigmasPion; }
    [[nodiscard]] double Pi2_NSigmasKaon() const { return IsAntiChannel ? L2->Pos_NSigmasKaon : L2->Neg_NSigmasKaon; }
    [[nodiscard]] double Pi2_NSigmasProton() const { return IsAntiChannel ? L2->Pos_NSigmasProton : L2->Neg_NSigmasProton; }
    [[nodiscard]] double Pi2_DCA_wrt_L2() const { return std::abs(((IsAntiChannel ? l2_pos_pca_wrt_l2 : l2_neg_pca_wrt_l2) - dv_l2).R()); }
    [[nodiscard]] double Pi2_PreDCAxy() const { return IsAntiChannel ? L2->Pos_PreDCAxy : L2->Neg_PreDCAxy; }
    [[nodiscard]] double Pi2_PreDCAz() const { return IsAntiChannel ? L2->Pos_PreDCAz : L2->Neg_PreDCAz; }
    [[nodiscard]] double Pi2_Eta() const { return IsAntiChannel ? lv_l2_pos.Eta() : lv_l2_neg.Eta(); }
    [[nodiscard]] double Pi2_Pt() const { return IsAntiChannel ? lv_l2_pos.Pt() : lv_l2_neg.Pt(); }
    [[nodiscard]] double Pi2_Pz() const { return IsAntiChannel ? lv_l2_pos.Pz() : lv_l2_neg.Pz(); }
    // correlations
    [[nodiscard]] double CosTheta_pp() const { return cos_theta_pp; }
    [[nodiscard]] double Theta_pp() const { return theta_pp; }
    [[nodiscard]] double CosThetaStar_L1() const { return cos_theta_star_l1; }
    [[nodiscard]] double CosThetaStar_L2() const { return cos_theta_star_l2; }
    [[nodiscard]] double CosThetaStar_Pr1() const { return cos_theta_star_p1; }
    [[nodiscard]] double CosThetaStar_Pr2() const { return cos_theta_star_p2; }
    [[nodiscard]] double Qrel() const { return q_rel; }

   private:
    // 4-momentum
    ROOT::Math::PxPyPzEVector lv;
    // -- (anti)lambda 1
    ROOT::Math::PxPyPzEVector lv_l1;
    ROOT::Math::PxPyPzEVector lv_l1_neg;
    ROOT::Math::PxPyPzEVector lv_l1_pos;
    // -- (anti)lambda 2
    ROOT::Math::PxPyPzEVector lv_l2;
    ROOT::Math::PxPyPzEVector lv_l2_neg;
    ROOT::Math::PxPyPzEVector lv_l2_pos;
    // coordinates
    ROOT::Math::XYZPoint dv;  // decay vertex
    ROOT::Math::XYZPoint pv;
    ROOT::Math::XYZPoint pca_wrt_pv;  // ~ origin
    // -- (anti)lambda 1
    ROOT::Math::XYZPoint dv_l1;
    ROOT::Math::XYZPoint l1_pca_wrt_pv;
    ROOT::Math::XYZPoint l1_pca_wrt_dv;
    ROOT::Math::XYZPoint l1_neg_pca_wrt_l1;
    ROOT::Math::XYZPoint l1_pos_pca_wrt_l1;
    // -- (anti)lambda 2
    ROOT::Math::XYZPoint dv_l2;
    ROOT::Math::XYZPoint l2_pca_wrt_pv;
    ROOT::Math::XYZPoint l2_pca_wrt_dv;
    ROOT::Math::XYZPoint l2_neg_pca_wrt_l2;
    ROOT::Math::XYZPoint l2_pos_pca_wrt_l2;
    // scalars
    double dca_btw_lambdas;
    double cpa_wrt_pv;
    // -- (anti)lambda 1
    double l1_cpa_wrt_pv;
    double l1_cpa_wrt_dv;
    // -- (anti)lambda 2
    double l2_cpa_wrt_pv;
    double l2_cpa_wrt_dv;
    // correlations
    double cos_theta_pp;
    double theta_pp;
    double cos_theta_star_l1;
    double cos_theta_star_l2;
    double cos_theta_star_p1;
    double cos_theta_star_p2;
    double q_rel;
    // pointers
    const POD::Extended::PreFoundLambda* L1;
    const POD::Extended::PreFoundLambda* L2;
};

}  // namespace Cached
