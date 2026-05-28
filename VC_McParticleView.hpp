#pragma once

#include <cstddef>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include <ROOT/RNTupleModel.hxx>

#include "Constants.hpp"
#include "DB_Particles.hpp"
#include "DB_ReactionChannels.hpp"
#include "Math.hpp"
#include "VC_McParticle.hpp"

namespace Vector {

// View of `Vector::McParticle` + cached variables and derived quantities
// (1) cache some ascendants and descendants info;
// (2) cache the classification info of the particle, whether it's true, signal, etc.;
struct McParticleView {

    McParticleView() = delete;
    explicit McParticleView(const Vector::McParticle* source, std::size_t entry = 0) : Source{source}, Entry{entry} {}

    // # Manipulation # //

    [[nodiscard]] std::size_t Size() const { return (*Source->PdgCode).size(); }

    // Fill `mother_{pdg_code/mc_status/is_primary}` and `gm_{mc_entry/pdg_code}`.
    // NOTES: must be executed after updating `Entry`
    void CacheAscendantsInfo() {
        // -- mother
        if (Mother_McEntry() < 0) return;
        Vector::McParticleView mother(Source, static_cast<std::size_t>(Mother_McEntry()));
        mother_pdg_code = mother.PdgCode();
        mother_mc_status = static_cast<int>(mother.Status());
        mother_is_primary = mother.IsLogicalPrimary();
        // -- grandmother
        gm_mc_entry = mother.Mother_McEntry();
        if (gm_mc_entry < 0) return;
        Vector::McParticleView gm(Source, static_cast<std::size_t>(gm_mc_entry));
        gm_pdg_code = gm.PdgCode();
    }

    // Fill `decay_xyz`.
    // NOTES: must be executed after updating `Entry`
    void CacheDescendantsInfo() {
        if (FirstDau_McEntry() < 0) return;
        Vector::McParticleView dau(Source, static_cast<std::size_t>(FirstDau_McEntry()));
        decay_xyz.SetCoordinates(dau.Origin_X(), dau.Origin_Y(), dau.Origin_Z());
    }

    // Fills `is_true`, `is_gen{1/2}_signal`, `reaction_id`.
    // NOTES:
    // - Must be executed after updating `Entry`
    // - Only Gen1 and Gen2 products are considered.
    void Classify_AsInSexaquarkSimulations(const DB::ReactionChannels::Definition& r_channel, int hypothesis_pdg) {
        // reset values
        is_gen1_signal = false;
        is_gen2_signal = false;
        reaction_id = Common::DummyInt;
        // (1) particle should be true
        // NOTE: it's always true when there's no hypothesis (useful when analyzing gen-MC-only)
        int pdg_code = PdgCode();
        is_true = hypothesis_pdg == Common::DummyInt || hypothesis_pdg == pdg_code;
        if (!is_true) return;
        // (2) should come from the anti-sexaquark reaction generator
        if (Generator() != E2R::EGenerator::kInjectedAntiSexaquarkReaction) return;
        auto mc_status = Status();
        // -- in case it could be first-gen product
        //    (a.1) pdg is found in reaction products?
        if (std::ranges::find(r_channel.products_pdg, pdg_code) != r_channel.products_pdg.end()) {
            //    (a.2) has to be marked as primary
            //    (a.3) mc status has to be [600,620[
            is_gen1_signal = IsLogicalPrimary() &&  //
                             E2R::ReactionID_Offset <= mc_status && mc_status < E2R::ReactionID_Offset + E2R::NReactionsPerEvent;
        }
        if (is_gen1_signal) {
            reaction_id = static_cast<int>(mc_status);
            return;
        }
        // -- in case it could be a second-gen product
        //    (b.1) needs to have a mother (implicit)
        //    (b.2) NOTE: don't check on mother's Generator, because it should have the same as daughter's, by construction
        //    (b.3) mother should be marked as primary
        if (!mother_is_primary) return;
        //    (b.4) mother should have a valid reaction id
        if (mother_mc_status < E2R::ReactionID_Offset || mother_mc_status >= E2R::ReactionID_Offset + E2R::NReactionsPerEvent) return;
        //    (b.5) mother should be in first-gen products
        if (std::ranges::find(r_channel.products_pdg, mother_pdg_code) == r_channel.products_pdg.end()) return;
        //    (b.6) mother should be relevant and have detectable-charged decay modes
        auto mother = DB::Particles::FindParticle(mother_pdg_code);
        if (!mother || mother->daughters_pdg.empty()) return;
        //    (b.7) current particle should appear in mother's decay mode
        if (std::ranges::find(mother->daughters_pdg, PdgCode()) == mother->daughters_pdg.end()) return;
        is_gen2_signal = true;
        reaction_id = mother_mc_status;
    }

    // Fills `is_true`, `is_gen{1/2}_signal`, `injection_id`.
    // NOTE: must be executed after updating `Entry` and `CacheAscendantsInfo`
    void Classify_AsInHdibaryonSimulations(bool anti_channel, int hypothesis_pdg) {
        // reset values
        is_gen1_signal = false;
        is_gen2_signal = false;
        injection_id = false;
        // (1) particle should be true
        // NOTE: it's always true when there's no hypothesis (useful when analyzing gen-MC-only)
        int pdg_code = PdgCode();
        is_true = hypothesis_pdg == Common::DummyInt || hypothesis_pdg == pdg_code;
        if (!is_true) return;
        // -- determine h-dibaryon definition
        DB::Particles::Definition pid_hdibaryon = anti_channel ? DB::Particles::Particle("AntiHdibaryon") : DB::Particles::Particle("Hdibaryon");
        // (2) as first-gen signal, it should have h-dibaryon as mother
        is_gen1_signal = Mother_PdgCode() == pid_hdibaryon.pdg_code;
        if (is_gen1_signal) {
            injection_id = static_cast<int>(Status());  // PENDING: i think...
            return;
        }
        DB::Particles::Definition pid_lambda = anti_channel ? DB::Particles::Particle("AntiLambda") : DB::Particles::Particle("Lambda");
        // (3) as second-gen signal,
        //     (3.a) it should have a lambda as a mother;
        if (Mother_PdgCode() != pid_lambda.pdg_code) return;
        //     (3.b) belong to the charged decay;
        if (std::ranges::find(pid_lambda.daughters_pdg, PdgCode()) == pid_lambda.daughters_pdg.end()) return;
        //     (3.c) and grandmother should be h-dibaryon
        is_gen2_signal = GrandMother_PdgCode() == pid_hdibaryon.pdg_code;
        if (is_gen2_signal) injection_id = static_cast<int>(Status());  // PENDING: i think...
    }

    // Fill `lv`, `origin_xyz`, `pca_wrt_pv_xyz`, `cpa_wrt_pv`, `dist_pca2pv`.
    void CacheCalculations(std::size_t mc_entry, const ROOT::Math::XYZPoint& pv, double bz) {
        namespace CMath = Common::Math;
        // load current particle
        Entry = mc_entry;
        // cache info
        lv.SetCoordinates(Px(), Py(), Pz(), Energy());
        origin_xyz.SetCoordinates(Origin_X(), Origin_Y(), Origin_Z());
        ROOT::Math::XYZPoint pca_wrt_pv(0, 0, 0);
        if (Charge() == 0) {
            pca_wrt_pv = CMath::FastPCA_LineVertex(lv.Vect(), origin_xyz, pv);
            cpa_wrt_pv = CMath::CosinePointingAngle(lv.Vect(), origin_xyz, pv);
        } else {
            pca_wrt_pv = CMath::FastPCA_HelixVertex(lv.Vect(), origin_xyz, Charge(), pv, bz);
        }
        dist_pca2pv = pca_wrt_pv - pv;
    }

    // Getters //
    [[nodiscard]] int PdgCode() const { return (*Source->PdgCode)[Entry]; }
    [[nodiscard]] char Charge() const { return (*Source->Charge)[Entry]; }
    [[nodiscard]] int Mother_McEntry() const { return (*Source->Mother_McEntry)[Entry]; }
    [[nodiscard]] unsigned int N_Daughters() const { return (*Source->N_Daughters)[Entry]; }
    [[nodiscard]] int FirstDau_McEntry() const { return (*Source->FirstDau_McEntry)[Entry]; }
    [[nodiscard]] int LastDau_McEntry() const { return (*Source->LastDau_McEntry)[Entry]; }
    [[nodiscard]] float Origin_X() const { return (*Source->Origin_X)[Entry]; }
    [[nodiscard]] float Origin_Y() const { return (*Source->Origin_Y)[Entry]; }
    [[nodiscard]] float Origin_Z() const { return (*Source->Origin_Z)[Entry]; }
    [[nodiscard]] float Px() const { return (*Source->Px)[Entry]; }
    [[nodiscard]] float Py() const { return (*Source->Py)[Entry]; }
    [[nodiscard]] float Pz() const { return (*Source->Pz)[Entry]; }
    [[nodiscard]] float Energy() const { return (*Source->Energy)[Entry]; }
    [[nodiscard]] unsigned int Status() const { return (*Source->Status)[Entry]; }
    [[nodiscard]] int Generator() const { return static_cast<int>((*Source->Generator)[Entry]); }
    [[nodiscard]] bool IsPhysPrimary() const { return static_cast<bool>((*Source->IsPhysPrimary)[Entry]); }
    [[nodiscard]] bool IsSecFromMat() const { return static_cast<bool>((*Source->IsSecFromMat)[Entry]); }
    [[nodiscard]] bool IsSecFromWeak() const { return static_cast<bool>((*Source->IsSecFromWeak)[Entry]); }

    // Derived Quantities //
    // -- labelling
    [[nodiscard]] bool IsTrue() const { return is_true; }
    [[nodiscard]] bool IsSecondary() const { return IsSecFromMat() || IsSecFromWeak() || IsSignal(); }
    [[nodiscard]] bool IsLogicalPrimary() const { return Mother_McEntry() == Common::DummyInt; }
    [[nodiscard]] bool IsFirstGenSignal() const { return is_gen1_signal; }
    [[nodiscard]] bool IsSecondGenSignal() const { return is_gen2_signal; }
    [[nodiscard]] bool IsSignal() const { return IsFirstGenSignal() || IsSecondGenSignal(); }
    // -- kinematics
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    // -- geometry
    [[nodiscard]] double Origin_Radius2D() const { return origin_xyz.Rho(); }
    [[nodiscard]] double Origin_Radius3D() const { return origin_xyz.R(); }
    [[nodiscard]] double DCAxy_wrt_PV() const { return dist_pca2pv.Rho(); }
    [[nodiscard]] double DCAz_wrt_PV() const { return dist_pca2pv.Z(); }
    [[nodiscard]] double DCA_wrt_PV() const { return dist_pca2pv.R(); }
    [[nodiscard]] double CPA_wrt_PV() const { return cpa_wrt_pv; }
    // -- mother-related quantities
    [[nodiscard]] int Mother_PdgCode() const { return mother_pdg_code; }
    [[nodiscard]] int Mother_McStatus() const { return mother_mc_status; }
    [[nodiscard]] bool Mother_IsLogicalPrimary() const { return mother_is_primary; }
    [[nodiscard]] int ReactionID() const { return reaction_id; }
    [[nodiscard]] int InjectionID() const { return injection_id; }
    // -- grandmother-related quantities
    [[nodiscard]] int GrandMother_McEntry() const { return gm_mc_entry; }
    [[nodiscard]] int GrandMother_PdgCode() const { return gm_pdg_code; }
    // -- daughter-related quantities
    [[nodiscard]] double Decay_X() const { return decay_xyz.X(); }
    [[nodiscard]] double Decay_Y() const { return decay_xyz.Y(); }
    [[nodiscard]] double Decay_Z() const { return decay_xyz.Z(); }
    [[nodiscard]] double Decay_Radius2D() const { return N_Daughters() > 0 ? decay_xyz.Rho() : Common::DummyDouble; }
    [[nodiscard]] double Decay_Radius3D() const { return N_Daughters() > 0 ? decay_xyz.R() : Common::DummyDouble; }
    [[nodiscard]] double DecayLength() const { return N_Daughters() > 0 ? (decay_xyz - origin_xyz).R() : Common::DummyDouble; }

    // cached variables //
    ROOT::Math::PxPyPzEVector lv{Common::DummyDouble, Common::DummyDouble, Common::DummyDouble, Common::DummyDouble};
    ROOT::Math::XYZPoint origin_xyz{Common::DummyDouble, Common::DummyDouble, Common::DummyDouble};
    ROOT::Math::XYZPoint decay_xyz{Common::DummyDouble, Common::DummyDouble, Common::DummyDouble};
    ROOT::Math::XYZVector dist_pca2pv{Common::DummyDouble, Common::DummyDouble, Common::DummyDouble};
    double cpa_wrt_pv{Common::DummyDouble};
    int mother_pdg_code{Common::DummyInt};
    int mother_mc_status{Common::DummyInt};
    int gm_mc_entry{Common::DummyInt};
    int gm_pdg_code{Common::DummyInt};
    int reaction_id{Common::DummyInt};   // (anti-sexaquark)
    int injection_id{Common::DummyInt};  // (h-dibaryon)
    bool mother_is_primary{false};
    bool is_true{false};
    bool is_gen1_signal{false};
    bool is_gen2_signal{false};
    // main variables //
    const Vector::McParticle* Source;
    std::size_t Entry{};
};

}  // namespace Vector
