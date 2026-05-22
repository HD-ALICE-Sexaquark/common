#pragma once

#include <cstddef>

#include <Math/Point3D.h>
#include <Math/Vector4D.h>

#include <ROOT/RNTupleModel.hxx>

#include "E2R_InjectedSexa.hpp"

namespace E2R {

struct InjectedSexaView {

    InjectedSexaView() = delete;
    InjectedSexaView(const E2R::InjectedSexa* source, std::size_t entry) : Source{source}, Entry{entry} {}

    // Manipulation //
    void Update(std::size_t ii, double s_mass, double n_mass, const ROOT::Math::XYZPoint& in_sv, const ROOT::Math::PxPyPzEVector& in_lv_after) {
        Entry = ii;
        // -- update cached variables
        lv.SetCoordinates(GetPx(), GetPy(), GetPz(), s_mass);
        lv_nucleon.SetCoordinates(GetNucleon_Px(), GetNucleon_Py(), GetNucleon_Pz(), n_mass);
        lv_after = in_lv_after;
        lv_mn = lv_after - ROOT::Math::PxPyPzMVector{0., 0., 0., n_mass};
        sv = in_sv;
    }
    [[nodiscard]] std::size_t GetSize() const { return (*Source->ReactionID).size(); }

    // Getters //
    [[nodiscard]] int GetReactionID() const { return (*Source->ReactionID)[Entry]; }
    [[nodiscard]] float GetPx() const { return (*Source->Sexa_Px)[Entry]; }
    [[nodiscard]] float GetPy() const { return (*Source->Sexa_Py)[Entry]; }
    [[nodiscard]] float GetPz() const { return (*Source->Sexa_Pz)[Entry]; }
    [[nodiscard]] float GetNucleon_Px() const { return (*Source->Nucleon_Px)[Entry]; }
    [[nodiscard]] float GetNucleon_Py() const { return (*Source->Nucleon_Py)[Entry]; }
    [[nodiscard]] float GetNucleon_Pz() const { return (*Source->Nucleon_Pz)[Entry]; }

    // Derived Quantities //
    [[nodiscard]] double DeriveEnergy() const { return lv.E(); }
    [[nodiscard]] double DerivePt() const { return lv.Pt(); }
    [[nodiscard]] double DeriveP() const { return lv.P(); }
    [[nodiscard]] double DeriveMass() const { return lv.M(); }
    [[nodiscard]] double DeriveEta() const { return lv.Eta(); }
    [[nodiscard]] double DeriveRapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double DerivePhi() const { return lv.Phi(); }

    [[nodiscard]] double DeriveNucleon_Energy() const { return lv_nucleon.E(); }
    [[nodiscard]] double DeriveNucleon_Pt() const { return lv_nucleon.Pt(); }
    [[nodiscard]] double DeriveNucleon_P() const { return lv_nucleon.P(); }
    [[nodiscard]] double DeriveNucleon_Mass() const { return lv_nucleon.M(); }
    [[nodiscard]] double DeriveNucleon_Eta() const { return lv_nucleon.Eta(); }
    [[nodiscard]] double DeriveNucleon_Rapidity() const { return lv_nucleon.Rapidity(); }
    [[nodiscard]] double DeriveNucleon_Phi() const { return lv_nucleon.Phi(); }

    [[nodiscard]] double DeriveSV_X() const { return sv.X(); }
    [[nodiscard]] double DeriveSV_Y() const { return sv.Y(); }
    [[nodiscard]] double DeriveSV_Z() const { return sv.Z(); }
    [[nodiscard]] double DeriveSV_Radius2D() const { return sv.Rho(); }
    [[nodiscard]] double DeriveSV_Radius3D() const { return sv.R(); }

    [[nodiscard]] double DeriveAfter_Px() const { return lv_after.Px(); }
    [[nodiscard]] double DeriveAfter_Py() const { return lv_after.Py(); }
    [[nodiscard]] double DeriveAfter_Pz() const { return lv_after.Pz(); }
    [[nodiscard]] double DeriveAfter_Energy() const { return lv_after.E(); }
    [[nodiscard]] double DeriveAfter_Pt() const { return lv_after.Pt(); }
    [[nodiscard]] double DeriveAfter_P() const { return lv_after.P(); }
    [[nodiscard]] double DeriveAfter_Mass() const { return lv_after.M(); }
    [[nodiscard]] double DeriveAfter_Eta() const { return lv_after.Eta(); }
    [[nodiscard]] double DeriveAfter_Rapidity() const { return lv_after.Rapidity(); }
    [[nodiscard]] double DeriveAfter_Phi() const { return lv_after.Phi(); }

    [[nodiscard]] double DeriveMinusNucleon_Px() const { return lv_mn.Px(); }
    [[nodiscard]] double DeriveMinusNucleon_Py() const { return lv_mn.Py(); }
    [[nodiscard]] double DeriveMinusNucleon_Pz() const { return lv_mn.Pz(); }
    [[nodiscard]] double DeriveMinusNucleon_Energy() const { return lv_mn.E(); }
    [[nodiscard]] double DeriveMinusNucleon_Pt() const { return lv_mn.Pt(); }
    [[nodiscard]] double DeriveMinusNucleon_P() const { return lv_mn.P(); }
    [[nodiscard]] double DeriveMinusNucleon_Mass() const { return lv_mn.M(); }
    [[nodiscard]] double DeriveMinusNucleon_Eta() const { return lv_mn.Eta(); }
    [[nodiscard]] double DeriveMinusNucleon_Rapidity() const { return lv_mn.Rapidity(); }
    [[nodiscard]] double DeriveMinusNucleon_Phi() const { return lv_mn.Phi(); }

    // cached variables //
    ROOT::Math::PxPyPzMVector lv;
    ROOT::Math::PxPyPzMVector lv_nucleon;
    ROOT::Math::PxPyPzEVector lv_after;
    ROOT::Math::PxPyPzEVector lv_mn;  // = after - nucleon at rest
    ROOT::Math::XYZPoint sv;
    // main variables //
    const E2R::InjectedSexa* Source;
    std::size_t Entry{};
};

}  // namespace E2R
