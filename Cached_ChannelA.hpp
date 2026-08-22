#pragma once

#include <algorithm>
#include <cmath>
#include <tuple>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>
#include <Math/VectorUtil.h>

#include "Constants.hpp"
#include "Math.hpp"
#include "POD_Sexaquark.hpp"
#include "POD_V0.hpp"

namespace Cached {

struct ChannelA : POD::Sexaquark {

    ChannelA(const POD::Sexaquark& sexa, const POD::V0& v0a, const POD::V0& v0b, const ROOT::Math::XYZPoint& ref)
        : POD::Sexaquark(sexa),
          lv{Px, Py, Pz, Energy},
          lv_n{0., 0., 0., Energy - E_MinusNucleon},
          lv_v0a{Dau1_Fit_Px, Dau1_Fit_Py, Dau1_Fit_Pz, Dau1_Fit_Energy},
          lv_v0b{Dau2_Fit_Px, Dau2_Fit_Py, Dau2_Fit_Pz, Dau2_Fit_Energy},
          v0a_dv{v0a.Decay_X, v0a.Decay_Y, v0a.Decay_Z},
          v0b_dv{v0b.Decay_X, v0b.Decay_Y, v0b.Decay_Z},
          v0a_pca_wrt_sv{Dau1_PCAwrtSV_X, Dau1_PCAwrtSV_Y, Dau1_PCAwrtSV_Z},
          v0b_pca_wrt_sv{Dau2_PCAwrtSV_X, Dau2_PCAwrtSV_Y, Dau2_PCAwrtSV_Z},
          pv{ref},
          sv{SV_X, SV_Y, SV_Z},
          pca_wrt_pv{Common::Math::FastPCA_LineVertex(lv.Vect(), sv, pv)},
          axis{(sv - pv).Unit()},
          v0a_cpa_wrt_sv{Common::Math::CosinePointingAngle(lv_v0a.Vect(), v0a_dv, sv)},
          v0b_cpa_wrt_sv{Common::Math::CosinePointingAngle(lv_v0b.Vect(), v0b_dv, sv)},
          cpa_wrt_pv{Common::Math::CosinePointingAngle(lv.Vect(), sv, pv)} {
        // close decay trees
        std::tie(lv_v0a_neg, lv_v0a_pos) = Common::Math::CloseDecay(v0a, IsBkgChannel ? Common::PdgMass_Pion : Common::PdgMass_Proton,
                                                                    IsBkgChannel ? Common::PdgMass_Proton : Common::PdgMass_Pion, lv_v0a);
        std::tie(lv_v0b_neg, lv_v0b_pos) = Common::Math::CloseDecay(v0b, Common::PdgMass_Pion, Common::PdgMass_Pion, lv_v0b);
    }

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
    [[nodiscard]] double SquaredDCA_btw_Daughters() const { return (v0a_pca_wrt_sv - v0b_pca_wrt_sv).Mag2(); }
    [[nodiscard]] double DCA_btw_Daughters() const { return (v0a_pca_wrt_sv - v0b_pca_wrt_sv).R(); }
    [[nodiscard]] double CosTheta_btw_Daughters() const { return ROOT::Math::VectorUtil::CosTheta(lv_v0a.Vect(), lv_v0b.Vect()); }
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

    // (anti)lambda; related to antisexaquark //
    [[nodiscard]] double V0A_SquaredDCA_wrt_SV() const { return (v0a_pca_wrt_sv - sv).Mag2(); }
    [[nodiscard]] double V0A_DCA_wrt_SV() const { return (v0a_pca_wrt_sv - sv).R(); }
    [[nodiscard]] double V0A_CPA_wrt_SV() const { return v0a_cpa_wrt_sv; }
    [[nodiscard]] double V0A_DecayLength() const { return (v0a_dv - sv).R(); }
    // -- (anti)lambda's negative daughter; with closed kinematics
    [[nodiscard]] double V0A_Neg_Px() const { return lv_v0a_neg.Px(); }
    [[nodiscard]] double V0A_Neg_Py() const { return lv_v0a_neg.Py(); }
    [[nodiscard]] double V0A_Neg_Pz() const { return lv_v0a_neg.Pz(); }
    [[nodiscard]] double V0A_Neg_Pt() const { return lv_v0a_neg.Pt(); }
    [[nodiscard]] double V0A_Neg_P() const { return lv_v0a_neg.P(); }
    [[nodiscard]] double V0A_Neg_Eta() const { return lv_v0a_neg.Eta(); }
    // -- (anti)lambda's positive daughter; with closed kinematics
    [[nodiscard]] double V0A_Pos_Px() const { return lv_v0a_pos.Px(); }
    [[nodiscard]] double V0A_Pos_Py() const { return lv_v0a_pos.Py(); }
    [[nodiscard]] double V0A_Pos_Pz() const { return lv_v0a_pos.Pz(); }
    [[nodiscard]] double V0A_Pos_Pt() const { return lv_v0a_pos.Pt(); }
    [[nodiscard]] double V0A_Pos_P() const { return lv_v0a_pos.P(); }
    [[nodiscard]] double V0A_Pos_Eta() const { return lv_v0a_pos.Eta(); }

    // kaon zero short; related to antisexaquark //
    [[nodiscard]] double V0B_SquaredDCA_wrt_SV() const { return (v0b_pca_wrt_sv - sv).Mag2(); }
    [[nodiscard]] double V0B_DCA_wrt_SV() const { return (v0b_pca_wrt_sv - sv).R(); }
    [[nodiscard]] double V0B_CPA_wrt_SV() const { return v0b_cpa_wrt_sv; }
    [[nodiscard]] double V0B_DecayLength() const { return (v0b_dv - sv).R(); }
    // -- k0s' negative daughter; with closed kinematics
    [[nodiscard]] double V0B_Neg_Px() const { return lv_v0b_neg.Px(); }
    [[nodiscard]] double V0B_Neg_Py() const { return lv_v0b_neg.Py(); }
    [[nodiscard]] double V0B_Neg_Pz() const { return lv_v0b_neg.Pz(); }
    [[nodiscard]] double V0B_Neg_Pt() const { return lv_v0b_neg.Pt(); }
    [[nodiscard]] double V0B_Neg_P() const { return lv_v0b_neg.P(); }
    [[nodiscard]] double V0B_Neg_Eta() const { return lv_v0b_neg.Eta(); }
    // -- k0s' positive daughter; with closed kinematics
    [[nodiscard]] double V0B_Pos_Px() const { return lv_v0b_pos.Px(); }
    [[nodiscard]] double V0B_Pos_Py() const { return lv_v0b_pos.Py(); }
    [[nodiscard]] double V0B_Pos_Pz() const { return lv_v0b_pos.Pz(); }
    [[nodiscard]] double V0B_Pos_Pt() const { return lv_v0b_pos.Pt(); }
    [[nodiscard]] double V0B_Pos_P() const { return lv_v0b_pos.P(); }
    [[nodiscard]] double V0B_Pos_Eta() const { return lv_v0b_pos.Eta(); }

   private:
    ROOT::Math::PxPyPzEVector lv;          // lorentz vector; formed by just adding the reaction products, no struck nucleon involved
    ROOT::Math::PxPyPzEVector lv_n;        // lorentz vector of assumed struck nucleon at rest
    ROOT::Math::PxPyPzEVector lv_v0a;      // lorentz vector of (anti)lambda, post-fit, at the secondary vertex
    ROOT::Math::PxPyPzEVector lv_v0b;      // lorentz vector of k0s, post-fit, at the secondary vertex
    ROOT::Math::PxPyPzEVector lv_v0a_neg;  // granddaughters, re-attached
    ROOT::Math::PxPyPzEVector lv_v0a_pos;  //
    ROOT::Math::PxPyPzEVector lv_v0b_neg;  //
    ROOT::Math::PxPyPzEVector lv_v0b_pos;  //
    ROOT::Math::XYZPoint v0a_dv;           // decay vertex of (anti)lambda
    ROOT::Math::XYZPoint v0b_dv;           // decay vertex of k0s
    ROOT::Math::XYZPoint v0a_pca_wrt_sv;   // PCA of (anti)lambda w.r.t. secondary vertex
    ROOT::Math::XYZPoint v0b_pca_wrt_sv;   // PCA of k0s w.r.t. secondary vertex
    ROOT::Math::XYZPoint pv;               // primary vertex
    ROOT::Math::XYZPoint sv;               // reconstructed secondary vertex
    ROOT::Math::XYZPoint pca_wrt_pv;       // point of closest approach w.r.t. primary vertex
    ROOT::Math::XYZVector axis;            // unit vector along the flight direction, primary -> secondary vertex
    double v0a_cpa_wrt_sv;                 // cosine pointing angle of (anti)lambda w.r.t. secondary vertex
    double v0b_cpa_wrt_sv;                 // cosine pointing angle of k0s w.r.t. secondary vertex
    double cpa_wrt_pv;                     // cosine pointing angle of antisexaquark w.r.t. primary vertex
};

}  // namespace Cached
