#pragma once

#include <optional>
#include <vector>

#include "Framework.hpp"
#include "POD_Event.hpp"
#include "POD_LambdaPair.hpp"

namespace Schema {

struct FoundHdib {
    POD::Event Event;
    std::optional<std::vector<POD::McParticle>> InjectedHdib;
    std::vector<POD::LambdaPair> LambdaPair;

    Framework::Model CreateModel() {
        Framework::Model model;
        model.RegisterField<POD::Event>(&Event, "Event");
        model.RegisterField<std::optional<std::vector<POD::McParticle>>>(&InjectedHdib, "InjectedHdib");
        model.RegisterField<std::vector<POD::LambdaPair>>(&LambdaPair, "LambdaPair");
        return model;
    }

    // Needs to be called at the end of the event loop.
    void Clear() {
        if (InjectedHdib.has_value()) InjectedHdib.value().clear();
        LambdaPair.clear();
    }
};

}  // namespace Schema
