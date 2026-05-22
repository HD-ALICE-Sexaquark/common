#pragma once

#include <cstddef>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "DB_Particles.hpp"
#include "E2R_Lambda.hpp"
#include "Math.hpp"

namespace E2R {

struct LambdaView {

    LambdaView() = delete;
    LambdaView(const E2R::Lambda* source, std::size_t entry) : Source{source}, Entry{entry} {}

    // Manipulation //
    void Update(std::size_t entry, const ROOT::Math::XYZPoint& pv, const DB::Particles::Definition& particle_def) {
        namespace RMath = ROOT::Math;
        namespace CMath = Common::Math;
        Entry = entry;
        // -- update daughters' momenta @ PCA and PCAs
        auto daughter_pdg_codes = particle_def.daughters_pdg;
        auto daughter0 = DB::Particles::FindParticle(daughter_pdg_codes[0]);
        auto daughter1 = DB::Particles::FindParticle(daughter_pdg_codes[1]);
        //   -- negative daughter
        double neg_mass = daughter0->charge < 0 ? daughter0->mass : daughter1->mass;
        double neg_px = GetNeg_PCAwrtV0_Px();
        double neg_py = GetNeg_PCAwrtV0_Py();
        double neg_pz = GetNeg_PCAwrtV0_Pz();
        double neg_p = std::sqrt(neg_px * neg_px + neg_py * neg_py + neg_pz * neg_pz);
        double neg_energy = std::sqrt(neg_p * neg_p + neg_mass * neg_mass);
        neg_lv.SetCoordinates(neg_px, neg_py, neg_pz, neg_energy);
        //   -- positive daughter
        double pos_mass = daughter0->charge > 0 ? daughter0->mass : daughter1->mass;
        double pos_px = GetPos_PCAwrtV0_Px();
        double pos_py = GetPos_PCAwrtV0_Py();
        double pos_pz = GetPos_PCAwrtV0_Pz();
        double pos_p = std::sqrt(pos_px * pos_px + pos_py * pos_py + pos_pz * pos_pz);
        double pos_energy = std::sqrt(pos_p * pos_p + pos_mass * pos_mass);
        pos_lv.SetCoordinates(pos_px, pos_py, pos_pz, pos_energy);
        // -- update momentum and decay vertex
        lv.SetPxPyPzE(GetPx(), GetPy(), GetPz(), neg_energy + pos_energy);
        decay_xyz.SetCoordinates(GetDecay_X(), GetDecay_Y(), GetDecay_Z());
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
        auto& daughter_proton = std::abs(daughter0->pdg_code) == DB::Particles::Particle("Proton").pdg_code ? daughter0 : daughter1;
        auto to_lambda = RMath::Boost(lv.BoostToCM());
        auto neg_in_lrf = to_lambda(daughter_proton->charge < 0 ? neg_lv : pos_lv);
        proton_cos_theta_star = neg_in_lrf.Vect().Unit().Dot(lv.Vect().Unit());
    }
    [[nodiscard]] std::size_t GetSize() const { return (*Source->EsdEntry).size(); }

    // Getters //
    [[nodiscard]] unsigned int GetEsdEntry() const { return (*Source->EsdEntry)[Entry]; }
    [[nodiscard]] float GetDecay_X() const { return (*Source->Decay_X)[Entry]; }
    [[nodiscard]] float GetDecay_Y() const { return (*Source->Decay_Y)[Entry]; }
    [[nodiscard]] float GetDecay_Z() const { return (*Source->Decay_Z)[Entry]; }
    [[nodiscard]] float GetPx() const { return (*Source->Px)[Entry]; }
    [[nodiscard]] float GetPy() const { return (*Source->Py)[Entry]; }
    [[nodiscard]] float GetPz() const { return (*Source->Pz)[Entry]; }
    [[nodiscard]] float GetDcaV0Daughters() const { return (*Source->DcaV0Daughters)[Entry]; }
    // -- negative daughter
    [[nodiscard]] unsigned int GetNeg_EsdEntry() const { return (*Source->Neg_EsdEntry)[Entry]; }
    [[nodiscard]] float GetNeg_PCAwrtV0_Px() const { return (*Source->Neg_PCAwrtV0_Px)[Entry]; }
    [[nodiscard]] float GetNeg_PCAwrtV0_Py() const { return (*Source->Neg_PCAwrtV0_Py)[Entry]; }
    [[nodiscard]] float GetNeg_PCAwrtV0_Pz() const { return (*Source->Neg_PCAwrtV0_Pz)[Entry]; }
    [[nodiscard]] float GetNeg_PreDCAxy() const { return (*Source->Neg_PreDCAxy)[Entry]; }
    [[nodiscard]] float GetNeg_PreDCAz() const { return (*Source->Neg_PreDCAz)[Entry]; }
    [[nodiscard]] float GetNeg_NSigmaProton() const { return (*Source->Neg_NSigmaProton)[Entry]; }
    [[nodiscard]] float GetNeg_NSigmaKaon() const { return (*Source->Neg_NSigmaKaon)[Entry]; }
    [[nodiscard]] float GetNeg_NSigmaPion() const { return (*Source->Neg_NSigmaPion)[Entry]; }
    // -- positive daughter
    [[nodiscard]] unsigned int GetPos_EsdEntry() const { return (*Source->Pos_EsdEntry)[Entry]; }
    [[nodiscard]] float GetPos_PCAwrtV0_Px() const { return (*Source->Pos_PCAwrtV0_Px)[Entry]; }
    [[nodiscard]] float GetPos_PCAwrtV0_Py() const { return (*Source->Pos_PCAwrtV0_Py)[Entry]; }
    [[nodiscard]] float GetPos_PCAwrtV0_Pz() const { return (*Source->Pos_PCAwrtV0_Pz)[Entry]; }
    [[nodiscard]] float GetPos_PreDCAxy() const { return (*Source->Pos_PreDCAxy)[Entry]; }
    [[nodiscard]] float GetPos_PreDCAz() const { return (*Source->Pos_PreDCAz)[Entry]; }
    [[nodiscard]] float GetPos_NSigmaProton() const { return (*Source->Pos_NSigmaProton)[Entry]; }
    [[nodiscard]] float GetPos_NSigmaKaon() const { return (*Source->Pos_NSigmaKaon)[Entry]; }
    [[nodiscard]] float GetPos_NSigmaPion() const { return (*Source->Pos_NSigmaPion)[Entry]; }

    // Derived Info //
    [[nodiscard]] double DeriveDecay_Radius2D() const { return decay_xyz.Rho(); }
    [[nodiscard]] double DeriveDecay_Radius3D() const { return decay_xyz.R(); }
    [[nodiscard]] double DerivePt() const { return lv.Pt(); }
    [[nodiscard]] double DeriveP() const { return lv.P(); }
    [[nodiscard]] double DeriveEta() const { return lv.Eta(); }
    [[nodiscard]] double DerivePhi() const { return lv.Phi(); }
    [[nodiscard]] double DeriveEnergy() const { return lv.E(); }
    [[nodiscard]] double DeriveMass() const { return lv.M(); }
    [[nodiscard]] double DeriveRapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double DeriveArmQt() const { return arm_qt; }
    [[nodiscard]] double DeriveArmAlpha() const { return arm_alpha; }
    [[nodiscard]] double DeriveCPAwrtPV() const { return cpa_wrt_pv; }
    [[nodiscard]] double DeriveDCAwrtPV() const { return dist_pca2pv.R(); }
    // -- negative daughter
    [[nodiscard]] double DeriveNeg_P() const { return neg_lv.P(); }
    [[nodiscard]] double DeriveNeg_Pt() const { return neg_lv.Pt(); }
    // -- positive daughter
    [[nodiscard]] double DerivePos_P() const { return pos_lv.P(); }
    [[nodiscard]] double DerivePos_Pt() const { return pos_lv.Pt(); }
    // -- whichever is the (anti)proton
    [[nodiscard]] double DeriveProton_CosThetaStar() const { return proton_cos_theta_star; }

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
    const E2R::Lambda* Source;
    std::size_t Entry{};
};

}  // namespace E2R
