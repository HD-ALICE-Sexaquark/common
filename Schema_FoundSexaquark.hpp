#pragma once

#include <vector>

#include "Framework.hpp"
#include "POD_Event.hpp"
#include "POD_InjectedSexa.hpp"
#include "POD_Sexaquark.hpp"

namespace Schema {

struct FoundSexaquark {
    POD::Event Event;
    std::vector<POD::Sexaquark> Sexaquark;
    // mc only
    POD::MC::Event MC_Event;
    std::vector<POD::Extended::InjectedSexa> Injected;    // total injected signal, useful for efficiency studies
    std::vector<POD::Linked::InjectedSexa> MC_Sexaquark;  // linked mc

    Framework::Model CreateModel_Base(bool is_mc) {
        Framework::Model model;
        model.RegisterField<POD::Event>(&Event, "Event");
        model.RegisterField<std::vector<POD::Sexaquark>>(&Sexaquark, "Sexaquark");
        if (is_mc) {
            model.RegisterField<POD::MC::Event>(&MC_Event, "MC_Event");
            model.RegisterField<std::vector<POD::Extended::InjectedSexa>>(&Injected, "Injected");
            model.RegisterField<std::vector<POD::Linked::InjectedSexa>>(&MC_Sexaquark, "MC_Sexaquark");
        }
        return model;
    }

    void Clear_Base(bool is_mc) {
        Sexaquark.clear();
        if (is_mc) {
            Injected.clear();
            MC_Sexaquark.clear();
        }
    }
};

}  // namespace Schema
