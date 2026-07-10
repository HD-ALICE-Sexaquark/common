#pragma once

#include <cmath>
#include <optional>

#include <Math/Point3Dfwd.h>
#include <Math/Vector3Dfwd.h>
#include <Math/Vector4Dfwd.h>

#include "Constants.hpp"
#include "Math.hpp"
#include "POD_Sexaquark.hpp"
#include "POD_V0.hpp"

namespace Cached {

struct Sexaquark : POD::Sexaquark {

    Sexaquark(const POD::Sexaquark& sexa, const ROOT::Math::XYZPoint& ref)  //
        : POD::Sexaquark(sexa),
          lv{Px, Py, Pz, Energy},
          lv_n{0., 0., 0., Energy - E_MinusNucleon},
          dau1_mom{Dau1_PCAwrtSV_Px, Dau1_PCAwrtSV_Py, Dau1_PCAwrtSV_Pz},
          dau2_mom{Dau2_PCAwrtSV_Px, Dau2_PCAwrtSV_Py, Dau2_PCAwrtSV_Pz},
          dau1_pca_wrt_sv{Dau1_PCAwrtSV_X, Dau1_PCAwrtSV_Y, Dau1_PCAwrtSV_Z},
          dau2_pca_wrt_sv{Dau2_PCAwrtSV_X, Dau2_PCAwrtSV_Y, Dau2_PCAwrtSV_Z},
          pv{ref},
          sv{SV_X, SV_Y, SV_Z},
          pca_wrt_pv{Common::Math::FastPCA_LineVertex(lv.Vect(), sv, pv)},
          cpa_wrt_pv{Common::Math::CosinePointingAngle(lv.Vect(), sv, pv)} {}

    Sexaquark(const POD::Sexaquark& sexa, const POD::V0& v0a, const POD::V0& v0b, const ROOT::Math::XYZPoint& ref)  //
        : Cached::Sexaquark(sexa, ref) {
        dau1_dv = {v0a.Decay_X, v0a.Decay_Y, v0a.Decay_Z};
        dau2_dv = {v0b.Decay_X, v0b.Decay_Y, v0b.Decay_Z};
        dau1_cpa_wrt_sv = Common::Math::CosinePointingAngle(dau1_mom, dau1_dv.value(), sv);
        dau2_cpa_wrt_sv = Common::Math::CosinePointingAngle(dau2_mom, dau2_dv.value(), sv);
    }

    Sexaquark(const POD::Sexaquark& sexa, const POD::V0& v0, const ROOT::Math::XYZPoint& ref)  //
        : Cached::Sexaquark(sexa, ref) {
        dau1_dv = {v0.Decay_X, v0.Decay_Y, v0.Decay_Z};
        dau1_cpa_wrt_sv = Common::Math::CosinePointingAngle(dau1_mom, dau1_dv.value(), sv);
    }

    // geometry
    [[nodiscard]] double SV_SquaredRadius2D() const { return sv.Perp2(); }
    [[nodiscard]] double SV_SquaredRadius3D() const { return sv.Mag2(); }
    [[nodiscard]] double SV_Radius2D() const { return sv.Rho(); }
    [[nodiscard]] double SV_Radius3D() const { return sv.R(); }
    [[nodiscard]] double SquaredDCA_btw_Daughters() const { return (dau1_pca_wrt_sv - dau2_pca_wrt_sv).Mag2(); }
    [[nodiscard]] double DCA_btw_Daughters() const { return (dau1_pca_wrt_sv - dau2_pca_wrt_sv).R(); }
    // kinematics
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    // kinematics+geometry
    [[nodiscard]] double SquaredDCA_wrt_PV() const { return (pca_wrt_pv - pv).Mag2(); }
    [[nodiscard]] double DCA_wrt_PV() const { return (pca_wrt_pv - pv).R(); }
    [[nodiscard]] double CPA_wrt_PV() const { return cpa_wrt_pv; }
    // kinematics (subtracted nucleon at rest)
    [[nodiscard]] double Mass_MinusNucleon() const { return (lv - lv_n).M(); }
    [[nodiscard]] double Rapidity_MinusNucleon() const { return (lv - lv_n).Rapidity(); }
    // daughter 1
    // -- kinematics
    [[nodiscard]] double Dau1_Pz() const { return dau1_mom.Z(); }
    [[nodiscard]] double Dau1_Pt() const { return dau1_mom.Rho(); }
    [[nodiscard]] double Dau1_P() const { return dau1_mom.R(); }
    [[nodiscard]] double Dau1_Eta() const { return dau1_mom.Eta(); }
    // -- related to anti-sexaquark
    [[nodiscard]] double Dau1_SquaredDCA_wrt_SV() const { return (dau1_pca_wrt_sv - sv).Mag2(); }
    [[nodiscard]] double Dau1_DCA_wrt_SV() const { return (dau1_pca_wrt_sv - sv).R(); }
    [[nodiscard]] double Dau1_CPA_wrt_SV() const { return dau1_cpa_wrt_sv.value_or(Common::DummyDouble); }
    [[nodiscard]] double Dau1_DecayLength() const { return dau1_dv.has_value() ? (dau1_dv.value() - sv).R() : Common::DummyDouble; }
    // daughter 2
    // -- kinematics
    [[nodiscard]] double Dau2_Pz() const { return dau2_mom.Z(); }
    [[nodiscard]] double Dau2_Pt() const { return dau2_mom.Rho(); }
    [[nodiscard]] double Dau2_P() const { return dau2_mom.R(); }
    [[nodiscard]] double Dau2_Eta() const { return dau2_mom.Eta(); }
    // -- related to anti-sexaquark
    [[nodiscard]] double Dau2_SquaredDCA_wrt_SV() const { return (dau2_pca_wrt_sv - sv).Mag2(); }
    [[nodiscard]] double Dau2_DCA_wrt_SV() const { return (dau2_pca_wrt_sv - sv).R(); }
    [[nodiscard]] double Dau2_CPA_wrt_SV() const { return dau2_cpa_wrt_sv.value_or(Common::DummyDouble); }
    [[nodiscard]] double Dau2_DecayLength() const { return dau2_dv.has_value() ? (dau2_dv.value() - sv).R() : Common::DummyDouble; }

   private:
    ROOT::Math::PxPyPzEVector lv;                 // lorentz vector
    ROOT::Math::PxPyPzEVector lv_n;               // lorentz vector of struck nucleon
    std::optional<ROOT::Math::XYZPoint> dau1_dv;  // decay vertex of daughter 1 (valid only when daughter is a V0)
    std::optional<ROOT::Math::XYZPoint> dau2_dv;  // decay vertex of daughter 2 (valid only when daughter is a V0)
    ROOT::Math::XYZVector dau1_mom;               // momentum
    ROOT::Math::XYZVector dau2_mom;               // momentum
    ROOT::Math::XYZPoint dau1_pca_wrt_sv;         // PCA of daughter 1 w.r.t. secondary vertex
    ROOT::Math::XYZPoint dau2_pca_wrt_sv;         // PCA of daughter 2 w.r.t. secondary vertex
    ROOT::Math::XYZPoint pv;                      // primary vertex
    ROOT::Math::XYZPoint sv;                      // secondary vertex
    ROOT::Math::XYZPoint pca_wrt_pv;              // point of closest approach w.r.t. primary vertex
    std::optional<double> dau1_cpa_wrt_sv;        // cosine pointing angle of daughter 1 (valid only when daughter is a V0)
    std::optional<double> dau2_cpa_wrt_sv;        // cosine pointing angle of daughter 2 (valid only when daughter is a V0)
    double cpa_wrt_pv;                            // cosine pointing angle
};

}  // namespace Cached
