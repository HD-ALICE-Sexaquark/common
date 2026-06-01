#pragma once

#include "DB_Particles.hpp"

// H-Dibaryon library.
namespace HD {

struct DecayTree {
    DB::Particles::Definition hdibaryon;
    DB::Particles::Definition lambda;
    DB::Particles::Definition neg;  // lambda's negative daughter
    DB::Particles::Definition pos;  // lambda's positive daughter
};

inline constexpr DecayTree Channel = {
    DB::Particles::Particle("Hdibaryon"),
    DB::Particles::Particle("Lambda"),
    DB::Particles::Particle("PiMinus"),
    DB::Particles::Particle("Proton"),
};
inline constexpr DecayTree AntiChannel = {
    DB::Particles::Particle("AntiHdibaryon"),
    DB::Particles::Particle("AntiLambda"),
    DB::Particles::Particle("PiPlus"),
    DB::Particles::Particle("AntiProton"),
};

constexpr const DecayTree& GetDecayTree(bool anti_channel) noexcept { return anti_channel ? AntiChannel : Channel; }

}  // namespace HD
