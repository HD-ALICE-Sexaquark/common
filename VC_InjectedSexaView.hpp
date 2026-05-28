#pragma once

#include <cstddef>

#include <Math/Point3D.h>
#include <Math/Vector4D.h>

#include <ROOT/RNTupleModel.hxx>

#include "VC_InjectedSexa.hpp"

namespace Vector {

struct InjectedSexaView {

    InjectedSexaView() = delete;
    explicit InjectedSexaView(const Vector::InjectedSexa* source, std::size_t entry = 0) : Source{source}, Entry{entry} {}

    // # Manipulation # //

    [[nodiscard]] std::size_t Size() const { return (*Source->ReactionID).size(); }

    void CacheCalculations(std::size_t ii, double s_mass, double n_mass) {
        Entry = ii;
        // -- update cached variables
        lv.SetCoordinates(Px(), Py(), Pz(), s_mass);
        lv_nucleon.SetCoordinates(Nucleon_Px(), Nucleon_Py(), Nucleon_Pz(), n_mass);
        sv.SetCoordinates(SV_X(), SV_Y(), SV_Z());
    }

    // # Getters # //

    [[nodiscard]] unsigned int ReactionID() const { return (*Source->ReactionID)[Entry]; }
    [[nodiscard]] float SV_X() const { return (*Source->SV_X)[Entry]; }
    [[nodiscard]] float SV_Y() const { return (*Source->SV_Y)[Entry]; }
    [[nodiscard]] float SV_Z() const { return (*Source->SV_Z)[Entry]; }
    [[nodiscard]] float Px() const { return (*Source->Sexa_Px)[Entry]; }
    [[nodiscard]] float Py() const { return (*Source->Sexa_Py)[Entry]; }
    [[nodiscard]] float Pz() const { return (*Source->Sexa_Pz)[Entry]; }
    [[nodiscard]] float Nucleon_Px() const { return (*Source->Nucleon_Px)[Entry]; }
    [[nodiscard]] float Nucleon_Py() const { return (*Source->Nucleon_Py)[Entry]; }
    [[nodiscard]] float Nucleon_Pz() const { return (*Source->Nucleon_Pz)[Entry]; }

    // # Derived Quantities # //

    [[nodiscard]] double Energy() const { return lv.E(); }
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }

    [[nodiscard]] double Nucleon_Energy() const { return lv_nucleon.E(); }
    [[nodiscard]] double Nucleon_Pt() const { return lv_nucleon.Pt(); }
    [[nodiscard]] double Nucleon_P() const { return lv_nucleon.P(); }
    [[nodiscard]] double Nucleon_Mass() const { return lv_nucleon.M(); }
    [[nodiscard]] double Nucleon_Eta() const { return lv_nucleon.Eta(); }
    [[nodiscard]] double Nucleon_Rapidity() const { return lv_nucleon.Rapidity(); }
    [[nodiscard]] double Nucleon_Phi() const { return lv_nucleon.Phi(); }

    [[nodiscard]] double SV_Radius2D() const { return sv.Rho(); }
    [[nodiscard]] double SV_Radius3D() const { return sv.R(); }

    // cached variables //
    ROOT::Math::PxPyPzMVector lv;
    ROOT::Math::PxPyPzMVector lv_nucleon;
    ROOT::Math::XYZPoint sv;
    // main variables //
    const Vector::InjectedSexa* Source;
    std::size_t Entry{};
};

}  // namespace Vector
