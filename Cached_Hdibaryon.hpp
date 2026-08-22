#pragma once

#include <algorithm>
#include <cmath>
#include <tuple>

#include <Math/Point3D.h>
#include <Math/Vector4D.h>
#include <Math/VectorUtil.h>

#include "Constants.hpp"
#include "HD_Library.hpp"
#include "Math.hpp"
#include "POD_LambdaPair.hpp"
#include "POD_PreFoundLambda.hpp"

namespace Cached {

struct Hdibaryon : POD::LambdaPair {

    Hdibaryon(const POD::LambdaPair& hdib, const POD::Extended::PreFoundLambda& l1, const POD::Extended::PreFoundLambda& l2,
              const ROOT::Math::XYZPoint& ref)
        : POD::LambdaPair(hdib),
          lv{hdib.Px, hdib.Py, hdib.Pz, hdib.Energy},
          lv_cv{hdib.CV_Px, hdib.CV_Py, hdib.CV_Pz, hdib.CV_Energy},
          lv_lambda1{Lambda1_Fit_Px, Lambda1_Fit_Py, Lambda1_Fit_Pz, Lambda1_Fit_Energy},
          lv_lambda2{Lambda2_Fit_Px, Lambda2_Fit_Py, Lambda2_Fit_Pz, Lambda2_Fit_Energy},
          dv{hdib.Decay_X, hdib.Decay_Y, hdib.Decay_Z},
          cv{hdib.CV_X, hdib.CV_Y, hdib.CV_Z},
          pv{ref},
          pca_wrt_pv{Common::Math::FastPCA_LineVertex(lv.Vect(), dv, pv)},
          dv_l1{l1.Decay_X, l1.Decay_Y, l1.Decay_Z},
          dv_l2{l2.Decay_X, l2.Decay_Y, l2.Decay_Z},
          l1_pca_wrt_dv{Lambda1_PCAwrtDV_X, Lambda1_PCAwrtDV_Y, Lambda1_PCAwrtDV_Z},
          l2_pca_wrt_dv{Lambda2_PCAwrtDV_X, Lambda2_PCAwrtDV_Y, Lambda2_PCAwrtDV_Z},
          cpa_wrt_pv{Common::Math::CosinePointingAngle(lv.Vect(), dv, pv)},
          l1_cpa_wrt_dv{Common::Math::CosinePointingAngle(lv_lambda1.Vect(), dv_l1, dv)},
          l2_cpa_wrt_dv{Common::Math::CosinePointingAngle(lv_lambda2.Vect(), dv_l2, dv)},
          cos_theta_ll{lv_lambda1.Vect().Unit().Dot(lv_lambda2.Vect().Unit())},
          is_anti{HD::IsAntiChannel(l1, l2)},
          is_mixed{HD::IsMixedChannel(l1, l2)} {
        // close decay trees
        if (l1.IsAntiLambda) {
            std::tie(lv_proton1, lv_pion1) = Common::Math::CloseDecay(l1, Common::PdgMass_Proton, Common::PdgMass_Pion, lv_lambda1);
        } else {
            std::tie(lv_pion1, lv_proton1) = Common::Math::CloseDecay(l1, Common::PdgMass_Pion, Common::PdgMass_Proton, lv_lambda1);
        }
        if (l2.IsAntiLambda) {
            std::tie(lv_proton2, lv_pion2) = Common::Math::CloseDecay(l2, Common::PdgMass_Proton, Common::PdgMass_Pion, lv_lambda2);
        } else {
            std::tie(lv_pion2, lv_proton2) = Common::Math::CloseDecay(l2, Common::PdgMass_Pion, Common::PdgMass_Proton, lv_lambda2);
        }
        // correlations
        HD::InfoCorrelation c_corr = HD::GetAngles(lv, lv_lambda1, lv_lambda2, lv_proton1, lv_proton2);
        cos_theta_pp = c_corr.cos_theta_pp;
        theta_pp = c_corr.theta_pp;
        cos_theta_star_l1 = c_corr.cos_theta_star_l1;
        cos_theta_star_l2 = c_corr.cos_theta_star_l2;
        cos_theta_star_p1 = c_corr.cos_theta_star_p1;
        cos_theta_star_p2 = c_corr.cos_theta_star_p2;
        q_rel = c_corr.q_rel;
    }

    // (anti)h-dibaryon candidate //
    // -- classification
    [[nodiscard]] bool IsAntiChannel() const { return is_anti; }
    [[nodiscard]] bool IsMixedChannel() const { return is_mixed; }
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

    // (anti)h-dibaryon candidate; available with PV constraint //
    [[nodiscard]] bool HasCV() const { return static_cast<double>(Chi2CV) > 0.; }  // valid guard because chi2 is never negative, the dummy is
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

    // (anti)lambda 1; related to (anti)h-dibaryon
    [[nodiscard]] double Lambda1_DecayLength() const { return (dv_l1 - dv).R(); }
    [[nodiscard]] double Lambda1_CPA_wrt_DV() const { return l1_cpa_wrt_dv; }
    [[nodiscard]] double Lambda1_DCA_wrt_DV() const { return (l1_pca_wrt_dv - dv).R(); }
    // -- (anti)proton 1
    [[nodiscard]] double Proton1_Px() const { return lv_proton1.Px(); }
    [[nodiscard]] double Proton1_Py() const { return lv_proton1.Py(); }
    [[nodiscard]] double Proton1_Pz() const { return lv_proton1.Pz(); }
    [[nodiscard]] double Proton1_Pt() const { return lv_proton1.Pt(); }
    [[nodiscard]] double Proton1_P() const { return lv_proton1.P(); }
    [[nodiscard]] double Proton1_Eta() const { return lv_proton1.Eta(); }
    // -- pi(minus/plus) 1
    [[nodiscard]] double Pion1_Px() const { return lv_pion1.Px(); }
    [[nodiscard]] double Pion1_Py() const { return lv_pion1.Py(); }
    [[nodiscard]] double Pion1_Pz() const { return lv_pion1.Pz(); }
    [[nodiscard]] double Pion1_Pt() const { return lv_pion1.Pt(); }
    [[nodiscard]] double Pion1_P() const { return lv_pion1.P(); }
    [[nodiscard]] double Pion1_Eta() const { return lv_pion1.Eta(); }

    // (anti)lambda 2; related to (anti)h-dibaryon
    [[nodiscard]] double Lambda2_DecayLength() const { return (dv_l2 - dv).R(); }
    [[nodiscard]] double Lambda2_CPA_wrt_DV() const { return l2_cpa_wrt_dv; }
    [[nodiscard]] double Lambda2_DCA_wrt_DV() const { return (l2_pca_wrt_dv - dv).R(); }
    // -- (anti)proton 2
    [[nodiscard]] double Proton2_Px() const { return lv_proton2.Px(); }
    [[nodiscard]] double Proton2_Py() const { return lv_proton2.Py(); }
    [[nodiscard]] double Proton2_Pz() const { return lv_proton2.Pz(); }
    [[nodiscard]] double Proton2_Pt() const { return lv_proton2.Pt(); }
    [[nodiscard]] double Proton2_P() const { return lv_proton2.P(); }
    [[nodiscard]] double Proton2_Eta() const { return lv_proton2.Eta(); }
    // -- pi(minus/plus) 2
    [[nodiscard]] double Pion2_Px() const { return lv_pion2.Px(); }
    [[nodiscard]] double Pion2_Py() const { return lv_pion2.Py(); }
    [[nodiscard]] double Pion2_Pz() const { return lv_pion2.Pz(); }
    [[nodiscard]] double Pion2_Pt() const { return lv_pion2.Pt(); }
    [[nodiscard]] double Pion2_P() const { return lv_pion2.P(); }
    [[nodiscard]] double Pion2_Eta() const { return lv_pion2.Eta(); }

    // (anti)lambda pair : lab frame //
    [[nodiscard]] double CosTheta_LL() const { return cos_theta_ll; }
    [[nodiscard]] double Theta_LL() const { return std::acos(std::clamp(cos_theta_ll, -1., 1.)); }
    [[nodiscard]] double DeltaEta_LL() const { return lv_lambda1.Eta() - lv_lambda2.Eta(); }
    [[nodiscard]] double DeltaPhi_LL() const { return ROOT::Math::VectorUtil::DeltaPhi(lv_lambda1, lv_lambda2); }
    [[nodiscard]] double DeltaR_LL() const { return std::hypot(DeltaEta_LL(), DeltaPhi_LL()); }
    // -- momentum sharing: q* = sqrt(M^2/4 - m_lambda^2) ~ 54 MeV/c at m(H) = 2.234 GeV/c^2,
    //    so a genuine pair splits the h-dibaryon momentum to within 2q*/M ~  5%
    [[nodiscard]] double Asym_LL() const {
        const double p1 = lv_lambda1.P();
        const double p2 = lv_lambda2.P();
        return (p1 + p2) > Common::AbsAlmostZero ? (p1 - p2) / (p1 + p2) : Common::DummyDouble;
    }

    // correlations //
    [[nodiscard]] double CosTheta_pp() const { return cos_theta_pp; }
    [[nodiscard]] double Theta_pp() const { return theta_pp; }
    [[nodiscard]] double CosThetaStar_L1() const { return cos_theta_star_l1; }
    [[nodiscard]] double CosThetaStar_L2() const { return cos_theta_star_l2; }
    [[nodiscard]] double CosThetaStar_Pr1() const { return cos_theta_star_p1; }
    [[nodiscard]] double CosThetaStar_Pr2() const { return cos_theta_star_p2; }
    [[nodiscard]] double Qrel() const { return q_rel; }

    // duplication tests //
    [[nodiscard]] double Dist_btw_LambdaDVs() const { return (dv_l1 - dv_l2).R(); }
    [[nodiscard]] double DeltaDecayLength_LL() const { return std::abs(Lambda1_DecayLength() - Lambda2_DecayLength()); }

   private:
    ROOT::Math::PxPyPzEVector lv;          // lorentz vector
    ROOT::Math::PxPyPzEVector lv_cv;       // constrained to production vertex
    ROOT::Math::PxPyPzEVector lv_lambda1;  // lorentz vector
    ROOT::Math::PxPyPzEVector lv_lambda2;  // lorentz vector
    ROOT::Math::PxPyPzEVector lv_proton1;  // lorentz vector
    ROOT::Math::PxPyPzEVector lv_proton2;  // lorentz vector
    ROOT::Math::PxPyPzEVector lv_pion1;    // lorentz vector
    ROOT::Math::PxPyPzEVector lv_pion2;    // lorentz vector
    ROOT::Math::XYZPoint dv;               // reconstructed decay vertex
    ROOT::Math::XYZPoint cv;               // constrained production vertex ~ primary vertex
    ROOT::Math::XYZPoint pv;               // primary vertex
    ROOT::Math::XYZPoint pca_wrt_pv;       // ~ origin
    ROOT::Math::XYZPoint dv_l1;
    ROOT::Math::XYZPoint dv_l2;
    ROOT::Math::XYZPoint l1_pca_wrt_dv;
    ROOT::Math::XYZPoint l2_pca_wrt_dv;
    double cpa_wrt_pv;
    double l1_cpa_wrt_dv;
    double l2_cpa_wrt_dv;
    double cos_theta_ll;
    double cos_theta_pp;
    double theta_pp;
    double cos_theta_star_l1;
    double cos_theta_star_l2;
    double cos_theta_star_p1;
    double cos_theta_star_p2;
    double q_rel;
    bool is_anti;
    bool is_mixed;
};

}  // namespace Cached
