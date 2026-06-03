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
    float Energy{Common::DummyFloat};
    float Nucleon_Energy{Common::DummyFloat};
};

}  // namespace Extended

namespace Linked {

struct InjectedSexa : Extended::InjectedSexa {
    bool IsHybrid{false};
};

}  // namespace Linked

}  // namespace POD
