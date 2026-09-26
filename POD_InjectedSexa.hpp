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

}  // namespace POD
