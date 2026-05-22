#pragma once

#include <cstddef>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include <ROOT/RNTupleModel.hxx>

#include "Constants.hpp"
#include "DB_Particles.hpp"
#include "DB_ReactionChannels.hpp"
#include "E2R_McParticle.hpp"
#include "Math.hpp"

namespace E2R {

struct McParticleView {

    McParticleView() = delete;
    McParticleView(const E2R::McParticle* source, std::size_t entry) : Source{source}, Entry{entry} {}

    // # Manipulation # //
    [[nodiscard]] std::size_t GetSize() const { return (*Source->PdgCode).size(); }

    // Fills `is_true`, `is_signal`, `reaction_id`.
    // NOTES:
    // - Must be executed after `MCParticle::Update()`.
    // - Only Gen1 and Gen2 products are considered.
    void Classify(std::size_t mc_entry, const DB::ReactionChannels::Definition& r_channel, int hypothesis_pdg = Common::DummyInt) {

        // reset values
        is_true = false;
        is_signal = false;
        reaction_id = Common::DummyInt;
        // load current particle
        Entry = mc_entry;
        // (1) particle should be true
        // NOTE: it's always true when there's no hypothesis (useful when analyzing gen-MC-only)
        int pdg_code = GetPdgCode();
        is_true = hypothesis_pdg == Common::DummyInt || hypothesis_pdg == pdg_code;
        if (!is_true) return;
        // (2) should come from the anti-sexaquark reaction generator
        if (GetGenerator() != E2R::EGenerator::kInjectedAntiSexaquarkReaction) return;
        unsigned int status = GetStatus();
        // -- in case it could be first-gen product
        //    (a.1) pdg is found in reaction products?
        if (std::ranges::find(r_channel.products_pdg, pdg_code) != r_channel.products_pdg.end()) {
            //    (a.2) has to be marked as primary
            is_signal = DeriveIsLogicalPrimary();
            //    (a.3) mc status has to be [600,620[
            is_signal = is_signal && Common::ReactionID_Offset <= status && status < Common::ReactionID_Offset + E2R::NReactionsPerEvent;
        }
        if (is_signal) {
            reaction_id = static_cast<int>(status);
            return;
        }
        // -- in case it could be a second-gen product
        //    (b.1) needs to have a mother (implicit)
        //    (b.2) NOTE: don't check on mother's Generator, because it should have the same as daughter's, by construction
        //    (b.3) mother should be marked as primary
        if (!mother_is_primary) return;
        //    (b.4) mother should have a valid reaction id
        if (mother_status < Common::ReactionID_Offset || mother_status >= Common::ReactionID_Offset + E2R::NReactionsPerEvent) return;
        //    (b.5) mother should be in first-gen products
        if (std::ranges::find(r_channel.products_pdg, mother_pdg_code) == r_channel.products_pdg.end()) return;
        //    (b.6) mother should be relevant and have detectable-charged decay modes
        auto mother = DB::Particles::FindParticle(mother_pdg_code);
        if (!mother || mother->daughters_pdg.empty()) return;
        //    (b.7) current particle should appear in mother's decay mode
        if (std::ranges::find(mother->daughters_pdg, GetPdgCode()) == mother->daughters_pdg.end()) return;
        is_signal = true;
        reaction_id = mother_status;
    }

    // Fills `mother_pdg_code`, `mother_status`, `mother_is_primary`.
    void CacheInfo_MotherOf(std::size_t mc_entry) {
        // reset values
        mother_pdg_code = Common::DummyInt;
        mother_status = Common::DummyInt;
        mother_is_primary = false;
        // load current particle
        std::size_t current_entry = mc_entry;
        Entry = current_entry;
        // check for mother
        if (GetMother_McEntry() == Common::DummyInt) return;
        // load mother particle
        Entry = GetMother_McEntry();
        mother_pdg_code = GetPdgCode();
        mother_status = static_cast<int>(GetStatus());
        mother_is_primary = DeriveIsLogicalPrimary();
        // load back current particle
        Entry = current_entry;
    }

    // Fills `decay_xyz`.
    void CacheInfo_DaughtersOf(std::size_t mc_entry) {
        // reset values
        decay_xyz.SetCoordinates(Common::DummyDouble, Common::DummyDouble, Common::DummyDouble);
        // load current particle
        std::size_t current_entry = mc_entry;
        Entry = current_entry;
        // check for daughters
        if (GetN_Daughters() <= 0) return;
        Entry = GetFirstDau_McEntry();
        decay_xyz.SetCoordinates(GetOrigin_X(), GetOrigin_Y(), GetOrigin_Z());
        // load back current particle
        Entry = current_entry;
    }

    void Update(std::size_t mc_entry, const ROOT::Math::XYZPoint& pv, double bz) {
        namespace CMath = Common::Math;
        // load current particle
        Entry = mc_entry;
        // cache info
        lv.SetCoordinates(GetPx(), GetPy(), GetPz(), GetE());
        origin_xyz.SetCoordinates(GetOrigin_X(), GetOrigin_Y(), GetOrigin_Z());
        ROOT::Math::XYZPoint pca_wrt_pv(0, 0, 0);
        if (GetCharge() == 0) {
            pca_wrt_pv = CMath::FastPCA_LineVertex(lv.Vect(), origin_xyz, pv);
            cpa_wrt_pv = CMath::CosinePointingAngle(lv.Vect(), origin_xyz, pv);
        } else {
            pca_wrt_pv = CMath::FastPCA_HelixVertex(lv.Vect(), origin_xyz, GetCharge(), pv, bz);
        }
        dist_pca2pv = pca_wrt_pv - pv;
        // cache info from related particles
        CacheInfo_MotherOf(mc_entry);
        CacheInfo_DaughtersOf(mc_entry);
    }

    // Getters //
    [[nodiscard]] int GetPdgCode() const { return (*Source->PdgCode)[Entry]; }
    [[nodiscard]] char GetCharge() const { return (*Source->Charge)[Entry]; }
    [[nodiscard]] int GetMother_McEntry() const { return (*Source->Mother_McEntry)[Entry]; }
    [[nodiscard]] unsigned int GetN_Daughters() const { return (*Source->N_Daughters)[Entry]; }
    [[nodiscard]] int GetFirstDau_McEntry() const { return (*Source->FirstDau_McEntry)[Entry]; }
    [[nodiscard]] int GetLastDau_McEntry() const { return (*Source->LastDau_McEntry)[Entry]; }
    [[nodiscard]] float GetOrigin_X() const { return (*Source->Origin_X)[Entry]; }
    [[nodiscard]] float GetOrigin_Y() const { return (*Source->Origin_Y)[Entry]; }
    [[nodiscard]] float GetOrigin_Z() const { return (*Source->Origin_Z)[Entry]; }
    [[nodiscard]] float GetPx() const { return (*Source->Px)[Entry]; }
    [[nodiscard]] float GetPy() const { return (*Source->Py)[Entry]; }
    [[nodiscard]] float GetPz() const { return (*Source->Pz)[Entry]; }
    [[nodiscard]] float GetE() const { return (*Source->E)[Entry]; }
    [[nodiscard]] unsigned int GetStatus() const { return (*Source->Status)[Entry]; }
    [[nodiscard]] int GetGenerator() const { return static_cast<int>((*Source->Generator)[Entry]); }
    [[nodiscard]] bool GetIsPhysPrimary() const { return static_cast<bool>((*Source->IsPhysPrimary)[Entry]); }
    [[nodiscard]] bool GetIsSecFromMat() const { return static_cast<bool>((*Source->IsSecFromMat)[Entry]); }
    [[nodiscard]] bool GetIsSecFromWeak() const { return static_cast<bool>((*Source->IsSecFromWeak)[Entry]); }

    // Derived Quantities //
    [[nodiscard]] bool DeriveIsTrue() const { return is_true; }
    [[nodiscard]] bool DeriveIsSignal() const { return is_signal; }
    [[nodiscard]] bool DeriveIsSecondary() const { return GetIsSecFromMat() || GetIsSecFromWeak() || is_signal; }
    [[nodiscard]] bool DeriveIsLogicalPrimary() const { return GetMother_McEntry() == Common::DummyInt; }

    [[nodiscard]] double DerivePt() const { return lv.Pt(); }
    [[nodiscard]] double DeriveP() const { return lv.P(); }
    [[nodiscard]] double DeriveMass() const { return lv.M(); }
    [[nodiscard]] double DeriveEta() const { return lv.Eta(); }
    [[nodiscard]] double DeriveRapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double DerivePhi() const { return lv.Phi(); }

    [[nodiscard]] double DeriveOrigin_Radius2D() const { return origin_xyz.Rho(); }
    [[nodiscard]] double DeriveOrigin_Radius3D() const { return origin_xyz.R(); }

    [[nodiscard]] double DeriveDCAxy_wrt_PV() const { return dist_pca2pv.Rho(); }
    [[nodiscard]] double DeriveDCAz_wrt_PV() const { return dist_pca2pv.Z(); }
    [[nodiscard]] double DeriveDCA_wrt_PV() const { return dist_pca2pv.R(); }
    [[nodiscard]] double DeriveCPA_wrt_PV() const { return cpa_wrt_pv; }
    // -- mother-related quantities
    [[nodiscard]] int DeriveMother_PdgCode() const { return mother_pdg_code; }
    [[nodiscard]] int DeriveMother_Status() const { return mother_status; }
    [[nodiscard]] bool DeriveMother_IsLogicalPrimary() const { return mother_is_primary; }
    [[nodiscard]] int DeriveReactionID() const { return reaction_id; }
    // -- daughter-related quantities
    [[nodiscard]] double DeriveDecay_X() const { return decay_xyz.X(); }
    [[nodiscard]] double DeriveDecay_Y() const { return decay_xyz.Y(); }
    [[nodiscard]] double DeriveDecay_Z() const { return decay_xyz.Z(); }
    [[nodiscard]] double DeriveDecay_Radius2D() const { return GetN_Daughters() > 0 ? decay_xyz.Rho() : Common::DummyDouble; }
    [[nodiscard]] double DeriveDecay_Radius3D() const { return GetN_Daughters() > 0 ? decay_xyz.R() : Common::DummyDouble; }
    [[nodiscard]] double DeriveDecayLength() const { return GetN_Daughters() > 0 ? (decay_xyz - origin_xyz).R() : Common::DummyDouble; }

    // cached variables //
    ROOT::Math::PxPyPzEVector lv;
    ROOT::Math::XYZPoint origin_xyz;
    ROOT::Math::XYZPoint decay_xyz;
    ROOT::Math::XYZVector dist_pca2pv;
    double cpa_wrt_pv{};
    int mother_pdg_code{};
    int mother_status{};
    int reaction_id{};
    bool mother_is_primary{};
    bool is_true{};
    bool is_signal{};
    // main variables //
    const E2R::McParticle* Source;
    std::size_t Entry{};
};

}  // namespace E2R
