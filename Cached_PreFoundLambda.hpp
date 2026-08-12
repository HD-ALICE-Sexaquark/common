#pragma once

#include <cmath>

#include <Math/GenVector/Boost.h>
#include <Math/Point3Dfwd.h>
#include <Math/Vector3Dfwd.h>
#include <Math/Vector4Dfwd.h>

#include "Constants.hpp"
#include "DB_Particles.hpp"
#include "Math.hpp"
#include "POD_PreFoundLambda.hpp"

namespace Cached {

struct PreFoundLambda : POD::Extended::PreFoundLambda {

    PreFoundLambda(const POD::Extended::PreFoundLambda& lambda, bool is_anti_channel, const ROOT::Math::XYZPoint& ref)
        : POD::Extended::PreFoundLambda(lambda),
          lv_neg{lambda.Neg_PCAwrtV0_Px, lambda.Neg_PCAwrtV0_Py, lambda.Neg_PCAwrtV0_Pz, lambda.Neg_Energy},
          lv_pos{lambda.Pos_PCAwrtV0_Px, lambda.Pos_PCAwrtV0_Py, lambda.Pos_PCAwrtV0_Pz, lambda.Pos_Energy},
          lv{lambda.Px, lambda.Py, lambda.Pz, lambda.Energy},
          v0{lambda.Decay_X, lambda.Decay_Y, lambda.Decay_Z},
          pv{ref},
          pca_wrt_pv{Common::Math::FastPCA_LineVertex(lv.Vect(), v0, pv)},
          neg_pca_wrt_v0{lambda.Neg_PCAwrtV0_X, lambda.Neg_PCAwrtV0_Y, lambda.Neg_PCAwrtV0_Z},
          pos_pca_wrt_v0{lambda.Pos_PCAwrtV0_X, lambda.Pos_PCAwrtV0_Y, lambda.Pos_PCAwrtV0_Z},
          cpa_wrt_pv{Common::Math::CosinePointingAngle(lv.Vect(), v0, pv)},
          arm_alpha{Common::Math::ArmenterosAlpha(lv.Vect(), lv_neg.Vect(), lv_pos.Vect()).value_or(Common::DummyFloat)},
          arm_qt{Common::Math::ArmenterosQt(lv.Vect(), lv_neg.Vect())},
          arm_radius_dev{Common::Math::ArmenterosEllipticRadius(
                             arm_alpha, arm_qt, DB::Particles::Particle("Lambda").mass,                                               //
                             is_anti_channel ? DB::Particles::Particle("AntiProton").mass : DB::Particles::Particle("PiMinus").mass,  //
                             is_anti_channel ? DB::Particles::Particle("PiPlus").mass : DB::Particles::Particle("Proton").mass,       //
                             lv.P()) -
                         1.},
          anti_channel{is_anti_channel} {}

    static PreFoundLambda CreateFromNonExtended(const POD::PreFoundLambda& non_ext_lambda, bool is_anti_channel, const ROOT::Math::XYZPoint& ref) {
        // NOTE: same hypotheses as the constructor above -- daughters are ordered by charge, so the roles swap
        double mass_neg = is_anti_channel ? DB::Particles::Particle("AntiProton").mass : DB::Particles::Particle("PiMinus").mass;
        double mass_pos = is_anti_channel ? DB::Particles::Particle("PiPlus").mass : DB::Particles::Particle("Proton").mass;
        double energy_neg =
            Common::Math::Hypot4(non_ext_lambda.Neg_PCAwrtV0_Px, non_ext_lambda.Neg_PCAwrtV0_Py, non_ext_lambda.Neg_PCAwrtV0_Pz, mass_neg);
        double energy_pos =
            Common::Math::Hypot4(non_ext_lambda.Pos_PCAwrtV0_Px, non_ext_lambda.Pos_PCAwrtV0_Py, non_ext_lambda.Pos_PCAwrtV0_Pz, mass_pos);
        POD::Extended::PreFoundLambda ext_lambda{non_ext_lambda,
                                                 non_ext_lambda.Neg_PCAwrtV0_Px + non_ext_lambda.Pos_PCAwrtV0_Px,
                                                 non_ext_lambda.Neg_PCAwrtV0_Py + non_ext_lambda.Pos_PCAwrtV0_Py,
                                                 non_ext_lambda.Neg_PCAwrtV0_Pz + non_ext_lambda.Pos_PCAwrtV0_Pz,
                                                 static_cast<float>(energy_neg + energy_pos),
                                                 {},  // empty cov. matrix
                                                 Common::DummyFloat,
                                                 Common::DummyFloat,
                                                 Common::DummyFloat,
                                                 Common::DummyFloat,
                                                 static_cast<float>(energy_neg),
                                                 Common::DummyFloat,
                                                 Common::DummyFloat,
                                                 Common::DummyFloat,
                                                 static_cast<float>(energy_pos)};
        return {ext_lambda, is_anti_channel, ref};
    }

    // (anti)lambda candidate //
    // -- kinematics
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    [[nodiscard]] double Energy() const { return lv.E(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    // -- geometry
    [[nodiscard]] double Decay_Radius2D() const { return v0.Rho(); }
    [[nodiscard]] double Decay_Radius3D() const { return v0.R(); }
    // -- kinematics + geometry
    [[nodiscard]] double DCAxy_wrt_PV() const { return (pca_wrt_pv - pv).Rho(); }
    [[nodiscard]] double DCAz_wrt_PV() const { return std::abs((pca_wrt_pv - pv).Z()); }
    [[nodiscard]] double DCA_wrt_PV() const { return (pca_wrt_pv - pv).R(); }
    [[nodiscard]] double CPA_wrt_PV() const { return cpa_wrt_pv; }
    // armenteros-podolanski
    [[nodiscard]] double ArmAlpha() const { return arm_alpha; }
    [[nodiscard]] double ArmQt() const { return arm_qt; }
    [[nodiscard]] double ArmRadiusDev() const { return arm_radius_dev; }
    // correlations
    [[nodiscard]] double Proton_CosThetaStar() const {
        // angle between proton and their respective lambdas in the lambda r.f.
        auto boost_to_l1_rf = ROOT::Math::Boost(lv.BoostToCM());
        auto p1_in_l1_rf = boost_to_l1_rf(anti_channel ? lv_neg : lv_pos);  // PENDING: maybe could polish
        auto u_l1 = lv.Vect().Unit();
        auto u_p1 = p1_in_l1_rf.Vect().Unit();
        return u_p1.Dot(u_l1);
    }

    // (anti)proton //
    [[nodiscard]] double Pr_Pz() const { return anti_channel ? lv_neg.Pz() : lv_pos.Pz(); }
    [[nodiscard]] double Pr_Pt() const { return anti_channel ? lv_neg.Pt() : lv_pos.Pt(); }
    [[nodiscard]] double Pr_P() const { return anti_channel ? lv_neg.P() : lv_pos.P(); }
    [[nodiscard]] double Pr_Eta() const { return anti_channel ? lv_neg.Eta() : lv_pos.Eta(); }
    [[nodiscard]] double Pr_PreDCAxy() const { return static_cast<double>(anti_channel ? Neg_PreDCAxy : Pos_PreDCAxy); }
    [[nodiscard]] double Pr_PreDCAz() const { return static_cast<double>(anti_channel ? Neg_PreDCAz : Pos_PreDCAz); }
    [[nodiscard]] double Pr_NSigmasPion() const { return static_cast<double>(anti_channel ? Neg_NSigmasPion : Pos_NSigmasPion); }
    [[nodiscard]] double Pr_NSigmasKaon() const { return static_cast<double>(anti_channel ? Neg_NSigmasKaon : Pos_NSigmasKaon); }
    [[nodiscard]] double Pr_NSigmasProton() const { return static_cast<double>(anti_channel ? Neg_NSigmasProton : Pos_NSigmasProton); }
    // -- related to (anti)lambda
    [[nodiscard]] double Pr_DCA_wrt_V0() const { return anti_channel ? (v0 - neg_pca_wrt_v0).R() : (v0 - pos_pca_wrt_v0).R(); }

    // pion (plus/minus) //
    [[nodiscard]] double Pi_Pz() const { return anti_channel ? lv_pos.Pz() : lv_neg.Pz(); }
    [[nodiscard]] double Pi_Pt() const { return anti_channel ? lv_pos.Pt() : lv_neg.Pt(); }
    [[nodiscard]] double Pi_P() const { return anti_channel ? lv_pos.P() : lv_neg.P(); }
    [[nodiscard]] double Pi_Eta() const { return anti_channel ? lv_pos.Eta() : lv_neg.Eta(); }
    [[nodiscard]] double Pi_PreDCAxy() const { return static_cast<double>(anti_channel ? Pos_PreDCAxy : Neg_PreDCAxy); }
    [[nodiscard]] double Pi_PreDCAz() const { return static_cast<double>(anti_channel ? Pos_PreDCAz : Neg_PreDCAz); }
    [[nodiscard]] double Pi_NSigmasPion() const { return static_cast<double>(anti_channel ? Pos_NSigmasPion : Neg_NSigmasPion); }
    [[nodiscard]] double Pi_NSigmasKaon() const { return static_cast<double>(anti_channel ? Pos_NSigmasKaon : Neg_NSigmasKaon); }
    [[nodiscard]] double Pi_NSigmasProton() const { return static_cast<double>(anti_channel ? Pos_NSigmasProton : Neg_NSigmasProton); }
    // -- related to (anti)lambda
    [[nodiscard]] double Pi_DCA_wrt_V0() const { return anti_channel ? (v0 - pos_pca_wrt_v0).R() : (v0 - neg_pca_wrt_v0).R(); }

    // negative daughter //
    [[nodiscard]] double Neg_Pt() const { return lv_neg.Pt(); }
    [[nodiscard]] double Neg_P() const { return lv_neg.P(); }

    // positive daughter //
    [[nodiscard]] double Pos_Pt() const { return lv_pos.Pt(); }
    [[nodiscard]] double Pos_P() const { return lv_pos.P(); }

   private:
    ROOT::Math::PxPyPzEVector lv_neg;     // lorentz vector of negative daughter
    ROOT::Math::PxPyPzEVector lv_pos;     // lorentz vector of positive daughter
    ROOT::Math::PxPyPzEVector lv;         // lorentz vector of v0
    ROOT::Math::XYZPoint v0;              // decay vertex
    ROOT::Math::XYZPoint pv;              // primary vertex
    ROOT::Math::XYZPoint pca_wrt_pv;      // point of closest approach w.r.t. primary vertex
    ROOT::Math::XYZPoint neg_pca_wrt_v0;  // pca(neg) w.r.t. v0
    ROOT::Math::XYZPoint pos_pca_wrt_v0;  // pca(pos) w.r.t. v0
    double cpa_wrt_pv;                    // cosine pointing angle w.r.t. primary vertex
    double arm_alpha;                     // armenteros-podolanki's variable alpha
    double arm_qt;                        // armenteros-podolanki's variable q_t
    double arm_radius_dev;                // deviation from armenteros-podolanki's elliptic radius = 1.
    bool anti_channel;
};

}  // namespace Cached
