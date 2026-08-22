#pragma once

#include <array>
#include <optional>
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
    std::optional<std::size_t> idx_neg_dau;
    std::optional<std::size_t> idx_pos_dau;
};

inline constexpr std::array DB = std::to_array<Definition>({
    {"Photon", "G", 22, 0, 0.0, 1e20, {}, std::nullopt, std::nullopt},
    //
    {"Electron", "EL", 11, -1, 0.00051100, 1e20, {}, std::nullopt, std::nullopt},
    {"Positron", "PO", -11, +1, 0.00051100, 1e20, {}, std::nullopt, std::nullopt},
    //
    {"PiMinus", "PM", -211, -1, Common::PdgMass_Pion, 780.45, {}, std::nullopt, std::nullopt},
    {"PiPlus", "PP", 211, +1, Common::PdgMass_Pion, 780.45, {}, std::nullopt, std::nullopt},
    {"PiZero", "P0", 111, 0, 0.13497680, 2.5e-6, DecayProducts_PiZero, std::nullopt, std::nullopt},
    //
    {"NegKaon", "NK", -321, -1, Common::PdgMass_Kaon, 371.1, {}, std::nullopt, std::nullopt},
    {"PosKaon", "PK", 321, +1, Common::PdgMass_Kaon, 371.1, {}, std::nullopt, std::nullopt},
    {"KaonZeroShort", "K0S", 310, 0, Common::PdgMass_KaonZeroShort, 2.6844, DecayProducts_KaonZeroShort, 0, 1},
    //
    {"AntiProton", "AP", -2212, -1, Common::PdgMass_Proton, 1e20, {}, std::nullopt, std::nullopt},
    {"Proton", "P", 2212, +1, Common::PdgMass_Proton, 1e20, {}, std::nullopt, std::nullopt},
    //
    {"AntiNeutron", "AN", -2112, 0, Common::PdgMass_Neutron, 1e20, {}, std::nullopt, std::nullopt},
    {"Neutron", "N", 2112, 0, Common::PdgMass_Neutron, 1e20, {}, std::nullopt, std::nullopt},
    //
    {"AntiLambda", "AL", -3122, 0, Common::PdgMass_Lambda, 7.89, DecayProducts_AntiLambda, 0, 1},
    {"Lambda", "L", 3122, 0, Common::PdgMass_Lambda, 7.89, DecayProducts_Lambda, 1, 0},
    //
    {"XiPlus", "XP", -3312, +1, 1.3217100, 4.91, DecayProducts_XiPlus, std::nullopt, std::nullopt},
    //
    // NOTE: ctau obtained from Gamma=0.002; the mass is a hypothesis, so it must never be constrained
    {"AntiHdibaryon", "AH", -1020000020, 0, 2.234, 9.866349e-12, DecayProducts_AntiHdibaryon, std::nullopt, std::nullopt},
    {"Hdibaryon", "H", 1020000020, 0, 2.234, 9.866349e-12, DecayProducts_Hdibaryon, std::nullopt, std::nullopt},
    //
    {"Unknown", "00", Common::DummyNNN, Common::DummyNNN, Common::DummyDouble, Common::DummyDouble, {}, std::nullopt, std::nullopt},
});

consteval std::size_t Index(int pdg) {
    for (std::size_t i = 0; i < DB.size(); ++i)
        if (DB[i].pdg_code == pdg) return i;
    throw "DB::Particles::Index: unknown pdg code";
}

consteval Definition Particle(int pdg) {
    for (const auto& i : DB)
        if (i.pdg_code == pdg) return i;
    throw "DB::Particles::Particle: unknown pdg code";
}

consteval Definition Particle(std::string_view name) {
    for (const auto& i : DB)
        if (i.name == name) return i;
    throw "DB::Particles::Particle: unknown name";
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

constexpr bool IsStable(const Definition& p) noexcept { return p.daughters_pdg.empty(); }

// Sum of the masses of a particle's decay products, i.e. the lowest mass it can physically be reconstructed at.
constexpr double SumDaughterMass(const Definition& p) noexcept {
    double sum = 0.;
    for (int pdg : p.daughters_pdg) sum += FindParticle(pdg).mass;
    return sum;
}

}  // namespace DB::Particles
