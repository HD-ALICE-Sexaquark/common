#pragma once

#include <vector>

#include "Framework.hpp"
#include "POD_Event.hpp"
#include "POD_InjectedHdib.hpp"
#include "POD_LambdaPair.hpp"
#include "POD_McParticle.hpp"
#include "POD_PreFoundLambda.hpp"

namespace Schema {

struct FoundHdibaryon {
    POD::Event Event;
    std::vector<POD::LambdaPair> Hdibaryon;
    std::vector<POD::Extended::PreFoundLambda> Lambda1;
    std::vector<POD::Extended::PreFoundLambda> Lambda2;
    // mc only
    POD::MC::Event MC_Event;
    std::vector<POD::InjectedHdib> Injected;              // total injected signal, useful for efficiency studies
    std::vector<POD::Extended::McParticle> MC_Hdibaryon;  // linked mc
    std::vector<POD::Extended::McParticle> MC_Lambda1;
    std::vector<POD::Extended::McParticle> MC_Lambda1_Neg;
    std::vector<POD::Extended::McParticle> MC_Lambda1_Pos;
    std::vector<POD::Extended::McParticle> MC_Lambda2;
    std::vector<POD::Extended::McParticle> MC_Lambda2_Neg;
    std::vector<POD::Extended::McParticle> MC_Lambda2_Pos;

    Framework::Model CreateModel(bool is_mc) {
        Framework::Model model;
        model.RegisterField<POD::Event>(&Event, "Event");
        model.RegisterField<std::vector<POD::LambdaPair>>(&Hdibaryon, "Hdibaryon");
        model.RegisterField<std::vector<POD::Extended::PreFoundLambda>>(&Lambda1, "Lambda1");
        model.RegisterField<std::vector<POD::Extended::PreFoundLambda>>(&Lambda2, "Lambda2");
        if (is_mc) {
            model.RegisterField<POD::MC::Event>(&MC_Event, "MC_Event");
            model.RegisterField<std::vector<POD::InjectedHdib>>(&Injected, "Injected");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Hdibaryon, "MC_Hdibaryon");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Lambda1, "MC_Lambda1");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Lambda1_Neg, "MC_Lambda1_Neg");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Lambda1_Pos, "MC_Lambda1_Pos");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Lambda2, "MC_Lambda2");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Lambda2_Neg, "MC_Lambda2_Neg");
            model.RegisterField<std::vector<POD::Extended::McParticle>>(&MC_Lambda2_Pos, "MC_Lambda2_Pos");
        }
        return model;
    }

    void Clear(bool is_mc) {
        Hdibaryon.clear();
        Lambda1.clear();
        Lambda2.clear();
        if (is_mc) {
            Injected.clear();
            MC_Hdibaryon.clear();
            MC_Lambda1.clear();
            MC_Lambda1_Neg.clear();
            MC_Lambda1_Pos.clear();
            MC_Lambda2.clear();
            MC_Lambda2_Neg.clear();
            MC_Lambda2_Pos.clear();
        }
    }
};

}  // namespace Schema
