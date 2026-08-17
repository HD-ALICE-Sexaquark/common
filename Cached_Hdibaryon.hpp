#pragma once

#include <cmath>

#include <Math/Point3Dfwd.h>
#include <Math/Vector4Dfwd.h>

#include "Constants.hpp"
#include "DB_Particles.hpp"
#include "HD_Library.hpp"
#include "Math.hpp"
#include "POD_LambdaPair.hpp"
#include "POD_PreFoundLambda.hpp"

namespace Cached {

struct Hdibaryon : POD::LambdaPair {

    Hdibaryon(const POD::LambdaPair& hdib, const POD::Extended::PreFoundLambda& l1, const POD::Extended::PreFoundLambda& l2,
              const ROOT::Math::XYZPoint& ref)
        : POD::LambdaPair(hdib),
          // 4-momentum (closed tree)
          lv{static_cast<double>(hdib.Px), static_cast<double>(hdib.Py), static_cast<double>(hdib.Pz), static_cast<double>(hdib.Energy)},
          // 4-momentum (constrained pv)
          lv_cv{static_cast<double>(hdib.CV_Px), static_cast<double>(hdib.CV_Py), static_cast<double>(hdib.CV_Pz),
                static_cast<double>(hdib.CV_Energy)},
          // coordinates
          dv{hdib.Decay_X, hdib.Decay_Y, hdib.Decay_Z},
          cv{hdib.CV_X, hdib.CV_Y, hdib.CV_Z},
          pv{ref},
          pca_wrt_pv{Common::Math::FastPCA_LineVertex(lv.Vect(), dv, pv)},
          // -- (anti)lambda 1
          dv_l1{l1.Decay_X, l1.Decay_Y, l1.Decay_Z},
          l1_pca_wrt_dv{Lambda1_PCAwrtDV_X, Lambda1_PCAwrtDV_Y, Lambda1_PCAwrtDV_Z},
          // -- (anti)lambda 2
          dv_l2{l2.Decay_X, l2.Decay_Y, l2.Decay_Z},
          l2_pca_wrt_dv{Lambda2_PCAwrtDV_X, Lambda2_PCAwrtDV_Y, Lambda2_PCAwrtDV_Z},
          // scalars
          cpa_wrt_pv{Common::Math::CosinePointingAngle(lv.Vect(), dv, pv)} {
        // auxiliary vectors (closed tree)
        ROOT::Math::PxPyPzEVector lv_l1{Lambda1_Fit_Px, Lambda1_Fit_Py, Lambda1_Fit_Pz, Lambda1_Fit_Energy};
        ROOT::Math::PxPyPzEVector lv_l1_neg{l1.Neg_Fit_Px, l1.Neg_Fit_Py, l1.Neg_Fit_Pz, l1.Neg_Fit_Energy};
        ROOT::Math::PxPyPzEVector lv_l1_pos{l1.Pos_Fit_Px, l1.Pos_Fit_Py, l1.Pos_Fit_Pz, l1.Pos_Fit_Energy};
        ROOT::Math::PxPyPzEVector lv_l2{Lambda2_Fit_Px, Lambda2_Fit_Py, Lambda2_Fit_Pz, Lambda2_Fit_Energy};
        ROOT::Math::PxPyPzEVector lv_l2_neg{l2.Neg_Fit_Px, l2.Neg_Fit_Py, l2.Neg_Fit_Pz, l2.Neg_Fit_Energy};
        ROOT::Math::PxPyPzEVector lv_l2_pos{l2.Pos_Fit_Px, l2.Pos_Fit_Py, l2.Pos_Fit_Pz, l2.Pos_Fit_Energy};
        // reattach grand-daughters to (anti)h-dibaryon fit (ensure closed tree)
        const ROOT::Math::PxPyPzEVector lv_l1_prefit{l1.Px, l1.Py, l1.Pz, l1.Energy};
        const ROOT::Math::PxPyPzEVector lv_l2_prefit{l2.Px, l2.Py, l2.Pz, l2.Energy};
        const double mass_neg = hdib.IsAntiChannel ? DB::Particles::Particle("AntiProton").mass : DB::Particles::Particle("PiMinus").mass;
        const double mass_pos = hdib.IsAntiChannel ? DB::Particles::Particle("PiPlus").mass : DB::Particles::Particle("Proton").mass;
        Common::Math::ReattachTo(lv_l1_neg, lv_l1_pos, mass_neg, mass_pos, lv_l1_prefit, lv_l1);
        Common::Math::ReattachTo(lv_l2_neg, lv_l2_pos, mass_neg, mass_pos, lv_l2_prefit, lv_l2);
        // (anti)lambda 1
        l1_cpa_wrt_dv = Common::Math::CosinePointingAngle(lv_l1.Vect(), dv_l1, dv);
        // (anti)lambda 2
        l2_cpa_wrt_dv = Common::Math::CosinePointingAngle(lv_l2.Vect(), dv_l2, dv);
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

    // (anti)h-dibaryon candidate //
    // -- kinematics
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    // -- decay vertex
    [[nodiscard]] double Decay_X() const { return dv.X(); }
    [[nodiscard]] double Decay_Y() const { return dv.Y(); }
    [[nodiscard]] double Decay_Z() const { return dv.Z(); }
    [[nodiscard]] double Decay_Radius2D() const { return dv.Rho(); }
    [[nodiscard]] double Decay_Radius3D() const { return dv.R(); }
    [[nodiscard]] double DecayLength() const { return (pca_wrt_pv - dv).R(); }  // legacy, distance between PCAwrtPV and DV
    [[nodiscard]] double DCA_btw_Lambdas() const { return (l1_pca_wrt_dv - l2_pca_wrt_dv).R(); }
    // -- origin ~ pca w.r.t. pv
    [[nodiscard]] double PCAwrtPV_X() const { return pca_wrt_pv.X(); }
    [[nodiscard]] double PCAwrtPV_Y() const { return pca_wrt_pv.Y(); }
    [[nodiscard]] double PCAwrtPV_Z() const { return pca_wrt_pv.Z(); }
    [[nodiscard]] double PCAwrtPV_Radius2D() const { return pca_wrt_pv.Rho(); }
    [[nodiscard]] double PCAwrtPV_Radius3D() const { return pca_wrt_pv.R(); }
    // -- kinematics + geometry (only make sense when PV constraint is turned off)
    [[nodiscard]] double DCAxy_wrt_PV() const { return (pca_wrt_pv - pv).Rho(); }
    [[nodiscard]] double DCAz_wrt_PV() const { return std::abs((pca_wrt_pv - pv).Z()); }
    [[nodiscard]] double DCA_wrt_PV() const { return (pca_wrt_pv - pv).R(); }
    [[nodiscard]] double CPA_wrt_PV() const { return cpa_wrt_pv; }

    // available with PV constraint //
    [[nodiscard]] bool HasCV() const { return static_cast<double>(Chi2CV) > 0.; }  // a chi2 is never negative, the dummy is
    // -- kinematics
    [[nodiscard]] double CV_Pt() const { return HasCV() ? lv_cv.Pt() : Common::DummyDouble; }
    [[nodiscard]] double CV_P() const { return HasCV() ? lv_cv.P() : Common::DummyDouble; }
    [[nodiscard]] double CV_Mass() const { return HasCV() ? lv_cv.M() : Common::DummyDouble; }
    [[nodiscard]] double CV_Eta() const { return HasCV() ? lv_cv.Eta() : Common::DummyDouble; }
    [[nodiscard]] double CV_Rapidity() const { return HasCV() ? lv_cv.Rapidity() : Common::DummyDouble; }
    [[nodiscard]] double CV_Phi() const { return HasCV() ? lv_cv.Phi() : Common::DummyDouble; }
    // -- constrained production vertex ~ primary vertex
    [[nodiscard]] double CV_Radius2D() const { return HasCV() ? cv.Rho() : Common::DummyDouble; }
    [[nodiscard]] double CV_Radius3D() const { return HasCV() ? cv.R() : Common::DummyDouble; }
    // -- decay length
    [[nodiscard]] double CV_DecayLength_DivByErr() const {
        auto err = static_cast<double>(CV_DecayLengthErr);
        if (err > 0.) return static_cast<double>(CV_DecayLength) / err;
        return Common::DummyDouble;
    }

    // (anti)lambda 1 //
    // -- related to (anti)h-dibaryon
    [[nodiscard]] double L1_DecayLength() const { return (dv_l1 - dv).R(); }
    [[nodiscard]] double L1_CPA_wrt_DV() const { return l1_cpa_wrt_dv; }
    [[nodiscard]] double L1_DCA_wrt_DV() const { return (l1_pca_wrt_dv - dv).R(); }

    // (anti)lambda 2 //
    // -- related to (anti)h-dibaryon
    [[nodiscard]] double L2_DecayLength() const { return (dv_l2 - dv).R(); }
    [[nodiscard]] double L2_CPA_wrt_DV() const { return l2_cpa_wrt_dv; }
    [[nodiscard]] double L2_DCA_wrt_DV() const { return (l2_pca_wrt_dv - dv).R(); }

    // correlations //
    [[nodiscard]] double CosTheta_pp() const { return cos_theta_pp; }
    [[nodiscard]] double Theta_pp() const { return theta_pp; }
    [[nodiscard]] double CosThetaStar_L1() const { return cos_theta_star_l1; }
    [[nodiscard]] double CosThetaStar_L2() const { return cos_theta_star_l2; }
    [[nodiscard]] double CosThetaStar_Pr1() const { return cos_theta_star_p1; }
    [[nodiscard]] double CosThetaStar_Pr2() const { return cos_theta_star_p2; }
    [[nodiscard]] double Qrel() const { return q_rel; }

   private:
    // 4-momentum
    ROOT::Math::PxPyPzEVector lv;     // kinematically closed w.r.t. decay tree
    ROOT::Math::PxPyPzEVector lv_cv;  // constrained to production vertex
    // coordinates
    ROOT::Math::XYZPoint dv;  // decay vertex
    ROOT::Math::XYZPoint cv;  // constrained production vertex ~ primary vertex
    ROOT::Math::XYZPoint pv;
    ROOT::Math::XYZPoint pca_wrt_pv;  // ~ origin
    // -- (anti)lambda 1
    ROOT::Math::XYZPoint dv_l1;
    ROOT::Math::XYZPoint l1_pca_wrt_dv;
    // -- (anti)lambda 2
    ROOT::Math::XYZPoint dv_l2;
    ROOT::Math::XYZPoint l2_pca_wrt_dv;
    // scalars
    double cpa_wrt_pv;
    // -- (anti)lambda 1
    double l1_cpa_wrt_dv;
    // -- (anti)lambda 2
    double l2_cpa_wrt_dv;
    // correlations
    double cos_theta_pp;
    double theta_pp;
    double cos_theta_star_l1;
    double cos_theta_star_l2;
    double cos_theta_star_p1;
    double cos_theta_star_p2;
    double q_rel;
};

}  // namespace Cached
