#pragma once

#include <array>
#include <cmath>
#include <cstdint>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Cached_Unit.hpp"
#include "Constants.hpp"
#include "Math.hpp"
#include "POD_V0.hpp"

namespace Cached {

struct V0 : POD::V0 {

    V0(const POD::V0& v0, const ROOT::Math::XYZPoint& ref, const std::array<float, Common::NCovMatrixComponents_State3>& ref_cov)
        : POD::V0(v0),
          seed{Seed_Neg_X, Seed_Neg_Y, Seed_Neg_Z, Seed_Neg_Px, Seed_Neg_Py, Seed_Neg_Pz, Seed_Neg_Energy,  //
               Seed_Pos_X, Seed_Pos_Y, Seed_Pos_Z, Seed_Pos_Px, Seed_Pos_Py, Seed_Pos_Pz, Seed_Pos_Energy},
          fit{Fit_Neg_Px, Fit_Neg_Py, Fit_Neg_Pz, Fit_Neg_Energy,  //
              Fit_Pos_Px, Fit_Pos_Py, Fit_Pos_Pz, Fit_Pos_Energy,  //
              Fit_X,      Fit_Y,      Fit_Z,      Common::Math::PositionCov(CovMatrix)},
          pv{ref},
          pv_cov{ref_cov} {}

    // post-seed/pre-fit //
    // -- geometry
    [[nodiscard]] double Seed_DCA_BetweenDaughters() const { return seed.DCA_Between(); }
    [[nodiscard]] double Seed_DCA_BetweenDaughters_Squared() const { return seed.DCA_Between_Squared(); }
    [[nodiscard]] double Seed_Radius2D() const { return seed.Radius2D(); }
    [[nodiscard]] double Seed_Radius2D_Squared() const { return seed.Radius2D_Squared(); }
    // -- kinematics
    [[nodiscard]] double Seed_Mass() const { return seed.Mass(); }
    [[nodiscard]] double Seed_ArmAlpha() const { return seed.ArmAlpha(); }
    [[nodiscard]] double Seed_ArmQt() const { return seed.ArmQt(); }
    [[nodiscard]] double Seed_Mass_AsK0S() const { return seed.Mass_AsK0S(); }
    [[nodiscard]] double Seed_Mass_AsLambda() const { return seed.Mass_AsLambda(); }
    [[nodiscard]] double Seed_Mass_AsAntiLambda() const { return seed.Mass_AsAntiLambda(); }
    [[nodiscard]] double Seed_Mass_AsPhoton() const { return seed.Mass_AsPhoton(); }

    // post-fit //
    // -- geometry
    [[nodiscard]] double Decay_Radius2D() const { return fit.Radius2D(); }
    [[nodiscard]] double Decay_Radius2D_Squared() const { return fit.Radius2D_Squared(); }
    [[nodiscard]] double Decay_Radius3D() const { return fit.Radius3D(); }
    [[nodiscard]] double Decay_Radius3D_Squared() const { return fit.Radius3D_Squared(); }
    // -- kinematics
    [[nodiscard]] double Fit_Px() const { return fit.Px(); }
    [[nodiscard]] double Fit_Py() const { return fit.Py(); }
    [[nodiscard]] double Fit_Pz() const { return fit.Pz(); }
    [[nodiscard]] double Fit_Pt() const { return fit.Pt(); }
    [[nodiscard]] double Fit_P() const { return fit.P(); }
    [[nodiscard]] double Fit_Eta() const { return fit.Eta(); }
    [[nodiscard]] double Fit_Energy() const { return fit.Energy(); }
    [[nodiscard]] double Fit_Mass() const { return fit.Mass(); }  // a delta when `IsMotherPinned()`
    [[nodiscard]] double Fit_Rapidity() const { return fit.Rapidity(); }
    [[nodiscard]] double Fit_ArmAlpha() const { return fit.ArmAlpha(); }
    [[nodiscard]] double Fit_ArmQt() const { return fit.ArmQt(); }
    [[nodiscard]] double Fit_ArmQtOverAlpha() const { return fit.ArmQtOverAlpha(); }
    [[nodiscard]] double Fit_ArmRadius_AsK0S() const { return fit.ArmRadius_AsK0S(); }
    [[nodiscard]] double Fit_ArmRadius_AsLambda() const { return fit.ArmRadius_AsLambda(); }  // restated `Fit_MassPull`, without its weighting
    [[nodiscard]] double Fit_ArmRadius_AsAntiLambda() const { return fit.ArmRadius_AsAntiLambda(); }  // idem
    [[nodiscard]] double Fit_Mass_AsK0S() const { return fit.Mass_AsK0S(); }
    [[nodiscard]] double Fit_Mass_AsLambda() const { return fit.Mass_AsLambda(); }
    [[nodiscard]] double Fit_Mass_AsAntiLambda() const { return fit.Mass_AsAntiLambda(); }
    [[nodiscard]] double Fit_Mass_AsPhoton() const { return fit.Mass_AsPhoton(); }
    [[nodiscard]] double Fit_UnpinnedMass() const {
        if (!IsMotherPinned()) return Fit_Mass();
        if (Fit_MassErr <= 0.F) return Common::DummyDouble;
        const double ps = static_cast<double>(Fit_MassPull) * static_cast<double>(Fit_MassErr);
        return ps + std::sqrt(ps * ps + Fit_Mass() * Fit_Mass());  // `Fit_Mass()` is a delta when `IsMotherPinned()`
    }
    // -- negative daughter's kinematics
    [[nodiscard]] double Fit_Neg_Pt() const { return fit.Dau1_Pt(); }
    [[nodiscard]] double Fit_Neg_Eta() const { return fit.Dau1_Eta(); }
    // -- positive daughter's kinematics
    [[nodiscard]] double Fit_Pos_Pt() const { return fit.Dau2_Pt(); }
    [[nodiscard]] double Fit_Pos_Eta() const { return fit.Dau2_Eta(); }
    // -- kinematics + geometry
    [[nodiscard]] double FlightLength_WithRespectToPV() const { return fit.FlightLength_WithRespectTo(pv); }
    [[nodiscard]] double FlightLength_DivByErr_WithRespectToPV() const {  //
        return fit.FlightLength_DivByErr_WithRespectTo(pv, pv_cov);
    }
    [[nodiscard]] double DCA_WithRespectToPV() const { return fit.DCA_WithRespectTo(pv); }
    [[nodiscard]] double DCA_WithRespectToPV_Squared() const { return fit.DCA_WithRespectTo_Squared(pv); }
    [[nodiscard]] double CPA_WithRespectToPV() const { return fit.CPA_WithRespectTo(pv); }
    // -- fit bookkeeping
    [[nodiscard]] bool IsMotherPinned() const {  //
        return (FitFlags & Common::FitFlag::kMotherPinned) != 0;
    }
    [[nodiscard]] bool AreDaughtersPinned() const {
        constexpr std::uint8_t both = Common::FitFlag::kDau1Pinned | Common::FitFlag::kDau2Pinned;
        return (FitFlags & both) == both;
    }
    [[nodiscard]] int NDF() const {  //
        return Common::NDF::kVertex + (IsMotherPinned() ? Common::NDF::kMassPin : 0);
    }
    [[nodiscard]] double Chi2Vtx() const {
        const auto chi2 = static_cast<double>(Chi2);
        if (!IsMotherPinned()) return chi2;
        const auto pull = static_cast<double>(Fit_MassPull);
        return chi2 - pull * pull;
    }
    [[nodiscard]] double Chi2NDF() const {  //
        return static_cast<double>(Chi2) / static_cast<double>(NDF());
    }

    // post-seed vs post-fit comparisons //
    [[nodiscard]] double MomentumKick() const { return (fit.lv.Vect() - seed.lv.Vect()).R(); }
    [[nodiscard]] double Neg_MomentumKick() const { return (fit.lv_dau1.Vect() - seed.lv_dau1.Vect()).R(); }
    [[nodiscard]] double Pos_MomentumKick() const { return (fit.lv_dau2.Vect() - seed.lv_dau2.Vect()).R(); }

   protected:
    Unit::PostSeed seed;
    Unit::PostFit fit;
    ROOT::Math::XYZPoint pv;                                        // primary vertex
    std::array<float, Common::NCovMatrixComponents_State3> pv_cov;  // its position covariance
};

}  // namespace Cached
