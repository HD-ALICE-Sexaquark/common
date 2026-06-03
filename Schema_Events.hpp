#pragma once

#include <vector>

#include "Framework.hpp"
#include "POD_Event.hpp"
#include "POD_InjectedSexa.hpp"
#include "POD_McParticle.hpp"
#include "POD_OnTheFlyLambda.hpp"
#include "POD_Track.hpp"

namespace Schema {

struct Events {
    POD::Extended::Event Event;
    std::vector<POD::Track> Track;
    std::vector<POD::OnTheFlyLambda> OnTheFlyLambda;
    // mc only //
    POD::MC::Event MC_Event;
    std::vector<POD::InjectedSexa> InjectedSexa;
    std::vector<POD::McParticle> McParticle;
    std::vector<unsigned int> Track_McEntry;
    std::vector<unsigned int> OnTheFlyLambda_Neg_McEntry;
    std::vector<unsigned int> OnTheFlyLambda_Pos_McEntry;

    Framework::Model CreateModel(bool is_mc, bool sexa_mc) {
        Framework::Model model;
        model.RegisterField<POD::Extended::Event>(&Event, "Event");
        model.RegisterField<std::vector<POD::Track>>(&Track, "Track");
        model.RegisterField<std::vector<POD::OnTheFlyLambda>>(&OnTheFlyLambda, "OnTheFlyLambda");
        if (is_mc) {
            model.RegisterField<POD::MC::Event>(&MC_Event, "MC_Event");
            if (sexa_mc) model.RegisterField<std::vector<POD::InjectedSexa>>(&InjectedSexa, "InjectedSexa");
            model.RegisterField<std::vector<POD::McParticle>>(&McParticle, "McParticle");
            model.RegisterField<std::vector<unsigned int>>(&Track_McEntry, "Track_McEntry");
            model.RegisterField<std::vector<unsigned int>>(&OnTheFlyLambda_Neg_McEntry, "OnTheFlyLambda_Neg_McEntry");
            model.RegisterField<std::vector<unsigned int>>(&OnTheFlyLambda_Pos_McEntry, "OnTheFlyLambda_Pos_McEntry");
        }
        return model;
    }

    void Clear(bool is_mc, bool sexa_mc) {
        Track.clear();
        OnTheFlyLambda.clear();
        if (is_mc) {
            if (sexa_mc) InjectedSexa.clear();
            McParticle.clear();
            Track_McEntry.clear();
            OnTheFlyLambda_Neg_McEntry.clear();
            OnTheFlyLambda_Pos_McEntry.clear();
        }
    }
};

}  // namespace Schema
