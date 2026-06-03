#pragma once

#include <vector>

#include "Framework.hpp"
#include "POD_McParticle.hpp"
#include "POD_Track.hpp"
#include "POD_V0.hpp"
#include "Schema_FoundSexaquark.hpp"

namespace Schema {

struct FoundChannelD : Schema::FoundSexaquark {
    std::vector<POD::V0> V0;
    std::vector<POD::Track> V0_Neg;
    std::vector<POD::Track> V0_Pos;
    std::vector<POD::Track> Kaon;
    // mc only
    std::vector<POD::Extended::McParticle> MC_V0;
    std::vector<POD::Extended::McParticle> MC_V0_Neg;
    std::vector<POD::Extended::McParticle> MC_V0_Pos;
    std::vector<POD::Extended::McParticle> MC_Kaon;

    Framework::Model CreateModel(bool is_mc) {
        Framework::Model model(CreateModel_Base(is_mc));
        model.RegisterField<std::vector<POD::V0>>(&V0, "V0");
        model.RegisterField<std::vector<POD::Track>>(&V0_Neg, "V0_Neg");
        model.RegisterField<std::vector<POD::Track>>(&V0_Pos, "V0_Pos");
        model.RegisterField<std::vector<POD::Track>>(&Kaon, "Kaon");
        if (is_mc) {
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_V0, "MC_V0");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_V0_Neg, "MC_V0_Neg");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_V0_Pos, "MC_V0_Pos");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Kaon, "MC_Kaon");
        }
        return model;
    }

    void Clear(bool is_mc) {
        Clear_Base(is_mc);
        V0.clear();
        V0_Neg.clear();
        V0_Pos.clear();
        Kaon.clear();
        if (is_mc) {
            MC_V0.clear();
            MC_V0_Neg.clear();
            MC_V0_Pos.clear();
            MC_Kaon.clear();
        }
    }
};

}  // namespace Schema
