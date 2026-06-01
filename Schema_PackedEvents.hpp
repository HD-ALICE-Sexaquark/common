#pragma once

#include <optional>
#include <vector>

#include "Framework.hpp"
#include "POD_Event.hpp"
#include "POD_InjectedSexa.hpp"
#include "POD_Track.hpp"
#include "POD_V0.hpp"

namespace Schema {

struct PackedEvents {
    POD::Event Event;
    std::optional<std::vector<POD::InjectedSexa>> InjectedSexa;
    std::vector<POD::Track> NegKaon;
    std::vector<POD::Track> PosKaon;
    std::vector<POD::V0> AntiLambda;
    std::vector<POD::V0> Lambda;
    std::vector<POD::V0> KaonZeroShort;

    Framework::Model CreateModel() {
        Framework::Model model;
        model.RegisterField<POD::Event>(&Event, "Event");
        model.RegisterField<std::optional<std::vector<POD::InjectedSexa>>>(&InjectedSexa, "InjectedSexa");
        model.RegisterField<std::vector<POD::Track>>(&NegKaon, "NegKaon");
        model.RegisterField<std::vector<POD::Track>>(&PosKaon, "PosKaon");
        model.RegisterField<std::vector<POD::V0>>(&AntiLambda, "AntiLambda");
        model.RegisterField<std::vector<POD::V0>>(&Lambda, "Lambda");
        model.RegisterField<std::vector<POD::V0>>(&KaonZeroShort, "KaonZeroShort");
        return model;
    }

    // Needs to be called at the end of the event loop.
    void Clear() {
        if (InjectedSexa.has_value()) InjectedSexa.value().clear();
        NegKaon.clear();
        PosKaon.clear();
        AntiLambda.clear();
        Lambda.clear();
        KaonZeroShort.clear();
    }
};

}  // namespace Schema
