#pragma once

#include <optional>
#include <vector>

#include "Framework.hpp"
#include "POD_Event.hpp"
#include "POD_InjectedSexa.hpp"
#include "POD_McParticle.hpp"
#include "POD_OnTheFlyLambda.hpp"
#include "POD_Track.hpp"

namespace Schema {

struct Events {
    POD::Event Event;
    std::optional<std::vector<POD::InjectedSexa>> InjectedSexa;
    std::optional<std::vector<POD::McParticle>> McParticle;
    std::vector<POD::Track> Track;
    std::vector<POD::OnTheFlyLambda> OnTheFlyLambda;

    Framework::Model CreateModel() {
        Framework::Model model;
        model.RegisterField<POD::Event>(&Event, "Event");
        model.RegisterField<std::optional<std::vector<POD::InjectedSexa>>>(&InjectedSexa, "InjectedSexa");
        model.RegisterField<std::optional<std::vector<POD::McParticle>>>(&McParticle, "McParticle");
        model.RegisterField<std::vector<POD::Track>>(&Track, "Track");
        model.RegisterField<std::vector<POD::OnTheFlyLambda>>(&OnTheFlyLambda, "OnTheFlyLambda");
        return model;
    }

    // Needs to be called at the end of the event loop.
    void Clear() {
        if (InjectedSexa.has_value()) InjectedSexa.value().clear();
        if (McParticle.has_value()) McParticle.value().clear();
        Track.clear();
        OnTheFlyLambda.clear();
    }
};

}  // namespace Schema
