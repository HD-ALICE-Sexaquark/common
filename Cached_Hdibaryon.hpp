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
#include "POD_Hdibaryon.hpp"
#include "POD_V0.hpp"

namespace Cached {

namespace RMath = ROOT::Math;
namespace CMath = Common::Math;

// (anti)h-dibaryon  -> (anti)lambda (anti)lambda
struct Hdibaryon : POD::Hdibaryon {

    Hdibaryon(const POD::Hdibaryon& hdib, const POD::V0& lambda1, const POD::V0& lambda2, const RMath::XYZPoint& ref,
              const std::array<float, Common::NCovMatrixComponents_State3>& ref_cov)
        : POD::Hdibaryon(hdib),
          seed{Seed_Lambda1_X,  Seed_Lambda1_Y,  Seed_Lambda1_Z,                        //
               Seed_Lambda1_Px, Seed_Lambda1_Py, Seed_Lambda1_Pz, Seed_Lambda1_Energy,  //
               Seed_Lambda2_X,  Seed_Lambda2_Y,  Seed_Lambda2_Z,                        //
               Seed_Lambda2_Px, Seed_Lambda2_Py, Seed_Lambda2_Pz, Seed_Lambda2_Energy},
          fit{Fit_Lambda1_Px, Fit_Lambda1_Py, Fit_Lambda1_Pz, Fit_Lambda1_Energy,  //
              Fit_Lambda2_Px, Fit_Lambda2_Py, Fit_Lambda2_Pz, Fit_Lambda2_Energy,  //
              Fit_X,          Fit_Y,          Fit_Z,          DV_CovMatrix},
          fit_l1{lambda1.Fit_Neg_Px, lambda1.Fit_Neg_Py, lambda1.Fit_Neg_Pz, lambda1.Fit_Neg_Energy,
                 lambda1.Fit_Pos_Px, lambda1.Fit_Pos_Py, lambda1.Fit_Pos_Pz, lambda1.Fit_Pos_Energy,
                 lambda1.Fit_X,      lambda1.Fit_Y,      lambda1.Fit_Z,      CMath::PositionCov(lambda1.CovMatrix)},
          fit_l2{lambda2.Fit_Neg_Px, lambda2.Fit_Neg_Py, lambda2.Fit_Neg_Pz, lambda2.Fit_Neg_Energy,
                 lambda2.Fit_Pos_Px, lambda2.Fit_Pos_Py, lambda2.Fit_Pos_Pz, lambda2.Fit_Pos_Energy,
                 lambda2.Fit_X,      lambda2.Fit_Y,      lambda2.Fit_Z,      CMath::PositionCov(lambda2.CovMatrix)},
          corr{Unit::Correlations::FromNonClosedTree(fit, fit_l1, fit_l2, Lambda1_IsAntiLambda, Lambda2_IsAntiLambda)},
          pv{ref},
          pv_cov{ref_cov} {}

    // (anti)h-dibaryon specifics //
    [[nodiscard]] bool IsAntiChannel() const { return Lambda1_IsAntiLambda && Lambda2_IsAntiLambda; }
    [[nodiscard]] bool IsMixedChannel() const { return Lambda1_IsAntiLambda != Lambda2_IsAntiLambda; }

    // post-seed/pre-fit //
    // -- geometry
    [[nodiscard]] double Seed_DCA_BetweenLambdas() const { return seed.DCA_Between(); }
    [[nodiscard]] double Seed_DCA_BetweenLambdas_Squared() const { return seed.DCA_Between_Squared(); }
    [[nodiscard]] double Seed_Radius2D() const { return seed.Radius2D(); }
    [[nodiscard]] double Seed_Radius2D_Squared() const { return seed.Radius2D_Squared(); }
    // -- kinematics
    [[nodiscard]] double Seed_Mass() const { return seed.Mass(); }

    // post-fit //
    // -- geometry
    [[nodiscard]] double DV_Radius2D() const { return fit.Radius2D(); }
    [[nodiscard]] double DV_Radius3D() const { return fit.Radius3D(); }
    // -- kinematics
    [[nodiscard]] double Fit_Pz() const { return fit.Pz(); }
    [[nodiscard]] double Fit_Pt() const { return fit.Pt(); }
    [[nodiscard]] double Fit_P() const { return fit.P(); }
    [[nodiscard]] double Fit_Mass() const { return fit.Mass(); }
    [[nodiscard]] double Fit_Rapidity() const { return fit.Rapidity(); }
    [[nodiscard]] double Fit_CosTheta_BetweenLambdas() const {  // `Fit_Mass()` restated at fixed leg momenta
        return fit.CosTheta_BetweenDaughters();
    }
    [[nodiscard]] double Fit_Theta_BetweenLambdas() const { return fit.Theta_BetweenDaughters(); }  // restated `Fit_CosTheta()`
    // -- (anti)lambda 1's kinematics
    [[nodiscard]] double Fit_L1_Pt() const { return fit.Dau1_Pt(); }
    [[nodiscard]] double Fit_L1_Rapidity() const { return fit.Dau1_Rapidity(); }
    // -- (anti)lambda 2's kinematics
    [[nodiscard]] double Fit_L2_Pt() const { return fit.Dau2_Pt(); }
    [[nodiscard]] double Fit_L2_Rapidity() const { return fit.Dau2_Rapidity(); }
    // -- kinematics + geometry
    [[nodiscard]] double FlightLength_WithRespectToPV() const { return fit.FlightLength_WithRespectTo(pv); }
    [[nodiscard]] double FlightLength_DivByErr_WithRespectToPV() const { return fit.FlightLength_DivByErr_WithRespectTo(pv, pv_cov); }
    [[nodiscard]] double DCA_WithRespectToPV() const { return fit.DCA_WithRespectTo(pv); }
    [[nodiscard]] double CPA_WithRespectToPV() const { return fit.CPA_WithRespectTo(pv); }
    // -- fit bookkeeping
    [[nodiscard]] bool IsProductionVertexPinned() const { return (FitFlags & Common::FitFlag::kAtProdVertex) != 0; }
    [[nodiscard]] bool AreDaughtersPinned() const {
        constexpr std::uint8_t both = Common::FitFlag::kDau1Pinned | Common::FitFlag::kDau2Pinned;
        return (FitFlags & both) == both;
    }
    [[nodiscard]] int NDF() const { return Common::NDF::kVertex + (IsProductionVertexPinned() ? Common::NDF::kProdVertex : 0); }
    [[nodiscard]] double Chi2Vtx() const { return static_cast<double>(Chi2); }
    [[nodiscard]] double Chi2NDF() const {  // joint: vertex, plus the production-vertex constraint when it fired
        const double chi2 = IsProductionVertexPinned() ? static_cast<double>(Chi2) + static_cast<double>(PVC_Chi2) : static_cast<double>(Chi2);
        return chi2 / static_cast<double>(NDF());
    }
    //  > production-vertex-constrained fit
    [[nodiscard]] double Chi2PVC() const { return IsProductionVertexPinned() ? static_cast<double>(PVC_Chi2) : Common::DummyDouble; }
    [[nodiscard]] double PVC_DecayLength_DivByErr() const {  // signed; a pull for a prompt candidate
        return IsProductionVertexPinned() ? CMath::DivByErr(static_cast<double>(PVC_DecayLength), static_cast<double>(PVC_DecayLengthErr))
                                          : Common::DummyDouble;
    }
    [[nodiscard]] double Chi2Prompt() const { return fit.Chi2PointCoincidence_WithRespectTo(pv, pv_cov); }

    // correlations //
    [[nodiscard]] double CosTheta_pp() const { return corr.CosTheta_pp(); }
    [[nodiscard]] double Theta_pp() const { return corr.Theta_pp(); }
    [[nodiscard]] double CosThetaStar_L1() const { return corr.CosThetaStar_L1(); }  // = `-CosThetaStar_L2()`
    [[nodiscard]] double CosThetaStar_L2() const { return corr.CosThetaStar_L2(); }  // = `-CosThetaStar_L1()`
    [[nodiscard]] double CosThetaStar_Pr1() const { return corr.CosThetaStar_P1(); }
    [[nodiscard]] double CosThetaStar_Pr2() const { return corr.CosThetaStar_P2(); }
    [[nodiscard]] double Qrel() const { return corr.Qrel(); }  // restated `Fit_Mass()`

    // (anti)lambda 1's fit vs (anti)h-dibaryon fit //
    [[nodiscard]] double L1_FlightLength_WithRespectToDV() const { return fit_l1.FlightLength_WithRespectTo(fit.vtx); }
    [[nodiscard]] double L1_FlightLength_DivByErr_WithRespectToDV() const { return fit_l1.FlightLength_DivByErr_WithRespectTo(fit.vtx, fit.cov_vtx); }
    [[nodiscard]] double L1_DCA_WithRespectToDV() const { return fit_l1.DCA_WithRespectTo(fit.vtx); }
    [[nodiscard]] double L1_CPA_WithRespectToDV() const { return fit_l1.CPA_WithRespectTo(fit.vtx); }

    // (anti)lambda 2's fit vs (anti)h-dibaryon fit //
    [[nodiscard]] double L2_FlightLength_WithRespectToDV() const { return fit_l2.FlightLength_WithRespectTo(fit.vtx); }
    [[nodiscard]] double L2_FlightLength_DivByErr_WithRespectToDV() const { return fit_l2.FlightLength_DivByErr_WithRespectTo(fit.vtx, fit.cov_vtx); }
    [[nodiscard]] double L2_DCA_WithRespectToDV() const { return fit_l2.DCA_WithRespectTo(fit.vtx); }
    [[nodiscard]] double L2_CPA_WithRespectToDV() const { return fit_l2.CPA_WithRespectTo(fit.vtx); }

    // (anti)lambda 1's fit vs (anti)lambda 2's fit ~ duplicate tests //
    [[nodiscard]] double Distance_BetweenLambdasDVs() const { return fit_l1.DistanceFrom(fit_l2.vtx); }
    [[nodiscard]] double Delta_FlightLength_BetweenLambdas() const {
        return std::abs(L1_FlightLength_WithRespectToDV() - L2_FlightLength_WithRespectToDV());
    }

    // post-seed/pre-fit vs post-fit comparisons //
    [[nodiscard]] double MomentumKick() const { return (fit.lv.Vect() - seed.lv.Vect()).R(); }
    [[nodiscard]] double L1_MomentumKick() const { return (fit.lv_dau1.Vect() - seed.lv_dau1.Vect()).R(); }
    [[nodiscard]] double L2_MomentumKick() const { return (fit.lv_dau2.Vect() - seed.lv_dau2.Vect()).R(); }

   private:
    Unit::PostSeed seed;   // (anti)lambdas at their PCAs, same as their own fits
    Unit::PostFit fit;     // (anti)h-dibaryon fit
    Unit::PostFit fit_l1;  // (anti)lambda 1's own fits
    Unit::PostFit fit_l2;  // (anti)lambda 2's own fits
    Unit::Correlations corr;
    RMath::XYZPoint pv;                                             // primary vertex
    std::array<float, Common::NCovMatrixComponents_State3> pv_cov;  // its position covariance
};

}  // namespace Cached
