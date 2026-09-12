#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>

#include <Math/GenVector/Boost.h>
#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>
#include <Math/VectorUtil.h>

#include "Constants.hpp"
#include "KF_PostProcessing.hpp"
#include "Math.hpp"

namespace Cached::Unit {

namespace RMath = ROOT::Math;
namespace CMath = Common::Math;

struct PostSeed {
    PostSeed(float dau1_x, float dau1_y, float dau1_z,                   //
             float dau1_px, float dau1_py, float dau1_pz, float dau1_e,  //
             float dau2_x, float dau2_y, float dau2_z,                   //
             float dau2_px, float dau2_py, float dau2_pz, float dau2_e)
        : lv_dau1{dau1_px, dau1_py, dau1_pz, dau1_e},  //
          lv_dau2{dau2_px, dau2_py, dau2_pz, dau2_e},
          lv{lv_dau1 + lv_dau2},
          pca_dau1{dau1_x, dau1_y, dau1_z},  //
          pca_dau2{dau2_x, dau2_y, dau2_z},
          midpoint{CMath::MiddlePoint(pca_dau2, pca_dau1)} {}

    [[nodiscard]] double Px() const { return lv.Px(); }
    [[nodiscard]] double Py() const { return lv.Py(); }
    [[nodiscard]] double Pz() const { return lv.Pz(); }
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Energy() const { return lv.E(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }

    [[nodiscard]] double ArmAlpha() const {  //
        return CMath::ArmenterosAlpha(lv.Vect(), lv_dau1.Vect(), lv_dau2.Vect()).value_or(Common::DummyDouble);
    }
    [[nodiscard]] double ArmQt() const {  //
        return CMath::ArmenterosQt(lv.Vect(), lv_dau1.Vect());
    }

    [[nodiscard]] double DCA_Between() const { return (pca_dau2 - pca_dau1).R(); }
    [[nodiscard]] double DCA_Between_Squared() const { return (pca_dau2 - pca_dau1).Mag2(); }
    [[nodiscard]] double Radius2D() const { return midpoint.Rho(); }
    [[nodiscard]] double Radius2D_Squared() const { return midpoint.Perp2(); }
    [[nodiscard]] double Radius3D() const { return midpoint.R(); }
    [[nodiscard]] double Radius3D_Squared() const { return midpoint.Mag2(); }
    [[nodiscard]] double DistanceFrom(const RMath::XYZPoint& ref) const { return (midpoint - ref).R(); }
    [[nodiscard]] double DistanceFrom_Squared(const RMath::XYZPoint& ref) const { return (midpoint - ref).Mag2(); }

    [[nodiscard]] double Mass_AsK0S() const {
        return Common::Math::InvariantMass(lv_dau1.Px(), lv_dau1.Py(), lv_dau1.Pz(), Common::PdgMass_Pion,  //
                                           lv_dau2.Px(), lv_dau2.Py(), lv_dau2.Pz(), Common::PdgMass_Pion);
    }
    [[nodiscard]] double Mass_AsLambda() const {
        return Common::Math::InvariantMass(lv_dau1.Px(), lv_dau1.Py(), lv_dau1.Pz(), Common::PdgMass_Pion,  //
                                           lv_dau2.Px(), lv_dau2.Py(), lv_dau2.Pz(), Common::PdgMass_Proton);
    }
    [[nodiscard]] double Mass_AsAntiLambda() const {
        return Common::Math::InvariantMass(lv_dau1.Px(), lv_dau1.Py(), lv_dau1.Pz(), Common::PdgMass_Proton,  //
                                           lv_dau2.Px(), lv_dau2.Py(), lv_dau2.Pz(), Common::PdgMass_Pion);
    }
    [[nodiscard]] double Mass_AsPhoton() const {
        return Common::Math::InvariantMass(lv_dau1.Px(), lv_dau1.Py(), lv_dau1.Pz(), Common::PdgMass_Electron,  //
                                           lv_dau2.Px(), lv_dau2.Py(), lv_dau2.Pz(), Common::PdgMass_Electron);
    }

    RMath::PxPyPzEVector lv_dau1;
    RMath::PxPyPzEVector lv_dau2;
    RMath::PxPyPzEVector lv;  // 4-vector sum of the above
    RMath::XYZPoint pca_dau1;
    RMath::XYZPoint pca_dau2;
    RMath::XYZPoint midpoint;  // midpoint of the above
};

struct PostFit {
    PostFit(float dau1_px, float dau1_py, float dau1_pz, float dau1_e,  //
            float dau2_px, float dau2_py, float dau2_pz, float dau2_e,  //
            float x, float y, float z,                                  //
            const std::array<float, Common::NCovMatrixComponents_State3>& cov_xyz)
        : lv_dau1{dau1_px, dau1_py, dau1_pz, dau1_e},
          lv_dau2{dau2_px, dau2_py, dau2_pz, dau2_e},
          lv{lv_dau1 + lv_dau2},
          vtx{x, y, z},
          cov_vtx{cov_xyz} {}

    [[nodiscard]] double Px() const { return lv.Px(); }
    [[nodiscard]] double Py() const { return lv.Py(); }
    [[nodiscard]] double Pz() const { return lv.Pz(); }
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Energy() const { return lv.E(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }

    [[nodiscard]] double ArmAlpha() const {  //
        return CMath::ArmenterosAlpha(lv.Vect(), lv_dau1.Vect(), lv_dau2.Vect()).value_or(Common::DummyDouble);
    }
    [[nodiscard]] double ArmQt() const {  //
        return CMath::ArmenterosQt(lv.Vect(), lv_dau1.Vect());
    }
    [[nodiscard]] double CosTheta_BetweenDaughters() const {  //
        return RMath::VectorUtil::CosTheta(lv_dau1.Vect(), lv_dau2.Vect());
    }
    [[nodiscard]] double Theta_BetweenDaughters() const {  //
        return std::acos(std::clamp(CosTheta_BetweenDaughters(), -1., 1.));
    }

    [[nodiscard]] double Dau1_Pt() const { return lv_dau1.Pt(); }
    [[nodiscard]] double Dau1_Pz() const { return lv_dau1.Pz(); }
    [[nodiscard]] double Dau1_P() const { return lv_dau1.P(); }
    [[nodiscard]] double Dau1_Eta() const { return lv_dau1.Eta(); }
    [[nodiscard]] double Dau1_Rapidity() const { return lv_dau1.Rapidity(); }

    [[nodiscard]] double Dau2_Pt() const { return lv_dau2.Pt(); }
    [[nodiscard]] double Dau2_Pz() const { return lv_dau2.Pz(); }
    [[nodiscard]] double Dau2_P() const { return lv_dau2.P(); }
    [[nodiscard]] double Dau2_Eta() const { return lv_dau2.Eta(); }
    [[nodiscard]] double Dau2_Rapidity() const { return lv_dau2.Rapidity(); }

    // -- vertex
    [[nodiscard]] double Radius2D() const { return vtx.Rho(); }
    [[nodiscard]] double Radius2D_Squared() const { return vtx.Perp2(); }
    [[nodiscard]] double Radius3D() const { return vtx.R(); }
    [[nodiscard]] double Radius3D_Squared() const { return vtx.Mag2(); }

    [[nodiscard]] double DistanceFrom(const RMath::XYZPoint& ref) const { return (vtx - ref).R(); }
    [[nodiscard]] double DistanceFrom_Squared(const RMath::XYZPoint& ref) const { return (vtx - ref).Mag2(); }
    template <std::size_t N>
    [[nodiscard]] double DistanceFrom_Err(const RMath::XYZPoint& ref, const std::array<float, N>& cov_ref) const {
        return CMath::DistanceErr(vtx - ref, cov_vtx, cov_ref).value_or(Common::DummyDouble);
    }
    template <std::size_t N>
    [[nodiscard]] double DistanceFrom_DivByErr(const RMath::XYZPoint& ref, const std::array<float, N>& cov_ref) const {
        return CMath::DivByErr(DistanceFrom(ref), DistanceFrom_Err(ref, cov_ref));
    }

    [[nodiscard]] double DCA_WithRespectTo(const RMath::XYZPoint& ref) const {  //
        return CMath::FastDCA_LineVertex(lv.Vect(), vtx, ref);
    }
    [[nodiscard]] double DCA_WithRespectTo_Squared(const RMath::XYZPoint& ref) const {
        return (CMath::FastPCA_LineVertex(lv.Vect(), vtx, ref) - ref).Mag2();
    }

    [[nodiscard]] double CPA_WithRespectTo(const RMath::XYZPoint& ref) const {  //
        return CMath::CosinePointingAngle(lv.Vect(), vtx, ref);
    }

    [[nodiscard]] double FlightLength_WithRespectTo(const RMath::XYZPoint& ref) const {  //
        return CMath::SignedFlightLength(lv.Vect(), vtx, ref);
    }
    template <std::size_t N>
    [[nodiscard]] double FlightLengthErr_WithRespectTo(const RMath::XYZPoint& /*ref*/, const std::array<float, N>& cov_ref) const {
        return CMath::ProjectedErr(lv.Vect().Unit(), cov_vtx, cov_ref).value_or(Common::DummyDouble);
    }
    template <std::size_t N>
    [[nodiscard]] double FlightLength_DivByErr_WithRespectTo(const RMath::XYZPoint& ref, const std::array<float, N>& cov_ref) const {
        return CMath::DivByErr(FlightLength_WithRespectTo(ref), FlightLengthErr_WithRespectTo(ref, cov_ref));
    }

    template <std::size_t N>
    [[nodiscard]] double Chi2PointCoincidence_WithRespectTo(const RMath::XYZPoint& ref, const std::array<float, N>& cov_ref) const {
        return CMath::Chi2_PointCoincidence(vtx - ref, cov_vtx, cov_ref).value_or(Common::DummyDouble);
    }

    RMath::PxPyPzEVector lv_dau1;
    RMath::PxPyPzEVector lv_dau2;
    RMath::PxPyPzEVector lv;  // 4-vector sum of the above
    RMath::XYZPoint vtx;
    std::array<float, Common::NCovMatrixComponents_State3> cov_vtx;
};

// Angular correlations of a two-step decay `H -> l1 l2 -> p1 ... p2 ... + c.c.`.
struct Correlations {
    Correlations() = default;
    Correlations(const RMath::PxPyPzEVector& h_lab, const RMath::PxPyPzEVector& l1_lab, const RMath::PxPyPzEVector& l2_lab,
                 const RMath::PxPyPzEVector& p1_lab, const RMath::PxPyPzEVector& p2_lab) {
        // 1. boost everything to H rest frame
        const RMath::Boost boost_to_h(h_lab.BoostToCM());
        const RMath::PxPyPzEVector l1_in_h = boost_to_h(l1_lab);
        const RMath::PxPyPzEVector l2_in_h = boost_to_h(l2_lab);
        const RMath::PxPyPzEVector p1_in_h = boost_to_h(p1_lab);
        const RMath::PxPyPzEVector p2_in_h = boost_to_h(p2_lab);
        // 2. boost protons to their respective lambda rest frames
        const RMath::Boost boost_to_l1(l1_in_h.BoostToCM());
        const RMath::Boost boost_to_l2(l2_in_h.BoostToCM());
        const RMath::PxPyPzEVector p1_in_l1 = boost_to_l1(p1_in_h);
        const RMath::PxPyPzEVector p2_in_l2 = boost_to_l2(p2_in_h);
        // -- cache sqrt calls
        const auto u_h = h_lab.Vect().Unit();
        const auto u_l1 = l1_in_h.Vect().Unit();
        const auto u_l2 = l2_in_h.Vect().Unit();
        const auto u_p1 = p1_in_l1.Vect().Unit();
        const auto u_p2 = p2_in_l2.Vect().Unit();
        // -- angle between protons
        cos_theta_pp = u_p1.Dot(u_p2);
        theta_pp = std::acos(std::clamp(cos_theta_pp, -1., 1.));
        // -- angle between lambdas and h-dibaryon
        cos_theta_star_l1 = u_l1.Dot(u_h);
        cos_theta_star_l2 = u_l2.Dot(u_h);
        // -- angle between protons and their respective lambdas
        cos_theta_star_p1 = u_p1.Dot(u_l1);
        cos_theta_star_p2 = u_p2.Dot(u_l2);
        // -- relative momentum
        q_rel = l1_in_h.Vect().R();  // = |l2_in_h|
    }

    static Correlations FromNonClosedTree(const PostFit& fit_h, const PostFit& fit_l1, const PostFit& fit_l2, bool l1_is_anti, bool l2_is_anti) {
        RMath::PxPyPzEVector new_l1 = fit_h.lv_dau1;
        RMath::PxPyPzEVector new_l2 = fit_h.lv_dau2;

        RMath::PxPyPzEVector old_l1 = fit_l1.lv;
        RMath::PxPyPzEVector p1_lab = l1_is_anti ? fit_l1.lv_dau1 : fit_l1.lv_dau2;
        RMath::PxPyPzEVector pi1_lab = l1_is_anti ? fit_l1.lv_dau2 : fit_l1.lv_dau1;

        RMath::PxPyPzEVector old_l2 = fit_l2.lv;
        RMath::PxPyPzEVector p2_lab = l2_is_anti ? fit_l2.lv_dau1 : fit_l2.lv_dau2;
        RMath::PxPyPzEVector pi2_lab = l2_is_anti ? fit_l2.lv_dau2 : fit_l2.lv_dau1;

        Common::PostProcessing::ReattachTo(p1_lab, pi1_lab, Common::PdgMass_Proton, Common::PdgMass_Pion, old_l1, new_l1);
        Common::PostProcessing::ReattachTo(p2_lab, pi2_lab, Common::PdgMass_Proton, Common::PdgMass_Pion, old_l2, new_l2);

        return Correlations{fit_h.lv, new_l1, new_l2, p1_lab, p2_lab};
    }

    [[nodiscard]] double CosTheta_pp() const { return cos_theta_pp; }
    [[nodiscard]] double Theta_pp() const { return theta_pp; }
    [[nodiscard]] double CosThetaStar_L1() const { return cos_theta_star_l1; }
    [[nodiscard]] double CosThetaStar_L2() const { return cos_theta_star_l2; }
    [[nodiscard]] double CosThetaStar_P1() const { return cos_theta_star_p1; }
    [[nodiscard]] double CosThetaStar_P2() const { return cos_theta_star_p2; }
    [[nodiscard]] double Qrel() const { return q_rel; }

    // angle between the two granddaughters, each in its own mother's rest frame (the spin correlation)
    double cos_theta_pp{Common::DummyDouble};
    double theta_pp{Common::DummyDouble};

    // helicity angle of each first-level daughter in `A`'s rest frame (back-to-back by closure, so
    // `cos_theta_star_l2 = -cos_theta_star_l1` exactly)
    double cos_theta_star_l1{Common::DummyDouble};
    double cos_theta_star_l2{Common::DummyDouble};

    // helicity angle of each granddaughter in its own mother's rest frame
    double cos_theta_star_p1{Common::DummyDouble};
    double cos_theta_star_p2{Common::DummyDouble};

    // `|p*|` of the first-level daughters in `A`'s frame; with both on shell it is `A`'s mass restated
    double q_rel{Common::DummyDouble};
};

}  // namespace Cached::Unit
