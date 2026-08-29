#pragma once

#include "Constants.hpp"

namespace POD {

struct InjectedSexa {
    int ReactionID{Common::DummyInt};
    float Px{Common::DummyFloat};
    float Py{Common::DummyFloat};
    float Pz{Common::DummyFloat};
    float Nucleon_Px{Common::DummyFloat};
    float Nucleon_Py{Common::DummyFloat};
    float Nucleon_Pz{Common::DummyFloat};
};

namespace Extended {

struct InjectedSexa : POD::InjectedSexa {
    float SV_X{Common::DummyFloat};
    float SV_Y{Common::DummyFloat};
    float SV_Z{Common::DummyFloat};
    float After_Px{Common::DummyFloat};
    float After_Py{Common::DummyFloat};
    float After_Pz{Common::DummyFloat};
    float After_Energy{Common::DummyFloat};
    float Energy{Common::DummyFloat};
    float Nucleon_Energy{Common::DummyFloat};
};

}  // namespace Extended

namespace Linked {

struct InjectedSexa : Extended::InjectedSexa {
    bool IsTrueSignal{false};  // all daughters are true products of this very reaction
    bool IsRealBkg{false};     // no daughter descends from any injected reaction
};

}  // namespace Linked

}  // namespace POD
