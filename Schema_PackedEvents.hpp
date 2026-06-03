#pragma once

#include <vector>

#include "Framework.hpp"
#include "POD_Event.hpp"
#include "POD_InjectedSexa.hpp"
#include "POD_McParticle.hpp"
#include "POD_Track.hpp"
#include "POD_V0.hpp"

namespace Schema {

struct PackedEvents {
    // -- event
    POD::Event Event;
    // -- negative kaon
    std::vector<POD::Track> NegKaon;
    // -- positive kaon
    std::vector<POD::Track> PosKaon;
    // -- anti-lambda
    std::vector<POD::V0> AntiLambda;
    std::vector<POD::Track> AntiLambda_Neg;
    std::vector<POD::Track> AntiLambda_Pos;
    // -- lambda
    std::vector<POD::V0> Lambda;
    std::vector<POD::Track> Lambda_Neg;
    std::vector<POD::Track> Lambda_Pos;
    // -- kaon-zero-short
    std::vector<POD::V0> KaonZeroShort;
    std::vector<POD::Track> KaonZeroShort_Neg;
    std::vector<POD::Track> KaonZeroShort_Pos;
    // mc only
    // -- event
    POD::MC::Event MC_Event;
    // -- injected
    std::vector<POD::Extended::InjectedSexa> InjectedSexa;
    // -- negative kaon
    std::vector<POD::Extended::McParticle> MC_NegKaon;
    // -- positive kaon
    std::vector<POD::Extended::McParticle> MC_PosKaon;
    // -- anti-lambda
    std::vector<POD::Extended::McParticle> MC_AntiLambda;
    std::vector<POD::Extended::McParticle> MC_AntiLambda_Neg;
    std::vector<POD::Extended::McParticle> MC_AntiLambda_Pos;
    // -- lambda
    std::vector<POD::Extended::McParticle> MC_Lambda;
    std::vector<POD::Extended::McParticle> MC_Lambda_Neg;
    std::vector<POD::Extended::McParticle> MC_Lambda_Pos;
    // -- kaon-zero-short
    std::vector<POD::Extended::McParticle> MC_KaonZeroShort;
    std::vector<POD::Extended::McParticle> MC_KaonZeroShort_Neg;
    std::vector<POD::Extended::McParticle> MC_KaonZeroShort_Pos;

    Framework::Model CreateModel(bool is_mc) {
        Framework::Model model;
        // event
        model.RegisterField<POD::Event>(&Event, "Event");
        // negative kaon
        model.RegisterField<std::vector<POD::Track>>(&NegKaon, "NegKaon");
        // positive kaon
        model.RegisterField<std::vector<POD::Track>>(&PosKaon, "PosKaon");
        // anti-lambda
        model.RegisterField<std::vector<POD::V0>>(&AntiLambda, "AntiLambda");
        model.RegisterField<std::vector<POD::Track>>(&AntiLambda_Neg, "AntiLambda_Neg");
        model.RegisterField<std::vector<POD::Track>>(&AntiLambda_Pos, "AntiLambda_Pos");
        // lambda
        model.RegisterField<std::vector<POD::V0>>(&Lambda, "Lambda");
        model.RegisterField<std::vector<POD::Track>>(&Lambda_Neg, "Lambda_Neg");
        model.RegisterField<std::vector<POD::Track>>(&Lambda_Pos, "Lambda_Pos");
        // kaon-zero-short
        model.RegisterField<std::vector<POD::V0>>(&KaonZeroShort, "KaonZeroShort");
        model.RegisterField<std::vector<POD::Track>>(&KaonZeroShort_Neg, "KaonZeroShort_Neg");
        model.RegisterField<std::vector<POD::Track>>(&KaonZeroShort_Pos, "KaonZeroShort_Pos");
        // mc
        if (is_mc) {
            // -- event
            model.RegisterField<POD::MC::Event>(&MC_Event, "MC_Event");
            // -- injected
            model.RegisterField<std::vector<POD::Extended::InjectedSexa>>(&InjectedSexa, "InjectedSexa");
            // -- negative kaon
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_NegKaon, "MC_NegKaon");
            // -- positive kaon
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_PosKaon, "MC_PosKaon");
            // -- anti-lambda
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_AntiLambda, "MC_AntiLambda");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_AntiLambda_Neg, "MC_AntiLambda_Neg");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_AntiLambda_Pos, "MC_AntiLambda_Pos");
            // -- lambda
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Lambda, "MC_Lambda");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Lambda_Neg, "MC_Lambda_Neg");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Lambda_Pos, "MC_Lambda_Pos");
            // -- kaon-zero-short
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_KaonZeroShort, "MC_KaonZeroShort");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_KaonZeroShort_Neg, "MC_KaonZeroShort_Neg");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_KaonZeroShort_Pos, "MC_KaonZeroShort_Pos");
        }
        return model;
    }

    // Needs to be called at the end of the event loop.
    void Clear(bool is_mc) {
        // negative kaon
        NegKaon.clear();
        // positive kaon
        PosKaon.clear();
        // anti-lambda
        AntiLambda.clear();
        AntiLambda_Neg.clear();
        AntiLambda_Pos.clear();
        // lambda
        Lambda.clear();
        Lambda_Neg.clear();
        Lambda_Pos.clear();
        // kaon-zero-short
        KaonZeroShort.clear();
        KaonZeroShort_Neg.clear();
        KaonZeroShort_Pos.clear();
        // mc
        if (is_mc) {
            // -- injected
            InjectedSexa.clear();
            // -- negative kaon
            MC_NegKaon.clear();
            // -- positive kaon
            MC_PosKaon.clear();
            // -- anti-lambda
            MC_AntiLambda.clear();
            MC_AntiLambda_Neg.clear();
            MC_AntiLambda_Pos.clear();
            // -- lambda
            MC_Lambda.clear();
            MC_Lambda_Neg.clear();
            MC_Lambda_Pos.clear();
            // -- kaon-zero-short
            MC_KaonZeroShort.clear();
            MC_KaonZeroShort_Neg.clear();
            MC_KaonZeroShort_Pos.clear();
        }
    }
};

}  // namespace Schema
