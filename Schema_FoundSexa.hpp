#pragma once

#include <optional>
#include <vector>

#include "Framework.hpp"
#include "POD_ChannelA.hpp"
#include "POD_ChannelD.hpp"
#include "POD_ChannelH.hpp"
#include "POD_Event.hpp"
#include "POD_InjectedSexa.hpp"

namespace Schema {

struct FoundSexa {
    POD::Event Event;
    std::optional<std::vector<POD::InjectedSexa>> InjectedSexa;
    std::vector<POD::ChannelA> ChannelA;
    std::vector<POD::ChannelD> ChannelD;
    std::vector<POD::ChannelH> ChannelH;

    Framework::Model CreateModel() {
        Framework::Model model;
        model.RegisterField<POD::Event>(&Event, "Event");
        model.RegisterField<std::optional<std::vector<POD::InjectedSexa>>>(&InjectedSexa, "InjectedSexa");
        model.RegisterField<std::vector<POD::ChannelA>>(&ChannelA, "ChannelA");
        model.RegisterField<std::vector<POD::ChannelD>>(&ChannelD, "ChannelD");
        model.RegisterField<std::vector<POD::ChannelH>>(&ChannelH, "ChannelH");
        return model;
    }

    // Needs to be called at the end of the event loop.
    void Clear() {
        if (InjectedSexa.has_value()) InjectedSexa.value().clear();
        ChannelA.clear();
        ChannelD.clear();
        ChannelH.clear();
    }
};

}  // namespace Schema
