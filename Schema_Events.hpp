#pragma once

#include <cstdint>
#include <vector>

#include <ROOT/RVersion.hxx>

#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 40, 0)
#include "Framework.hpp"
#endif
#include "Framework_TeeTree.hpp"
#include "POD_Event.hpp"
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
    std::vector<POD::McParticle> McParticle;
    // NOTE: frozen as in E2T `f1ba831`, whose antisexaquark MC trees stand in for general-purpose MC: `|AliESDtrack::GetLabel()|`,
    //       so a fake track isn't flagged
    std::vector<unsigned int> Track_McEntry;
    std::vector<unsigned int> PreFoundLambda_Neg_McEntry;
    std::vector<unsigned int> PreFoundLambda_Pos_McEntry;
    // NOTE: parallel to `McParticle`, the furthest bookkeeping bin + 1 that any of its ESD tracks (|label|) or pre-found V0s
    //       (made of two of its daughters) reached, 0 for none; bit 7 set if only fake ones (label < 0) reached it.
    //       `McParticle_TrackStage` filled with the tracks, `McParticle_PreFoundLambdaStage` for the h-dibaryon MC only;
    //       otherwise written empty
    std::vector<std::uint8_t> McParticle_TrackStage;
    std::vector<std::uint8_t> McParticle_PreFoundLambdaStage;

#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 40, 0)
    Framework::Model CreateModel(bool is_mc) {
        Framework::Model model;
        model.RegisterField<POD::Event>(&Event, "Event");
        model.RegisterField<std::vector<POD::Track>>(&Track, "Track");
        model.RegisterField<std::vector<POD::PreFoundLambda>>(&PreFoundLambda, "PreFoundLambda");
        if (is_mc) {
            model.RegisterField<POD::MC::Event>(&MC_Event, "MC_Event");
            model.RegisterField<std::vector<POD::McParticle>>(&McParticle, "McParticle");
            model.RegisterField<std::vector<unsigned int>>(&Track_McEntry, "Track_McEntry");
            model.RegisterField<std::vector<unsigned int>>(&PreFoundLambda_Neg_McEntry, "PreFoundLambda_Neg_McEntry");
            model.RegisterField<std::vector<unsigned int>>(&PreFoundLambda_Pos_McEntry, "PreFoundLambda_Pos_McEntry");
            model.RegisterField<std::vector<std::uint8_t>>(&McParticle_TrackStage, "McParticle_TrackStage");
            model.RegisterField<std::vector<std::uint8_t>>(&McParticle_PreFoundLambdaStage, "McParticle_PreFoundLambdaStage");
        }
        return model;
    }
#endif

    Framework::TeeTree::Model CreateModel_TeeTree(bool is_mc) {
        Framework::TeeTree::Model model;
        model.RegisterBranch<POD::Event>(&Event, "Event");
        model.RegisterBranch<std::vector<POD::Track>>(&Track, "Track");
        model.RegisterBranch<std::vector<POD::PreFoundLambda>>(&PreFoundLambda, "PreFoundLambda");
        if (is_mc) {
            model.RegisterBranch<POD::MC::Event>(&MC_Event, "MC_Event");
            model.RegisterBranch<std::vector<POD::McParticle>>(&McParticle, "McParticle");
            model.RegisterBranch<std::vector<unsigned int>>(&Track_McEntry, "Track_McEntry");
            model.RegisterBranch<std::vector<unsigned int>>(&PreFoundLambda_Neg_McEntry, "PreFoundLambda_Neg_McEntry");
            model.RegisterBranch<std::vector<unsigned int>>(&PreFoundLambda_Pos_McEntry, "PreFoundLambda_Pos_McEntry");
            model.RegisterBranch<std::vector<std::uint8_t>>(&McParticle_TrackStage, "McParticle_TrackStage");
            model.RegisterBranch<std::vector<std::uint8_t>>(&McParticle_PreFoundLambdaStage, "McParticle_PreFoundLambdaStage");
        }
        return model;
    }

    void Clear(bool is_mc) {
        Track.clear();
        PreFoundLambda.clear();
        if (is_mc) {
            McParticle.clear();
            Track_McEntry.clear();
            PreFoundLambda_Neg_McEntry.clear();
            PreFoundLambda_Pos_McEntry.clear();
            McParticle_TrackStage.clear();
            McParticle_PreFoundLambdaStage.clear();
        }
    }
};

}  // namespace Schema
