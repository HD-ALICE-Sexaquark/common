#pragma once

#include <cmath>

#include <Math/Point3Dfwd.h>
#include <Math/Vector3Dfwd.h>
#include <Math/Vector4Dfwd.h>

#include "Constants.hpp"
#include "HD_Library.hpp"
#include "MC_Helpers.hpp"
#include "Math.hpp"
#include "POD_McParticle.hpp"

namespace Cached {

struct McParticle : POD::Extended::McParticle {

    McParticle(const POD::Extended::McParticle& e_mc, const ROOT::Math::XYZPoint& ref, double bz)  //
        : POD::Extended::McParticle(e_mc),
          lv{e_mc.Px, e_mc.Py, e_mc.Pz, e_mc.Energy},
          origin{e_mc.Origin_X, e_mc.Origin_Y, e_mc.Origin_Z},
          decay{e_mc.Decay_X, e_mc.Decay_Y, e_mc.Decay_Z},
          pv{ref} {
        if (Charge != 0) {
            pca_wrt_pv = Common::Math::FastPCA_HelixVertex(lv.Vect(), decay, e_mc.Charge, pv, bz);
        } else {
            pca_wrt_pv = Common::Math::FastPCA_LineVertex(lv.Vect(), decay, pv);
            cpa_wrt_pv = Common::Math::CosinePointingAngle(lv.Vect(), decay, pv);
        }
    }

    static McParticle CreateFromNonExtendedMc(const POD::McParticle& mc_part, const Cached::McExtension& extension, const ROOT::Math::XYZPoint& ref,
                                              double bz) {
        POD::Extended::McParticle ext_mc(mc_part);
        MC::Apply(ext_mc, extension);  // don't worry about `IsHybrid`
        return {ext_mc, ref, bz};
    }

    static McParticle CreateFromV0(const POD::Extended::McParticle& mc_v0, const POD::Extended::McParticle& mc_neg,
                                   const POD::Extended::McParticle& mc_pos, const ROOT::Math::XYZPoint& ref) {
        Cached::McParticle new_v0(mc_v0, ref, 0.);                                // `bz` doesn't matter here
        new_v0.arm_qt = Common::Math::ArmenterosQt(mc_v0.Px, mc_v0.Py, mc_v0.Pz,  //
                                                   mc_neg.Px, mc_neg.Py, mc_neg.Pz);
        new_v0.arm_alpha = Common::Math::ArmenterosAlpha(mc_v0.Px, mc_v0.Py, mc_v0.Pz,     //
                                                         mc_neg.Px, mc_neg.Py, mc_neg.Pz,  //
                                                         mc_pos.Px, mc_pos.Py, mc_pos.Pz)
                               .value_or(Common::DummyDouble);
        return new_v0;
    }

    static McParticle CreateFromHdib(const POD::Extended::McParticle& mc_hdib, const POD::Extended::McParticle& mc_l1,
                                     const POD::Extended::McParticle& mc_l2, const POD::Extended::McParticle& mc_p1,
                                     const POD::Extended::McParticle& mc_p2, const ROOT::Math::XYZPoint& ref) {
        Cached::McParticle new_hdib(mc_hdib, ref, 0.);                                // `bz` doesn't matter here
        auto corr_info = HD::GetAngles(new_hdib.lv,                                   //
                                       {mc_l1.Px, mc_l1.Py, mc_l1.Pz, mc_l1.Energy},  //
                                       {mc_l2.Px, mc_l2.Py, mc_l2.Pz, mc_l2.Energy},  //
                                       {mc_p1.Px, mc_p1.Py, mc_p1.Pz, mc_p1.Energy},  //
                                       {mc_p2.Px, mc_p2.Py, mc_p2.Pz, mc_p2.Energy});
        new_hdib.cos_theta_pp = corr_info.cos_theta_pp;
        new_hdib.theta_pp = corr_info.theta_pp;
        new_hdib.cos_theta_star_l1 = corr_info.cos_theta_star_l1;
        new_hdib.cos_theta_star_l2 = corr_info.cos_theta_star_l2;
        new_hdib.cos_theta_star_p1 = corr_info.cos_theta_star_p1;
        new_hdib.cos_theta_star_p2 = corr_info.cos_theta_star_p2;
        new_hdib.q_rel = corr_info.q_rel;
        return new_hdib;
    }

    // kinematics
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    // geometry
    [[nodiscard]] double Origin_Radius2D() const { return origin.Rho(); }
    [[nodiscard]] double Origin_Radius3D() const { return origin.R(); }
    [[nodiscard]] double Decay_Radius2D() const { return decay.Rho(); }
    [[nodiscard]] double Decay_Radius3D() const { return decay.R(); }
    [[nodiscard]] double DecayLength() const { return (decay - origin).R(); }
    // kinematics + geometry
    [[nodiscard]] double DCAxy_wrt_PV() const { return (pv - pca_wrt_pv).Rho(); }
    [[nodiscard]] double DCAz_wrt_PV() const { return std::abs((pv - pca_wrt_pv).Z()); }
    [[nodiscard]] double DCA_wrt_PV() const { return (pv - pca_wrt_pv).R(); }
    [[nodiscard]] double CPA_wrt_PV() const { return cpa_wrt_pv; }
    // related to daughters
    [[nodiscard]] double ArmQt() const { return arm_qt; }
    [[nodiscard]] double ArmAlpha() const { return arm_alpha; }
    // aliases
    [[nodiscard]] bool IsLogicalPrimary() const { return Mother_McEntry == Common::DummyInt; }
    [[nodiscard]] int ReactionID() const { return SignalID; }
    // correlations
    [[nodiscard]] double CosTheta_pp() const { return cos_theta_pp; }
    [[nodiscard]] double Theta_pp() const { return theta_pp; }
    [[nodiscard]] double CosThetaStar_L1() const { return cos_theta_star_l1; }
    [[nodiscard]] double CosThetaStar_L2() const { return cos_theta_star_l2; }
    [[nodiscard]] double CosThetaStar_Pr1() const { return cos_theta_star_p1; }
    [[nodiscard]] double CosThetaStar_Pr2() const { return cos_theta_star_p2; }
    [[nodiscard]] double Qrel() const { return q_rel; }

   private:
    ROOT::Math::PxPyPzEVector lv;
    ROOT::Math::XYZPoint origin;
    ROOT::Math::XYZPoint decay;
    ROOT::Math::XYZPoint pv;
    ROOT::Math::XYZPoint pca_wrt_pv;
    double cpa_wrt_pv{Common::DummyDouble};  // (neutral particles only)
    double arm_qt{Common::DummyDouble};      // (V0s only)
    double arm_alpha{Common::DummyDouble};   // (V0s only)
    // correlations
    double cos_theta_pp{Common::DummyDouble};       // (injected hdib only)
    double theta_pp{Common::DummyDouble};           // (injected hdib only)
    double cos_theta_star_l1{Common::DummyDouble};  // (injected hdib only)
    double cos_theta_star_l2{Common::DummyDouble};  // (injected hdib only)
    double cos_theta_star_p1{Common::DummyDouble};  // (injected hdib only)
    double cos_theta_star_p2{Common::DummyDouble};  // (injected hdib only)
    double q_rel{Common::DummyDouble};              // (injected hdib only)
};

}  // namespace Cached
