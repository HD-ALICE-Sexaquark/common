#pragma once

#include <algorithm>
#include <cmath>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>
#include <Math/VectorUtil.h>

#include "Math.hpp"
#include "POD_Sexaquark.hpp"

namespace Cached {

struct ChannelH : POD::Sexaquark {

    ChannelH(const POD::Sexaquark& sexa, const ROOT::Math::XYZPoint& ref)
        : POD::Sexaquark(sexa),
          lv{Px, Py, Pz, Energy},
          lv_n{0., 0., 0., Energy - E_MinusNucleon},
          lv_ka1{Dau1_Fit_Px, Dau1_Fit_Py, Dau1_Fit_Pz, Dau1_Fit_Energy},
          lv_ka2{Dau2_Fit_Px, Dau2_Fit_Py, Dau2_Fit_Pz, Dau2_Fit_Energy},
          ka1_pca_wrt_sv{Dau1_PCAwrtSV_X, Dau1_PCAwrtSV_Y, Dau1_PCAwrtSV_Z},
          ka2_pca_wrt_sv{Dau2_PCAwrtSV_X, Dau2_PCAwrtSV_Y, Dau2_PCAwrtSV_Z},
          pv{ref},
          sv{SV_X, SV_Y, SV_Z},
          pca_wrt_pv{Common::Math::FastPCA_LineVertex(lv.Vect(), sv, pv)},
          axis{(sv - pv).Unit()},
          cpa_wrt_pv{Common::Math::CosinePointingAngle(lv.Vect(), sv, pv)} {}

    // antisexaquark candidate //
    // -- kinematics
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    // -- geometry
    [[nodiscard]] double SV_SquaredRadius2D() const { return sv.Perp2(); }
    [[nodiscard]] double SV_SquaredRadius3D() const { return sv.Mag2(); }
    [[nodiscard]] double SV_Radius2D() const { return sv.Rho(); }
    [[nodiscard]] double SV_Radius3D() const { return sv.R(); }
    [[nodiscard]] double SV_Phi() const { return sv.Phi(); }
    [[nodiscard]] double FlightLength() const { return (sv - pv).R(); }
    [[nodiscard]] double SquaredDCA_btw_Daughters() const { return (ka1_pca_wrt_sv - ka2_pca_wrt_sv).Mag2(); }
    [[nodiscard]] double DCA_btw_Daughters() const { return (ka1_pca_wrt_sv - ka2_pca_wrt_sv).R(); }
    [[nodiscard]] double CosTheta_btw_Daughters() const { return ROOT::Math::VectorUtil::CosTheta(lv_ka1.Vect(), lv_ka2.Vect()); }
    [[nodiscard]] double Theta_btw_Daughters() const { return std::acos(std::clamp(CosTheta_btw_Daughters(), -1., 1.)); }
    // -- kinematics + geometry
    [[nodiscard]] double Qt_wrt_PV() const { return lv.Vect().Cross(axis).R(); }
    [[nodiscard]] double Ql_wrt_PV() const { return lv.Vect().Dot(axis); }
    [[nodiscard]] double SquaredDCA_wrt_PV() const { return (pca_wrt_pv - pv).Mag2(); }
    [[nodiscard]] double DCA_wrt_PV() const { return (pca_wrt_pv - pv).R(); }
    [[nodiscard]] double CPA_wrt_PV() const { return cpa_wrt_pv; }
    // -- kinematics (subtracted nucleon at rest)
    [[nodiscard]] double Mass_MinusNucleon() const { return (lv - lv_n).M(); }
    [[nodiscard]] double Rapidity_MinusNucleon() const { return (lv - lv_n).Rapidity(); }

    // charged kaon 1; related to antisexaquark //
    [[nodiscard]] double Kaon1_SquaredDCA_wrt_SV() const { return (ka1_pca_wrt_sv - sv).Mag2(); }
    [[nodiscard]] double Kaon1_DCA_wrt_SV() const { return (ka1_pca_wrt_sv - sv).R(); }

    // charged kaon 2; related to antisexaquark //
    [[nodiscard]] double Kaon2_SquaredDCA_wrt_SV() const { return (ka2_pca_wrt_sv - sv).Mag2(); }
    [[nodiscard]] double Kaon2_DCA_wrt_SV() const { return (ka2_pca_wrt_sv - sv).R(); }

   private:
    ROOT::Math::PxPyPzEVector lv;         // lorentz vector; formed by just adding the reaction products, no struck nucleon involved
    ROOT::Math::PxPyPzEVector lv_n;       // lorentz vector of assumed struck nucleon at rest
    ROOT::Math::PxPyPzEVector lv_ka1;     // lorentz vector of kaon 1, post-fit, at the secondary vertex
    ROOT::Math::PxPyPzEVector lv_ka2;     // lorentz vector of kaon 2, post-fit, at the secondary vertex
    ROOT::Math::XYZPoint ka1_pca_wrt_sv;  // PCA of kaon 1 w.r.t. secondary vertex
    ROOT::Math::XYZPoint ka2_pca_wrt_sv;  // PCA of kaon 2 w.r.t. secondary vertex
    ROOT::Math::XYZPoint pv;              // primary vertex
    ROOT::Math::XYZPoint sv;              // reconstructed secondary vertex
    ROOT::Math::XYZPoint pca_wrt_pv;      // point of closest approach w.r.t. primary vertex
    ROOT::Math::XYZVector axis;           // unit vector along the flight direction, primary -> secondary vertex
    double cpa_wrt_pv;                    // cosine pointing angle
};

}  // namespace Cached
