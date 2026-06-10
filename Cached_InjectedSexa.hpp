#pragma once

#include <cmath>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Math.hpp"
#include "POD_InjectedSexa.hpp"

namespace Cached {

struct InjectedSexa : POD::Extended::InjectedSexa {

    InjectedSexa(const POD::Extended::InjectedSexa& sexa, const ROOT::Math::PxPyPzEVector& sum_lv_daughters,
                 const ROOT::Math::XYZPoint& ref)  //
        : POD::Extended::InjectedSexa(sexa),
          lv{sexa.Px, sexa.Py, sexa.Pz, sexa.Energy},
          lv_nucleon{sexa.Nucleon_Px, sexa.Nucleon_Py, sexa.Nucleon_Pz, sexa.Nucleon_Energy},
          sum_lv_daughters{sum_lv_daughters},
          pv{ref},
          sv{sexa.SV_X, sexa.SV_Y, sexa.SV_Z},
          pca_wrt_pv{Common::Math::FastPCA_LineVertex(lv.Vect(), sv, pv)} {}

    // kinematics
    // -- injected
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double AbsEta() const { return std::abs(Eta()); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    // -- struck nucleon
    [[nodiscard]] double Nucleon_Pt() const { return lv_nucleon.Pt(); }
    [[nodiscard]] double Nucleon_P() const { return lv_nucleon.P(); }
    [[nodiscard]] double Nucleon_Mass() const { return lv_nucleon.M(); }
    [[nodiscard]] double Nucleon_Eta() const { return lv_nucleon.Eta(); }
    [[nodiscard]] double Nucleon_Rapidity() const { return lv_nucleon.Rapidity(); }
    [[nodiscard]] double Nucleon_Phi() const { return lv_nucleon.Phi(); }
    // -- after reaction ~ sum of all reaction products
    [[nodiscard]] double After_Px() const { return sum_lv_daughters.Px(); }
    [[nodiscard]] double After_Py() const { return sum_lv_daughters.Py(); }
    [[nodiscard]] double After_Pz() const { return sum_lv_daughters.Pz(); }
    [[nodiscard]] double After_Energy() const { return sum_lv_daughters.E(); }
    [[nodiscard]] double After_Pt() const { return sum_lv_daughters.Pt(); }
    [[nodiscard]] double After_P() const { return sum_lv_daughters.P(); }
    [[nodiscard]] double After_Mass() const { return sum_lv_daughters.M(); }
    [[nodiscard]] double After_Eta() const { return sum_lv_daughters.Eta(); }
    [[nodiscard]] double After_Rapidity() const { return sum_lv_daughters.Rapidity(); }
    [[nodiscard]] double After_Phi() const { return sum_lv_daughters.Phi(); }
    // geometry
    [[nodiscard]] double SV_Radius2D() const { return sv.Rho(); }
    [[nodiscard]] double SV_Radius3D() const { return sv.R(); }
    [[nodiscard]] double DCAxy_wrt_PV() const { return (pca_wrt_pv - pv).Rho(); }
    [[nodiscard]] double DCAz_wrt_PV() const { return std::abs((pca_wrt_pv - pv).Z()); }
    [[nodiscard]] double DCA_wrt_PV() const { return (pca_wrt_pv - pv).R(); }

   private:
    ROOT::Math::PxPyPzEVector lv;
    ROOT::Math::PxPyPzEVector lv_nucleon;
    ROOT::Math::PxPyPzEVector sum_lv_daughters;
    ROOT::Math::XYZPoint pv;
    ROOT::Math::XYZPoint sv;
    ROOT::Math::XYZPoint pca_wrt_pv;
};

}  // namespace Cached
