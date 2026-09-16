#pragma once

#include <array>
#include <cmath>
#include <cstdint>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Cached_Unit.hpp"
#include "Constants.hpp"
#include "DB_SexaquarkMasses.hpp"
#include "Math.hpp"
#include "POD_Sexaquark.hpp"

namespace Cached {

namespace RMath = ROOT::Math;
namespace CMath = Common::Math;

struct Sexaquark : POD::Sexaquark {

    Sexaquark(const POD::Sexaquark& sexa, double nucleon_mass, const RMath::XYZPoint& ref,
              const std::array<float, Common::NCovMatrixComponents_State3>& ref_cov)
        : POD::Sexaquark(sexa),
          seed{Seed_Dau1_X, Seed_Dau1_Y, Seed_Dau1_Z, Seed_Dau1_Px, Seed_Dau1_Py, Seed_Dau1_Pz, Seed_Dau1_Energy,  //
               Seed_Dau2_X, Seed_Dau2_Y, Seed_Dau2_Z, Seed_Dau2_Px, Seed_Dau2_Py, Seed_Dau2_Pz, Seed_Dau2_Energy},
          fit{Fit_Dau1_Px, Fit_Dau1_Py, Fit_Dau1_Pz, Fit_Dau1_Energy,  //
              Fit_Dau2_Px, Fit_Dau2_Py, Fit_Dau2_Pz, Fit_Dau2_Energy,  //
              Fit_X,       Fit_Y,       Fit_Z,       SV_CovMatrix},
          pv{ref},
          pv_cov{ref_cov},
          axis{(fit.vtx - pv).Unit()},
          m_nucleon{nucleon_mass},
          lv_nar{CMath::NucleonAtRest(fit.lv, m_nucleon)},
          lv_naa{CMath::NucleonAlongAxis(fit.lv, axis, m_nucleon)} {}

    // post-seed/pre-fit //
    // -- geometry
    [[nodiscard]] double Seed_DCA_BetweenDaughters() const { return seed.DCA_Between(); }
    [[nodiscard]] double Seed_DCA_BetweenDaughters_Squared() const { return seed.DCA_Between_Squared(); }
    [[nodiscard]] double Seed_Radius2D() const { return seed.Radius2D(); }
    [[nodiscard]] double Seed_Radius2D_Squared() const { return seed.Radius2D_Squared(); }
    // -- kinematics (of the products' sum)
    [[nodiscard]] double Seed_Mass() const { return seed.Mass(); }
    [[nodiscard]] double Seed_Pt() const { return seed.Pt(); }

    // post-fit //
    // -- geometry
    [[nodiscard]] double SV_Radius2D() const { return fit.Radius2D(); }
    [[nodiscard]] double SV_Radius2D_Squared() const { return fit.Radius2D_Squared(); }
    [[nodiscard]] double SV_Radius3D() const { return fit.Radius3D(); }
    [[nodiscard]] double SV_Radius3D_Squared() const { return fit.Radius3D_Squared(); }
    [[nodiscard]] double FlightLength_WithRespectToPV() const { return fit.DistanceFrom(pv); }
    [[nodiscard]] double FlightLength_DivByErr_WithRespectToPV() const { return fit.DistanceFrom_DivByErr(pv, pv_cov); }
    // -- kinematics (of the products' sum, affected by Fermi motion)
    [[nodiscard]] double Fit_Pt() const { return fit.Pt(); }
    [[nodiscard]] double Fit_Pz() const { return fit.Pz(); }
    [[nodiscard]] double Fit_P() const { return fit.P(); }
    [[nodiscard]] double Fit_Eta() const { return fit.Eta(); }
    [[nodiscard]] double Fit_Phi() const { return fit.Phi(); }
    [[nodiscard]] double Fit_Mass() const { return fit.Mass(); }
    [[nodiscard]] double Fit_Rapidity() const { return fit.Rapidity(); }
    [[nodiscard]] double Fit_CosTheta_BetweenDaughters() const { return fit.CosTheta_BetweenDaughters(); }  // restated `Fit_Mass()`
    [[nodiscard]] double Fit_Theta_BetweenDaughters() const { return fit.Theta_BetweenDaughters(); }        // restated `Fit_CosTheta()`
    // -- daughter 1's kinematics
    [[nodiscard]] double Fit_Dau1_Pt() const { return fit.Dau1_Pt(); }
    [[nodiscard]] double Fit_Dau1_Eta() const { return fit.Dau1_Eta(); }
    // -- daughter 2's kinematics
    [[nodiscard]] double Fit_Dau2_Pt() const { return fit.Dau2_Pt(); }
    [[nodiscard]] double Fit_Dau2_Eta() const { return fit.Dau2_Eta(); }
    // -- kinematics + geometry
    [[nodiscard]] double Ql_WithRespectToPV() const { return fit.lv.Vect().Dot(axis); }        // products' total momentum along flight direction
    [[nodiscard]] double Qt_WithRespectToPV() const { return fit.lv.Vect().Cross(axis).R(); }  // products' total momentum across flight direction
    [[nodiscard]] double DCA_WithRespectToPV() const { return fit.DCA_WithRespectTo(pv); }     // = FlightLength * Qt / Fit_P
    [[nodiscard]] double CPA_WithRespectToPV() const { return fit.CPA_WithRespectTo(pv); }     // = Ql / Fit_P
    // -- kinematics assuming a struck-nucleon at rest
    [[nodiscard]] double NucleonAtRest_Mass() const { return lv_nar.M(); }
    [[nodiscard]] double NucleonAtRest_Pt() const { return lv_nar.Pt(); }
    [[nodiscard]] double NucleonAtRest_Rapidity() const { return lv_nar.Rapidity(); }
    // -- kinematics after extracted the estimated perp. component from the nucleon's Fermi motion
    [[nodiscard]] double NucleonAlongAxis_Mass() const { return lv_naa.M(); }
    [[nodiscard]] double NucleonAlongAxis_Pt() const { return lv_naa.Pt(); }
    [[nodiscard]] double NucleonAlongAxis_Rapidity() const { return lv_naa.Rapidity(); }
    // -- the missing nucleon momentum this candidate needs to be a projectile of mass `m` annihilating on the struck
    //    nucleon (`Dummy` below threshold): Fermi-bounded for signal, arbitrary for background; a mass window that
    //    scales with `p_S`
    [[nodiscard]] double RequiredNucleonMomentum_1730MeV() const {
        return CMath::RequiredNucleonMomentum(fit.lv, axis, m_nucleon, DB::SexaquarkMasses::Value[0]);
    }
    [[nodiscard]] double RequiredNucleonMomentum_1800MeV() const {
        return CMath::RequiredNucleonMomentum(fit.lv, axis, m_nucleon, DB::SexaquarkMasses::Value[1]);
    }
    [[nodiscard]] double RequiredNucleonMomentum_1870MeV() const {
        return CMath::RequiredNucleonMomentum(fit.lv, axis, m_nucleon, DB::SexaquarkMasses::Value[2]);
    }
    [[nodiscard]] double RequiredNucleonMomentum_1940MeV() const {
        return CMath::RequiredNucleonMomentum(fit.lv, axis, m_nucleon, DB::SexaquarkMasses::Value[3]);
    }
    [[nodiscard]] double RequiredNucleonMomentum_2010MeV() const {
        return CMath::RequiredNucleonMomentum(fit.lv, axis, m_nucleon, DB::SexaquarkMasses::Value[4]);
    }
    [[nodiscard]] double RequiredNucleonMomentum_AsAntiNeutron() const {
        return CMath::RequiredNucleonMomentum(fit.lv, axis, m_nucleon, Common::PdgMass_Neutron);
    }
    [[nodiscard]] double RequiredNucleonMomentum_AsAntiDeuteron() const {
        return CMath::RequiredNucleonMomentum(fit.lv, axis, m_nucleon, Common::PdgMass_Deuteron);
    }
    // -- fit bookkeeping
    [[nodiscard]] bool AreDaughtersPinned() const {
        constexpr std::uint8_t both = Common::FitFlag::kDau1Pinned | Common::FitFlag::kDau2Pinned;
        return (FitFlags & both) == both;
    }
    [[nodiscard]] double Chi2NDF() const { return static_cast<double>(Chi2) / static_cast<double>(Common::NDF::kVertex); }
    [[nodiscard]] double Chi2Vtx() const { return static_cast<double>(Chi2); }  // alias

    // post-seed/pre-fit vs post-fit comparisons //
    [[nodiscard]] double MomentumKick() const { return (fit.lv.Vect() - seed.lv.Vect()).R(); }
    [[nodiscard]] double Dau1_MomentumKick() const { return (fit.lv_dau1.Vect() - seed.lv_dau1.Vect()).R(); }
    [[nodiscard]] double Dau2_MomentumKick() const { return (fit.lv_dau2.Vect() - seed.lv_dau2.Vect()).R(); }

   protected:
    Unit::PostSeed seed;
    Unit::PostFit fit;
    RMath::XYZPoint pv;                                             // primary vertex
    std::array<float, Common::NCovMatrixComponents_State3> pv_cov;  // its position covariance
    RMath::XYZVector axis;                                          // unit vector along the flight direction, primary -> secondary vertex
    double m_nucleon;                                               // mass of the struck nucleon, set by the channel
    RMath::PxPyPzEVector lv_nar;                                    // antisexaquark if the struck nucleon was at rest
    RMath::PxPyPzEVector lv_naa;                                    // antisexaquark if the struck nucleon carried the transverse momentum
};

}  // namespace Cached
