#pragma once

#include <vector>

#include "Framework.hpp"
#include "POD_Event.hpp"
#include "POD_InjectedSexa.hpp"
#include "POD_McParticle.hpp"
#include "POD_Sexaquark.hpp"
#include "POD_Track.hpp"
#include "POD_V0.hpp"

namespace Schema {

struct FoundSexaquark {
    // reconstructed
    POD::Event Event;
    // -- channel a
    std::vector<POD::Sexaquark> ChannelA;
    std::vector<POD::V0> ChannelA_V0A;
    std::vector<POD::Track> ChannelA_V0A_Neg;
    std::vector<POD::Track> ChannelA_V0A_Pos;
    std::vector<POD::V0> ChannelA_V0B;
    std::vector<POD::Track> ChannelA_V0B_Neg;
    std::vector<POD::Track> ChannelA_V0B_Pos;
    // -- channel d
    std::vector<POD::Sexaquark> ChannelD;
    std::vector<POD::V0> ChannelD_V0;
    std::vector<POD::Track> ChannelD_V0_Neg;
    std::vector<POD::Track> ChannelD_V0_Pos;
    std::vector<POD::Track> ChannelD_Kaon;
    // -- channel h
    std::vector<POD::Sexaquark> ChannelH;
    std::vector<POD::Track> ChannelH_Kaon1;
    std::vector<POD::Track> ChannelH_Kaon2;

    // mc only
    POD::MC::Event MC_Event;
    std::vector<POD::Extended::InjectedSexa> Injected;  // total injected signal, useful for efficiency studies
    // -- channel a
    std::vector<POD::Linked::InjectedSexa> MC_ChannelA;  // linked mc
    std::vector<POD::Extended::McParticle> MC_ChannelA_V0A;
    std::vector<POD::Extended::McParticle> MC_ChannelA_V0A_Neg;
    std::vector<POD::Extended::McParticle> MC_ChannelA_V0A_Pos;
    std::vector<POD::Extended::McParticle> MC_ChannelA_V0B;
    std::vector<POD::Extended::McParticle> MC_ChannelA_V0B_Neg;
    std::vector<POD::Extended::McParticle> MC_ChannelA_V0B_Pos;
    // -- channel d
    std::vector<POD::Linked::InjectedSexa> MC_ChannelD;  // linked mc
    std::vector<POD::Extended::McParticle> MC_ChannelD_V0;
    std::vector<POD::Extended::McParticle> MC_ChannelD_V0_Neg;
    std::vector<POD::Extended::McParticle> MC_ChannelD_V0_Pos;
    std::vector<POD::Extended::McParticle> MC_ChannelD_Kaon;
    // -- channel h
    std::vector<POD::Linked::InjectedSexa> MC_ChannelH;  // linked mc
    std::vector<POD::Extended::McParticle> MC_ChannelH_Kaon1;
    std::vector<POD::Extended::McParticle> MC_ChannelH_Kaon2;

    Framework::Model CreateModel(bool is_mc) {
        Framework::Model model;
        // reconstructed
        model.RegisterField<POD::Event>(&Event, "Event");
        // -- channel a
        model.RegisterField<std::vector<POD::Sexaquark>>(&ChannelA, "ChannelA");
        model.RegisterField<std::vector<POD::V0>>(&ChannelA_V0A, "ChannelA_V0A");
        model.RegisterField<std::vector<POD::Track>>(&ChannelA_V0A_Neg, "ChannelA_V0A_Neg");
        model.RegisterField<std::vector<POD::Track>>(&ChannelA_V0A_Pos, "ChannelA_V0A_Pos");
        model.RegisterField<std::vector<POD::V0>>(&ChannelA_V0B, "ChannelA_V0B");
        model.RegisterField<std::vector<POD::Track>>(&ChannelA_V0B_Neg, "ChannelA_V0B_Neg");
        model.RegisterField<std::vector<POD::Track>>(&ChannelA_V0B_Pos, "ChannelA_V0B_Pos");
        // -- channel d
        model.RegisterField<std::vector<POD::Sexaquark>>(&ChannelD, "ChannelD");
        model.RegisterField<std::vector<POD::V0>>(&ChannelD_V0, "ChannelD_V0");
        model.RegisterField<std::vector<POD::Track>>(&ChannelD_V0_Neg, "ChannelD_V0_Neg");
        model.RegisterField<std::vector<POD::Track>>(&ChannelD_V0_Pos, "ChannelD_V0_Pos");
        model.RegisterField<std::vector<POD::Track>>(&ChannelD_Kaon, "ChannelD_Kaon");
        // -- channel h
        model.RegisterField<std::vector<POD::Sexaquark>>(&ChannelH, "ChannelH");
        model.RegisterField<std::vector<POD::Track>>(&ChannelH_Kaon1, "ChannelH_Kaon1");
        model.RegisterField<std::vector<POD::Track>>(&ChannelH_Kaon2, "ChannelH_Kaon2");

        if (is_mc) {
            model.RegisterField<POD::MC::Event>(&MC_Event, "MC_Event");
            model.RegisterField<std::vector<POD::Extended::InjectedSexa>>(&Injected, "Injected");
            // -- channel a
            model.RegisterField<std::vector<POD::Linked::InjectedSexa>>(&MC_ChannelA, "MC_ChannelA");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_ChannelA_V0A, "MC_ChannelA_V0A");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_ChannelA_V0A_Neg, "MC_ChannelA_V0A_Neg");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_ChannelA_V0A_Pos, "MC_ChannelA_V0A_Pos");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_ChannelA_V0B, "MC_ChannelA_V0B");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_ChannelA_V0B_Neg, "MC_ChannelA_V0B_Neg");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_ChannelA_V0B_Pos, "MC_ChannelA_V0B_Pos");
            // -- channel d
            model.RegisterField<std::vector<POD::Linked::InjectedSexa>>(&MC_ChannelD, "MC_ChannelD");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_ChannelD_V0, "MC_ChannelD_V0");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_ChannelD_V0_Neg, "MC_ChannelD_V0_Neg");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_ChannelD_V0_Pos, "MC_ChannelD_V0_Pos");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_ChannelD_Kaon, "MC_ChannelD_Kaon");
            // -- channel h
            model.RegisterField<std::vector<POD::Linked::InjectedSexa>>(&MC_ChannelH, "MC_ChannelH");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_ChannelH_Kaon1, "MC_ChannelH_Kaon1");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_ChannelH_Kaon2, "MC_ChannelH_Kaon2");
        }
        return model;
    }

    void Clear(bool is_mc) {
        // -- channel a
        ChannelA.clear();
        ChannelA_V0A.clear();
        ChannelA_V0A_Neg.clear();
        ChannelA_V0A_Pos.clear();
        ChannelA_V0B.clear();
        ChannelA_V0B_Neg.clear();
        ChannelA_V0B_Pos.clear();
        // -- channel d
        ChannelD.clear();
        ChannelD_V0.clear();
        ChannelD_V0_Neg.clear();
        ChannelD_V0_Pos.clear();
        ChannelD_Kaon.clear();
        // -- channel h
        ChannelH.clear();
        ChannelH_Kaon1.clear();
        ChannelH_Kaon2.clear();
        if (is_mc) {
            Injected.clear();
            // -- channel a
            MC_ChannelA.clear();
            MC_ChannelA_V0A.clear();
            MC_ChannelA_V0A_Neg.clear();
            MC_ChannelA_V0A_Pos.clear();
            MC_ChannelA_V0B.clear();
            MC_ChannelA_V0B_Neg.clear();
            MC_ChannelA_V0B_Pos.clear();
            // -- channel d
            MC_ChannelD.clear();
            MC_ChannelD_V0.clear();
            MC_ChannelD_V0_Neg.clear();
            MC_ChannelD_V0_Pos.clear();
            MC_ChannelD_Kaon.clear();
            // -- channel h
            MC_ChannelH.clear();
            MC_ChannelH_Kaon1.clear();
            MC_ChannelH_Kaon2.clear();
        }
    }
};

}  // namespace Schema
