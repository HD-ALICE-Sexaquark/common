#pragma once

#include <vector>

#include "Framework.hpp"
#include "POD_McParticle.hpp"
#include "POD_Track.hpp"
#include "Schema_FoundSexaquark.hpp"

namespace Schema {

struct FoundChannelH : Schema::FoundSexaquark {
    std::vector<POD::Track> Kaon1;
    std::vector<POD::Track> Kaon2;
    // mc only
    std::vector<POD::Extended::McParticle> MC_Kaon1;
    std::vector<POD::Extended::McParticle> MC_Kaon2;

    Framework::Model CreateModel(bool is_mc) {
        Framework::Model model(CreateModel_Base(is_mc));
        model.RegisterField<std::vector<POD::Track>>(&Kaon1, "Kaon1");
        model.RegisterField<std::vector<POD::Track>>(&Kaon2, "Kaon2");
        if (is_mc) {
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Kaon1, "MC_Kaon1");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Kaon2, "MC_Kaon2");
        }
        return model;
    }

    void Clear(bool is_mc) {
        Clear_Base(is_mc);
        Kaon1.clear();
        Kaon2.clear();
        if (is_mc) {
            MC_Kaon1.clear();
            MC_Kaon2.clear();
        }
    }
};

}  // namespace Schema
