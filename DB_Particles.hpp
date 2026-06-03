#pragma once

#include <array>
#include <span>
#include <string_view>

#include "Constants.hpp"

namespace DB::Particles {

inline constexpr std::array DecayProducts_KaonZeroShort = std::to_array<int>({-211, 211});
inline constexpr std::array DecayProducts_AntiLambda = std::to_array<int>({-2212, 211});
inline constexpr std::array DecayProducts_Lambda = std::to_array<int>({2212, -211});

inline constexpr std::array DecayProducts_PiZero = std::to_array<int>({22, 22});
inline constexpr std::array DecayProducts_XiPlus = std::to_array<int>({-3122, 211});  // Xi+ -> AntiLambda Pi+

inline constexpr std::array DecayProducts_Hdibaryon = std::to_array<int>({3122, 3122});
inline constexpr std::array DecayProducts_AntiHdibaryon = std::to_array<int>({-3122, -3122});

struct Definition {
    std::string_view name;               // canonical name
    std::string_view acronym;            // short name
    int pdg_code;                        // PDG MC numbering scheme
    int charge;                          // in units of e
    double mass;                         // (GeV/c^2)
    double ctau;                         // (cm)
    std::span<const int> daughters_pdg;  // NOTE: only detectable charged modes
    std::size_t idx_neg_dau;
    std::size_t idx_pos_dau;
};

inline constexpr std::array DB = std::to_array<Definition>({
    {"Photon", "G", 22, 0, 0.0, 1e20, {}, 0, 0},
    //
    {"Electron", "EL", 11, -1, 0.00051100, 1e20, {}, 0, 0},
    {"Positron", "PO", -11, +1, 0.00051100, 1e20, {}, 0, 0},
    //
    {"PiMinus", "PM", -211, -1, 0.13957040, 780.45, {}, 0, 0},
    {"PiPlus", "PP", 211, +1, 0.13957040, 780.45, {}, 0, 0},
    {"PiZero", "P0", 111, 0, 0.13497680, 2.5e-6, DecayProducts_PiZero, 0, 0},
    //
    {"NegKaon", "NK", -321, -1, 0.49367700, 371.1, {}, 0, 0},
    {"PosKaon", "PK", 321, +1, 0.49367700, 371.1, {}, 0, 0},
    {"KaonZeroShort", "K0S", 310, 0, 0.49761100, 2.6844, DecayProducts_KaonZeroShort, 0, 1},
    //
    {"AntiProton", "AP", -2212, -1, 0.93827210, 1e20, {}, 0, 0},
    {"Proton", "P", 2212, +1, 0.93827210, 1e20, {}, 0, 0},
    //
    {"AntiNeutron", "AN", -2112, 0, 0.93956540, 1e20, {}, 0, 0},
    {"Neutron", "N", 2112, 0, 0.93956540, 1e20, {}, 0, 0},
    //
    {"AntiLambda", "AL", -3122, 0, 1.1156830, 7.89, DecayProducts_AntiLambda, 0, 1},
    {"Lambda", "L", 3122, 0, 1.1156830, 7.89, DecayProducts_Lambda, 1, 0},
    //
    {"XiPlus", "XP", -3312, +1, 1.3217100, 4.91, DecayProducts_XiPlus, 0, 1},
    //
    {"AntiHdibaryon", "AH", -1020000020, 0, 2.234, 9.866349e-12, DecayProducts_AntiHdibaryon, 0, 0},  // NOTE: ctau obtained from Gamma=0.002
    {"Hdibaryon", "H", 1020000020, 0, 2.234, 9.866349e-12, DecayProducts_Hdibaryon, 0, 0},            // NOTE: ctau obtained from Gamma=0.002
    //
    {"Unknown", "00", Common::DummyNNN, Common::DummyNNN, Common::DummyFloat, Common::DummyFloat, {}, 0, 0},
});

consteval std::size_t Index(int pdg) {
    for (std::size_t i = 0; i < DB.size(); ++i)
        if (DB[i].pdg_code == pdg) return i;
    return DB.size() - 1;
}

consteval Definition Particle(int pdg) {
    for (const auto& i : DB)
        if (i.pdg_code == pdg) return i;
    return DB.back();
}

consteval Definition Particle(std::string_view name) {
    for (const auto& i : DB)
        if (i.name == name) return i;
    return DB.back();
}

constexpr std::size_t FindIndex(int pdg) noexcept {
    for (std::size_t i = 0; i < DB.size(); ++i)
        if (DB[i].pdg_code == pdg) return i;
    return DB.size() - 1;
}

constexpr Definition FindParticle(int pdg) noexcept {
    for (auto const& p : DB)
        if (p.pdg_code == pdg) return p;
    return DB.back();
}

constexpr Definition FindParticle(std::string_view name) noexcept {
    for (auto const& p : DB)
        if (p.name == name) return p;
    return DB.back();
}

}  // namespace DB::Particles
