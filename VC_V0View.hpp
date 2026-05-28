#pragma once

#include <cstddef>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Math.hpp"
#include "VC_TrackView.hpp"
#include "VC_V0.hpp"

namespace Vector {

struct V0View {

    V0View() = delete;
    explicit V0View(const Vector::V0* source, std::size_t entry = 0)
        : Source{source},  //
          Neg{&source->Neg},
          Pos{&source->Pos},
          Entry{entry} {}

    // # Manipulation # //

    [[nodiscard]] std::size_t Size() const { return (*Source->Decay_X).size(); }

    void CacheCalculations(size_t entry, const ROOT::Math::XYZPoint& pv) {
        Entry = entry;
        // -- update decay vertex and momentum
        xyz.SetCoordinates(Decay_X(), Decay_Y(), Decay_Z());
        lv.SetCoordinates(Px(), Py(), Pz(), Energy());
        // -- update daughters' momenta @ PCA and PCAs
        neg_pxpypz.SetCoordinates(Neg_PCAwrtV0_Px(), Neg_PCAwrtV0_Py(), Neg_PCAwrtV0_Pz());
        neg_xyz.SetCoordinates(Neg_PCAwrtV0_X(), Neg_PCAwrtV0_Y(), Neg_PCAwrtV0_Z());
        pos_pxpypz.SetCoordinates(Pos_PCAwrtV0_Px(), Pos_PCAwrtV0_Py(), Pos_PCAwrtV0_Pz());
        pos_xyz.SetCoordinates(Pos_PCAwrtV0_X(), Pos_PCAwrtV0_Y(), Pos_PCAwrtV0_Z());
        // -- update pca w.r.t. pv
        pca_wrt_pv = Common::Math::FastPCA_LineVertex(lv.Vect(), xyz, pv);
        // -- update difference between pca and pv
        dist_pca2pv = pca_wrt_pv - pv;
        // -- update CPA wrt pv
        cpa_wrt_pv = Common::Math::CosinePointingAngle(lv.Vect(), xyz, pv);
        // -- update armenteros-podolanski variables
        arm_qt = Common::Math::ArmenterosQt(lv.Vect(), neg_pxpypz);
        arm_alpha = Common::Math::ArmenterosAlpha(lv.Vect(), neg_pxpypz, pos_pxpypz);
    }

    // Getters //
    [[nodiscard]] float Decay_X() const { return (*Source->Decay_X)[Entry]; }
    [[nodiscard]] float Decay_Y() const { return (*Source->Decay_Y)[Entry]; }
    [[nodiscard]] float Decay_Z() const { return (*Source->Decay_Z)[Entry]; }
    [[nodiscard]] float Px() const { return (*Source->Px)[Entry]; }
    [[nodiscard]] float Py() const { return (*Source->Py)[Entry]; }
    [[nodiscard]] float Pz() const { return (*Source->Pz)[Entry]; }
    [[nodiscard]] float Energy() const { return (*Source->Energy)[Entry]; }
    [[nodiscard]] std::array<float, Common::NCovMatrixComponents_State7>& CovMatrix() const { return (*Source->CovMatrix)[Entry]; }
    [[nodiscard]] float Chi2NDF() const { return (*Source->Chi2NDF)[Entry]; }
    // linked mc
    [[nodiscard]] int McEntry() const { return (*Source->McEntry)[Entry]; }
    [[nodiscard]] int PdgCode() const { return (*Source->PdgCode)[Entry]; }
    [[nodiscard]] float MC_Px() const { return (*Source->MC_Px)[Entry]; }
    [[nodiscard]] float MC_Py() const { return (*Source->MC_Py)[Entry]; }
    [[nodiscard]] float MC_Pz() const { return (*Source->MC_Pz)[Entry]; }
    [[nodiscard]] float MC_Energy() const { return (*Source->MC_Energy)[Entry]; }
    [[nodiscard]] float Origin_X() const { return (*Source->Origin_X)[Entry]; }
    [[nodiscard]] float Origin_Y() const { return (*Source->Origin_Y)[Entry]; }
    [[nodiscard]] float Origin_Z() const { return (*Source->Origin_Z)[Entry]; }
    [[nodiscard]] float MC_Decay_X() const { return (*Source->MC_Decay_X)[Entry]; }
    [[nodiscard]] float MC_Decay_Y() const { return (*Source->MC_Decay_Y)[Entry]; }
    [[nodiscard]] float MC_Decay_Z() const { return (*Source->MC_Decay_Z)[Entry]; }
    [[nodiscard]] char IsTrue() const { return (*Source->IsTrue)[Entry]; }
    [[nodiscard]] char IsSignal() const { return (*Source->IsSignal)[Entry]; }
    [[nodiscard]] char IsSecondary() const { return (*Source->IsSecondary)[Entry]; }
    [[nodiscard]] char IsHybrid() const { return (*Source->IsHybrid)[Entry]; }
    [[nodiscard]] int ReactionID() const { return (*Source->ReactionID)[Entry]; }
    // -- mother
    [[nodiscard]] int Mother_McEntry() const { return (*Source->Mother_McEntry)[Entry]; }
    [[nodiscard]] int Mother_PdgCode() const { return (*Source->Mother_PdgCode)[Entry]; }
    // negative daughter
    [[nodiscard]] float Neg_PCAwrtV0_X() const { return (*Source->Neg_PCAwrtV0_X)[Entry]; }
    [[nodiscard]] float Neg_PCAwrtV0_Y() const { return (*Source->Neg_PCAwrtV0_Y)[Entry]; }
    [[nodiscard]] float Neg_PCAwrtV0_Z() const { return (*Source->Neg_PCAwrtV0_Z)[Entry]; }
    [[nodiscard]] float Neg_PCAwrtV0_Px() const { return (*Source->Neg_PCAwrtV0_Px)[Entry]; }
    [[nodiscard]] float Neg_PCAwrtV0_Py() const { return (*Source->Neg_PCAwrtV0_Py)[Entry]; }
    [[nodiscard]] float Neg_PCAwrtV0_Pz() const { return (*Source->Neg_PCAwrtV0_Pz)[Entry]; }
    // positive daughter
    [[nodiscard]] float Pos_PCAwrtV0_X() const { return (*Source->Pos_PCAwrtV0_X)[Entry]; }
    [[nodiscard]] float Pos_PCAwrtV0_Y() const { return (*Source->Pos_PCAwrtV0_Y)[Entry]; }
    [[nodiscard]] float Pos_PCAwrtV0_Z() const { return (*Source->Pos_PCAwrtV0_Z)[Entry]; }
    [[nodiscard]] float Pos_PCAwrtV0_Px() const { return (*Source->Pos_PCAwrtV0_Px)[Entry]; }
    [[nodiscard]] float Pos_PCAwrtV0_Py() const { return (*Source->Pos_PCAwrtV0_Py)[Entry]; }
    [[nodiscard]] float Pos_PCAwrtV0_Pz() const { return (*Source->Pos_PCAwrtV0_Pz)[Entry]; }

    // Derived Info //
    // -- kinematics
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double ArmQt() const { return arm_qt; }
    [[nodiscard]] double ArmAlpha() const { return arm_alpha; }
    // -- geometry
    [[nodiscard]] double Radius2D() const { return xyz.Rho(); }
    [[nodiscard]] double Radius3D() const { return xyz.R(); }
    [[nodiscard]] double CPAwrtPV() const { return cpa_wrt_pv; }
    [[nodiscard]] double DCAwrtPV() const { return dist_pca2pv.R(); }
    [[nodiscard]] double DCAnegV0() const { return (neg_xyz - xyz).R(); }
    [[nodiscard]] double DCAposV0() const { return (pos_xyz - xyz).R(); }
    [[nodiscard]] double DCAbtwDau() const { return (neg_xyz - pos_xyz).R(); }

    // cached variables //
    ROOT::Math::PxPyPzEVector lv;
    ROOT::Math::XYZPoint xyz;
    ROOT::Math::XYZVector neg_pxpypz;
    ROOT::Math::XYZPoint neg_xyz;
    ROOT::Math::XYZVector pos_pxpypz;
    ROOT::Math::XYZPoint pos_xyz;
    ROOT::Math::XYZVector dist_pca2pv;
    ROOT::Math::XYZPoint pca_wrt_pv;
    double cpa_wrt_pv{};
    double arm_qt{};
    double arm_alpha{};
    // main variables //
    const Vector::V0* Source;
    Vector::TrackView Neg;
    Vector::TrackView Pos;
    std::size_t Entry{};
};

}  // namespace Vector
