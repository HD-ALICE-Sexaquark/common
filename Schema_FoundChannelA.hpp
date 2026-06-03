#pragma once

#include <vector>

#include "Framework.hpp"
#include "POD_McParticle.hpp"
#include "POD_Track.hpp"
#include "POD_V0.hpp"
#include "Schema_FoundSexaquark.hpp"

namespace Schema {

struct FoundChannelA : Schema::FoundSexaquark {
    std::vector<POD::V0> V0A;
    std::vector<POD::Track> V0A_Neg;
    std::vector<POD::Track> V0A_Pos;
    std::vector<POD::V0> V0B;
    std::vector<POD::Track> V0B_Neg;
    std::vector<POD::Track> V0B_Pos;
    // mc only
    std::vector<POD::Extended::McParticle> MC_V0A;
    std::vector<POD::Extended::McParticle> MC_V0A_Neg;
    std::vector<POD::Extended::McParticle> MC_V0A_Pos;
    std::vector<POD::Extended::McParticle> MC_V0B;
    std::vector<POD::Extended::McParticle> MC_V0B_Neg;
    std::vector<POD::Extended::McParticle> MC_V0B_Pos;

    Framework::Model CreateModel(bool is_mc) {
        Framework::Model model(CreateModel_Base(is_mc));
        model.RegisterField<std::vector<POD::V0>>(&V0A, "V0A");
        model.RegisterField<std::vector<POD::Track>>(&V0A_Neg, "V0A_Neg");
        model.RegisterField<std::vector<POD::Track>>(&V0A_Pos, "V0A_Pos");
        model.RegisterField<std::vector<POD::V0>>(&V0B, "V0B");
        model.RegisterField<std::vector<POD::Track>>(&V0B_Neg, "V0B_Neg");
        model.RegisterField<std::vector<POD::Track>>(&V0B_Pos, "V0B_Pos");
        if (is_mc) {
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_V0A, "MC_V0A");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_V0A_Neg, "MC_V0A_Neg");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_V0A_Pos, "MC_V0A_Pos");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_V0B, "MC_V0B");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_V0B_Neg, "MC_V0B_Neg");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_V0B_Pos, "MC_V0B_Pos");
        }
        return model;
    }

    void Clear(bool is_mc) {
        Clear_Base(is_mc);
        V0A.clear();
        V0A_Neg.clear();
        V0A_Pos.clear();
        V0B.clear();
        V0B_Neg.clear();
        V0B_Pos.clear();
        if (is_mc) {
            MC_V0A.clear();
            MC_V0A_Neg.clear();
            MC_V0A_Pos.clear();
            MC_V0B.clear();
            MC_V0B_Neg.clear();
            MC_V0B_Pos.clear();
        }
    }
};

}  // namespace Schema
