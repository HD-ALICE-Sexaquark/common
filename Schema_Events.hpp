#pragma once

#include <vector>

#include <ROOT/RVersion.hxx>

#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 40, 0)
#include "Framework.hpp"
#else
#include "Framework_TeeTree.hpp"
#endif
#include "POD_Event.hpp"
#include "POD_InjectedSexa.hpp"
#include "POD_McParticle.hpp"
#include "POD_PreFoundLambda.hpp"
#include "POD_Track.hpp"

namespace Schema {

struct Events {
    POD::Event Event;
    std::vector<POD::Track> Track;
    std::vector<POD::PreFoundLambda> PreFoundLambda;
    // mc only //
    POD::MC::Event MC_Event;
    std::vector<POD::InjectedSexa> InjectedSexa;
    std::vector<POD::McParticle> McParticle;
    std::vector<unsigned int> Track_McEntry;
    std::vector<unsigned int> PreFoundLambda_Neg_McEntry;
    std::vector<unsigned int> PreFoundLambda_Pos_McEntry;

#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 40, 0)
    Framework::Model CreateModel(bool is_mc, bool sexa_mc) {
        Framework::Model model;
        model.RegisterField<POD::Event>(&Event, "Event");
        model.RegisterField<std::vector<POD::Track>>(&Track, "Track");
        model.RegisterField<std::vector<POD::PreFoundLambda>>(&PreFoundLambda, "PreFoundLambda");
        if (is_mc) {
            model.RegisterField<POD::MC::Event>(&MC_Event, "MC_Event");
            if (sexa_mc) model.RegisterField<std::vector<POD::InjectedSexa>>(&InjectedSexa, "InjectedSexa");
            model.RegisterField<std::vector<POD::McParticle>>(&McParticle, "McParticle");
            model.RegisterField<std::vector<unsigned int>>(&Track_McEntry, "Track_McEntry");
            model.RegisterField<std::vector<unsigned int>>(&PreFoundLambda_Neg_McEntry, "PreFoundLambda_Neg_McEntry");
            model.RegisterField<std::vector<unsigned int>>(&PreFoundLambda_Pos_McEntry, "PreFoundLambda_Pos_McEntry");
        }
        return model;
    }
#endif

    Framework::TeeTree::Model CreateModel_TeeTree(bool is_mc, bool sexa_mc) {
        Framework::TeeTree::Model model;
        model.RegisterBranch<POD::Event>(&Event, "Event");
        model.RegisterBranch<std::vector<POD::Track>>(&Track, "Track");
        model.RegisterBranch<std::vector<POD::PreFoundLambda>>(&PreFoundLambda, "PreFoundLambda");
        if (is_mc) {
            model.RegisterBranch<POD::MC::Event>(&MC_Event, "MC_Event");
            if (sexa_mc) model.RegisterBranch<std::vector<POD::InjectedSexa>>(&InjectedSexa, "InjectedSexa");
            model.RegisterBranch<std::vector<POD::McParticle>>(&McParticle, "McParticle");
            model.RegisterBranch<std::vector<unsigned int>>(&Track_McEntry, "Track_McEntry");
            model.RegisterBranch<std::vector<unsigned int>>(&PreFoundLambda_Neg_McEntry, "PreFoundLambda_Neg_McEntry");
            model.RegisterBranch<std::vector<unsigned int>>(&PreFoundLambda_Pos_McEntry, "PreFoundLambda_Pos_McEntry");
        }
        return model;
    }

    void Clear(bool is_mc, bool sexa_mc) {
        Track.clear();
        PreFoundLambda.clear();
        if (is_mc) {
            if (sexa_mc) InjectedSexa.clear();
            McParticle.clear();
            Track_McEntry.clear();
            PreFoundLambda_Neg_McEntry.clear();
            PreFoundLambda_Pos_McEntry.clear();
        }
    }
};

}  // namespace Schema
