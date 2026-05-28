#pragma once

#include <cstddef>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "DB_Particles.hpp"
#include "Math.hpp"
#include "VC_OnTheFlyLambda.hpp"

namespace Vector {

struct OnTheFlyLambdaView {

    OnTheFlyLambdaView() = delete;
    explicit OnTheFlyLambdaView(const Vector::OnTheFlyLambda* source, std::size_t entry = 0) : Source{source}, Entry{entry} {}

    // # Manipulation # //

    [[nodiscard]] std::size_t Size() const { return (*Source->OnTheFlyEntry).size(); }

    void CacheCalculations(std::size_t entry, const ROOT::Math::XYZPoint& pv, const DB::Particles::Definition& pid_neg,
                           const DB::Particles::Definition& pid_pos) {
        namespace RMath = ROOT::Math;
        namespace CMath = Common::Math;
        Entry = entry;
        // -- negative daughter
        double neg_mass = pid_neg.mass;
        double neg_px = Neg_PCAwrtV0_Px();
        double neg_py = Neg_PCAwrtV0_Py();
        double neg_pz = Neg_PCAwrtV0_Pz();
        double neg_p = std::sqrt(neg_px * neg_px + neg_py * neg_py + neg_pz * neg_pz);
        double neg_energy = std::sqrt(neg_p * neg_p + neg_mass * neg_mass);
        neg_lv.SetCoordinates(neg_px, neg_py, neg_pz, neg_energy);
        // -- positive daughter
        double pos_mass = pid_pos.mass;
        double pos_px = Pos_PCAwrtV0_Px();
        double pos_py = Pos_PCAwrtV0_Py();
        double pos_pz = Pos_PCAwrtV0_Pz();
        double pos_p = std::sqrt(pos_px * pos_px + pos_py * pos_py + pos_pz * pos_pz);
        double pos_energy = std::sqrt(pos_p * pos_p + pos_mass * pos_mass);
        pos_lv.SetCoordinates(pos_px, pos_py, pos_pz, pos_energy);
        // -- update momentum and decay vertex
        lv.SetPxPyPzE(Px(), Py(), Pz(), neg_energy + pos_energy);
        decay_xyz.SetCoordinates(Decay_X(), Decay_Y(), Decay_Z());
        // -- update pca w.r.t. pv
        pca_wrt_pv = CMath::FastPCA_LineVertex(lv.Vect(), decay_xyz, pv);
        // -- update difference between pca and pv
        dist_pca2pv = pca_wrt_pv - pv;
        // -- update CPA wrt pv
        cpa_wrt_pv = CMath::CosinePointingAngle(lv.Vect(), decay_xyz, pv);
        // -- update armenteros-podolanski variables
        arm_qt = CMath::ArmenterosQt(lv.Vect(), neg_lv.Vect());
        arm_alpha = CMath::ArmenterosAlpha(lv.Vect(), neg_lv.Vect(), pos_lv.Vect());
        // -- update cos(theta*) for each daughter
        const auto& daughter_proton = std::abs(pid_neg.pdg_code) == DB::Particles::Particle("Proton").pdg_code ? pid_neg : pid_pos;
        auto to_lambda = RMath::Boost(lv.BoostToCM());
        auto proton_in_lrf = to_lambda(daughter_proton.charge < 0 ? neg_lv : pos_lv);
        proton_cos_theta_star = proton_in_lrf.Vect().Unit().Dot(lv.Vect().Unit());
    }

    // Getters //
    [[nodiscard]] unsigned int OnTheFlyEntry() const { return (*Source->OnTheFlyEntry)[Entry]; }
    [[nodiscard]] float Decay_X() const { return (*Source->Decay_X)[Entry]; }
    [[nodiscard]] float Decay_Y() const { return (*Source->Decay_Y)[Entry]; }
    [[nodiscard]] float Decay_Z() const { return (*Source->Decay_Z)[Entry]; }
    [[nodiscard]] float Px() const { return (*Source->Px)[Entry]; }
    [[nodiscard]] float Py() const { return (*Source->Py)[Entry]; }
    [[nodiscard]] float Pz() const { return (*Source->Pz)[Entry]; }
    [[nodiscard]] float DcaV0Daughters() const { return (*Source->DcaV0Daughters)[Entry]; }
    // -- negative daughter
    [[nodiscard]] unsigned int Neg_EsdEntry() const { return (*Source->Neg_EsdEntry)[Entry]; }
    [[nodiscard]] float Neg_PCAwrtV0_Px() const { return (*Source->Neg_PCAwrtV0_Px)[Entry]; }
    [[nodiscard]] float Neg_PCAwrtV0_Py() const { return (*Source->Neg_PCAwrtV0_Py)[Entry]; }
    [[nodiscard]] float Neg_PCAwrtV0_Pz() const { return (*Source->Neg_PCAwrtV0_Pz)[Entry]; }
    [[nodiscard]] float Neg_PreDCAxy() const { return (*Source->Neg_PreDCAxy)[Entry]; }
    [[nodiscard]] float Neg_PreDCAz() const { return (*Source->Neg_PreDCAz)[Entry]; }
    [[nodiscard]] float Neg_NSigmaProton() const { return (*Source->Neg_NSigmaProton)[Entry]; }
    [[nodiscard]] float Neg_NSigmaKaon() const { return (*Source->Neg_NSigmaKaon)[Entry]; }
    [[nodiscard]] float Neg_NSigmaPion() const { return (*Source->Neg_NSigmaPion)[Entry]; }
    [[nodiscard]] unsigned int Neg_McEntry() const { return (*Source->Neg_McEntry)[Entry]; }
    // -- positive daughter
    [[nodiscard]] unsigned int Pos_EsdEntry() const { return (*Source->Pos_EsdEntry)[Entry]; }
    [[nodiscard]] float Pos_PCAwrtV0_Px() const { return (*Source->Pos_PCAwrtV0_Px)[Entry]; }
    [[nodiscard]] float Pos_PCAwrtV0_Py() const { return (*Source->Pos_PCAwrtV0_Py)[Entry]; }
    [[nodiscard]] float Pos_PCAwrtV0_Pz() const { return (*Source->Pos_PCAwrtV0_Pz)[Entry]; }
    [[nodiscard]] float Pos_PreDCAxy() const { return (*Source->Pos_PreDCAxy)[Entry]; }
    [[nodiscard]] float Pos_PreDCAz() const { return (*Source->Pos_PreDCAz)[Entry]; }
    [[nodiscard]] float Pos_NSigmaProton() const { return (*Source->Pos_NSigmaProton)[Entry]; }
    [[nodiscard]] float Pos_NSigmaKaon() const { return (*Source->Pos_NSigmaKaon)[Entry]; }
    [[nodiscard]] float Pos_NSigmaPion() const { return (*Source->Pos_NSigmaPion)[Entry]; }
    [[nodiscard]] unsigned int Pos_McEntry() const { return (*Source->Pos_McEntry)[Entry]; }

    // Derived Info //
    [[nodiscard]] double Decay_Radius2D() const { return decay_xyz.Rho(); }
    [[nodiscard]] double Decay_Radius3D() const { return decay_xyz.R(); }
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    [[nodiscard]] double Energy() const { return lv.E(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double ArmQt() const { return arm_qt; }
    [[nodiscard]] double ArmAlpha() const { return arm_alpha; }
    [[nodiscard]] double CPAwrtPV() const { return cpa_wrt_pv; }
    [[nodiscard]] double DCAwrtPV() const { return dist_pca2pv.R(); }
    // -- negative daughter
    [[nodiscard]] double Neg_P() const { return neg_lv.P(); }
    [[nodiscard]] double Neg_Pt() const { return neg_lv.Pt(); }
    // -- positive daughter
    [[nodiscard]] double Pos_P() const { return pos_lv.P(); }
    [[nodiscard]] double Pos_Pt() const { return pos_lv.Pt(); }
    // -- whichever is the (anti)proton
    [[nodiscard]] double Proton_CosThetaStar() const { return proton_cos_theta_star; }

    // cached variables //
    ROOT::Math::PxPyPzEVector lv;
    ROOT::Math::PxPyPzEVector neg_lv;
    ROOT::Math::PxPyPzEVector pos_lv;
    ROOT::Math::XYZPoint decay_xyz;
    ROOT::Math::XYZVector dist_pca2pv;
    ROOT::Math::XYZPoint pca_wrt_pv;
    double cpa_wrt_pv{};
    double arm_qt{};
    double arm_alpha{};
    double proton_cos_theta_star{};
    // main variables //
    const Vector::OnTheFlyLambda* Source;
    std::size_t Entry{};
};

}  // namespace Vector
