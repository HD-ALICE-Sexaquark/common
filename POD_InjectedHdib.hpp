#pragma once

#include "Constants.hpp"

namespace POD {

struct InjectedHdib {
    // identifiers
    int SignalID{Common::DummyInt};
    bool IsAntiChannel{false};
    // properties
    float Decay_X{Common::DummyFloat};
    float Decay_Y{Common::DummyFloat};
    float Decay_Z{Common::DummyFloat};
    float Px{Common::DummyFloat};
    float Py{Common::DummyFloat};
    float Pz{Common::DummyFloat};
    float Energy{Common::DummyFloat};
    // lambda 1
    float Lambda1_Decay_X{Common::DummyFloat};
    float Lambda1_Decay_Y{Common::DummyFloat};
    float Lambda1_Decay_Z{Common::DummyFloat};
    float Lambda1_Px{Common::DummyFloat};
    float Lambda1_Py{Common::DummyFloat};
    float Lambda1_Pz{Common::DummyFloat};
    float Lambda1_Energy{Common::DummyFloat};
    // lambda 1's proton daughter
    float Proton1_Px{Common::DummyFloat};
    float Proton1_Py{Common::DummyFloat};
    float Proton1_Pz{Common::DummyFloat};
    float Proton1_Energy{Common::DummyFloat};
    // lambda 1's pion daughter
    float Pion1_Px{Common::DummyFloat};
    float Pion1_Py{Common::DummyFloat};
    float Pion1_Pz{Common::DummyFloat};
    float Pion1_Energy{Common::DummyFloat};
    // lambda 2
    float Lambda2_Decay_X{Common::DummyFloat};
    float Lambda2_Decay_Y{Common::DummyFloat};
    float Lambda2_Decay_Z{Common::DummyFloat};
    float Lambda2_Px{Common::DummyFloat};
    float Lambda2_Py{Common::DummyFloat};
    float Lambda2_Pz{Common::DummyFloat};
    float Lambda2_Energy{Common::DummyFloat};
    // lambda 2's proton daughter
    float Proton2_Px{Common::DummyFloat};
    float Proton2_Py{Common::DummyFloat};
    float Proton2_Pz{Common::DummyFloat};
    float Proton2_Energy{Common::DummyFloat};
    // lambda 2's pion daughter
    float Pion2_Px{Common::DummyFloat};
    float Pion2_Py{Common::DummyFloat};
    float Pion2_Pz{Common::DummyFloat};
    float Pion2_Energy{Common::DummyFloat};
};

}  // namespace POD
