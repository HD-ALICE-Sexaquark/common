#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <random>
#include <span>
#include <utility>
#include <vector>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>

// 3D model of the material a ray from the origin traverses, transcribed from AliRoot's `AliTPCv4::CreateGeometry()`
// plus an approximate beam pipe + ITS. The nested TGeo volumes are flattened into a list of mutually disjoint
// polycones; air is not listed -- it fills the gaps between solids (see `TraceRay`). Muon side = negative z, shaft side
// = positive z. All lengths in cm, densities in g/cm^3.
//
// Omitted on purpose:
// - endcaps (readout chambers, wheels, service support wheels) and anything at |z| > 253.6, including the ICV/OCV/OFC
//   end flanges that live there
// - the 36 makrolon support rods (r = 81.5 and 254.25, ~15% azimuthal coverage)
// - the central HV membrane and its hub rings
// - central drum seals and collars (TPC_CDRS, TPC_CDCE, TPC_CDCI), guard-ring resistors
// - isotope abundances and the choice of nucleon inside the struck nucleus (GEANT4 does both; the reaction channel
//   fixes the nucleon here, and hydrogen is not special-cased)
namespace FastSim::Geometry {

// ## What a Ray Can Hit ## //

// One nuclear species of a material: its mass number and its weight within the material. The weight is either a mass
// fraction or an atom count -- the two conventions are mixed on purpose, as in the AliRoot/GEANT4 sources this was
// transcribed from -- and is normalised at sampling.
struct Component {
    int A;
    double weight;
};

struct Material {
    double density;
    std::span<const Component> components;
};

// -- pure elements
inline constexpr std::array Components_Be = std::to_array<Component>({{9, 1.}});
inline constexpr std::array Components_Si = std::to_array<Component>({{28, 1.}});
inline constexpr std::array Components_Al = std::to_array<Component>({{27, 1.}});
// -- mixtures, by mass fraction (C N O Ar; Fe Cr Ni; C H O; C H O Si; C H O Si; Ne C O N)
inline constexpr std::array Components_Air = std::to_array<Component>({{12, 0.000124}, {14, 0.755267}, {16, 0.231781}, {40, 0.012827}});
inline constexpr std::array Components_Steel = std::to_array<Component>({{56, 0.7462}, {52, 0.1690}, {59, 0.0848}});
inline constexpr std::array Components_Prepreg1 = std::to_array<Component>({{12, 0.923}, {1, 0.023}, {16, 0.054}});
inline constexpr std::array Components_Prepreg2 = std::to_array<Component>({{12, 0.194}, {1, 0.023}, {16, 0.443}, {28, 0.34}});
inline constexpr std::array Components_Prepreg3 = std::to_array<Component>({{12, 0.257}, {1, 0.03}, {16, 0.412}, {28, 0.3}});
inline constexpr std::array Components_DriftGas = std::to_array<Component>({{20, 0.758}, {12, 0.050}, {16, 0.134}, {14, 0.058}});
// -- compounds, by atom count (H2 O; C O2; C14 H20 O3; C2 H3 F; C14 H22 O2 N2)
inline constexpr std::array Components_Water = std::to_array<Component>({{1, 2.}, {16, 1.}});
inline constexpr std::array Components_CO2 = std::to_array<Component>({{12, 1.}, {16, 2.}});
inline constexpr std::array Components_Epoxy = std::to_array<Component>({{12, 14.}, {1, 20.}, {16, 3.}});
inline constexpr std::array Components_Tedlar = std::to_array<Component>({{12, 2.}, {1, 3.}, {19, 1.}});
inline constexpr std::array Components_Nomex = std::to_array<Component>({{12, 14.}, {1, 22.}, {16, 2.}, {14, 2.}});

enum class EMaterial : std::uint8_t {
    kAir,
    kBe,
    kSi,
    kAl,
    kSteel,
    kWater,
    kCO2,
    kEpoxy,
    kTedlar,
    kPrepreg1,
    kPrepreg2,
    kPrepreg3,
    kNomex,
    kDriftGas,
    kNMaterials,
};

// Indexed by `EMaterial`.
inline constexpr std::array<Material, static_cast<std::size_t>(EMaterial::kNMaterials)> Materials{{
    {0.00120479, Components_Air},
    {1.85, Components_Be},
    {2.33, Components_Si},
    {2.70, Components_Al},
    {8.00, Components_Steel},
    {1., Components_Water},
    {0.00184212, Components_CO2},
    {1.25, Components_Epoxy},
    {1.71, Components_Tedlar},
    {1.859, Components_Prepreg1},
    {1.82, Components_Prepreg2},
    {1.725, Components_Prepreg3},
    {0.029, Components_Nomex},
    {0.00095, Components_DriftGas},  // Ne-CO2-N2 (85.72/9.52/4.76 % vol), M = 22.82 g/mol / 24.05 L/mol at 20 C, 1 atm
}};

[[nodiscard]] constexpr const Material& MaterialOf(EMaterial material) { return Materials[static_cast<std::size_t>(material)]; }

// The mass number of the nucleus struck in `material`, each component weighted by its weight.
template <std::uniform_random_bit_generator G>
[[nodiscard]] int SampleStruckMassNumber(G& rng, EMaterial material) {
    const std::span<const Component> components = MaterialOf(material).components;
    double sum_w = 0.;
    for (const auto& c : components) sum_w += c.weight;
    double roll = std::uniform_real_distribution<double>{0., sum_w}(rng);
    for (const auto& c : components) {
        roll -= c.weight;
        if (roll <= 0.) return c.A;
    }
    return components.back().A;
}

// ## The Solids ## //

// A section of a cone, symmetric around the beam axis: linear inner and outer radii between `z_min` and `z_max`.
struct Polycone {
    double z_min;
    double z_max;
    double r_inner_at_zmin;
    double r_inner_at_zmax;
    double r_outer_at_zmin;
    double r_outer_at_zmax;
    EMaterial material;
};

consteval Polycone Cylinder(double z_min, double z_max, double r_min, double r_max, EMaterial material) {
    return {z_min, z_max, r_min, r_min, r_max, r_max, material};
}

consteval Polycone Cone(double z_min, double z_max, double r_inner_at_zmin, double r_inner_at_zmax, double r_outer_at_zmin, double r_outer_at_zmax,
                        EMaterial material) {
    return {z_min, z_max, r_inner_at_zmin, r_inner_at_zmax, r_outer_at_zmin, r_outer_at_zmax, material};
}

// Tracked region: the TPC barrel. Rays are traced from the origin until they exit it.
inline constexpr double Boundary_HalfZ = 253.6;  // endcap plane (TPC_M sections 7/22)
inline constexpr double Boundary_R = 278.;       // TPC_M outer radius

// clang-format off
inline constexpr std::array Polycones = std::to_array<Polycone>({
    // -- Beam pipe + ITS (approximate; half-lengths from the ITS TDR) -- //
    Cylinder(-Boundary_HalfZ, Boundary_HalfZ, 2.9, 2.98, EMaterial::kBe),  // beam pipe (real Be section is shorter in z)
    Cylinder(-14.1, 14.1, 4.0, 4.01, EMaterial::kSi),                      // SPD 1
    Cylinder(-14.1, 14.1, 7.2, 7.21, EMaterial::kSi),                      // SPD 2
    Cylinder(-22.2, 22.2, 15.0, 15.01, EMaterial::kSi),                    // SDD 1
    Cylinder(-29.7, 29.7, 23.9, 23.91, EMaterial::kSi),                    // SDD 2
    Cylinder(-43.1, 43.1, 38.5, 38.51, EMaterial::kSi),                    // SSD 1
    Cylinder(-48.9, 48.9, 43.6, 43.61, EMaterial::kSi),                    // SSD 2
    // -- Inner containment vessel (TPC_ICVM / TPC_ICVS, Al) -- //
    Cone(-253.6, -74.0, 68.1, 60.9, 68.4, 61.2, EMaterial::kAl),  // muon cone
    Cylinder(-74.0, -73.3, 60.1, 61.2, EMaterial::kAl),           // muon rings
    Cylinder(-73.3, -73.0, 56.9, 61.2, EMaterial::kAl),
    Cylinder(-73.0, -71.3, 56.9, 58.8, EMaterial::kAl),
    Cylinder(71.3, 73.0, 56.9, 58.8, EMaterial::kAl),  // shaft rings
    Cylinder(73.0, 73.3, 56.9, 61.2, EMaterial::kAl),
    Cylinder(73.3, 74.0, 60.1, 61.2, EMaterial::kAl),
    Cone(74.0, 253.6, 60.9, 65.6, 61.2, 65.9, EMaterial::kAl),  // shaft cone
    // -- Heat screens (TPC_HSM / TPC_HSS): steel walls with a water layer inside -- //
    Cone(-253.6, -74.0, 67.88, 60.68, 67.94, 60.74, EMaterial::kSteel),  // muon
    Cone(-253.6, -74.0, 67.94, 60.74, 68.04, 60.84, EMaterial::kWater),
    Cone(-253.6, -74.0, 68.04, 60.84, 68.1, 60.9, EMaterial::kSteel),
    Cone(74.0, 253.6, 60.68, 65.38, 60.74, 65.44, EMaterial::kSteel),  // shaft
    Cone(74.0, 253.6, 60.74, 65.44, 60.84, 65.54, EMaterial::kWater),
    Cone(74.0, 253.6, 60.84, 65.54, 60.9, 65.6, EMaterial::kSteel),
    // -- Central drum (TPC_CDR + sandwich): Al flanges + Al/tedlar/prepreg1/nomex barrel -- //
    // (the sandwich actually extends to |z| < 71.1, carving into the flanges; ignored)
    Cylinder(-71.1, -68.6, 59.7, 61.2, EMaterial::kAl),  // flange rings
    Cylinder(68.6, 71.1, 59.7, 61.2, EMaterial::kAl),
    Cylinder(-68.6, 68.6, 60.6324, 60.6424, EMaterial::kAl),
    Cylinder(-68.6, 68.6, 60.6424, 60.6462, EMaterial::kTedlar),
    Cylinder(-68.6, 68.6, 60.6462, 60.6662, EMaterial::kPrepreg1),
    Cylinder(-68.6, 68.6, 60.6662, 61.1662, EMaterial::kNomex),
    Cylinder(-68.6, 68.6, 61.1662, 61.1862, EMaterial::kPrepreg1),
    Cylinder(-68.6, 68.6, 61.1862, 61.19, EMaterial::kTedlar),
    Cylinder(-68.6, 68.6, 61.19, 61.2, EMaterial::kAl),
    // -- Inner insulator (TPC_INI, CO2): from the ICV outer surface up to the IFC -- //
    // (split around the Al IFC flange rings, TPC_IFC1; 68.28 / 65.82 = inner radius interpolated at |z| = 250.6)
    Cone(-253.6, -250.6, 68.4, 68.28, 69.9, 69.9, EMaterial::kCO2),
    Cylinder(-253.6, -250.6, 69.9, 76.6774, EMaterial::kAl),  // TPC_IFC1 ring, muon
    Cone(-250.6, -74.0, 68.28, 61.2, 76.6774, 76.6774, EMaterial::kCO2),
    Cylinder(-74.0, 74.0, 61.2, 76.6774, EMaterial::kCO2),
    Cone(74.0, 250.6, 61.2, 65.82, 76.6774, 76.6774, EMaterial::kCO2),
    Cone(250.6, 253.6, 65.82, 65.9, 69.9, 69.9, EMaterial::kCO2),
    Cylinder(250.6, 253.6, 69.9, 76.6774, EMaterial::kAl),  // TPC_IFC1 ring, shaft
    // -- Inner field cage (TPC_IFC): tedlar/prepreg2/prepreg3/nomex sandwich + Al end rings -- //
    // (outermost-part layering used for the whole length; middle/central parts differ by < 1 mm)
    Cylinder(-252.1, 252.1, 76.6774, 76.6812, EMaterial::kTedlar),
    Cylinder(-252.1, 252.1, 76.6812, 76.7312, EMaterial::kPrepreg2),
    Cylinder(-252.1, 252.1, 76.7312, 76.7612, EMaterial::kPrepreg3),
    Cylinder(-252.1, 252.1, 76.7612, 78.7612, EMaterial::kNomex),
    Cylinder(-252.1, 252.1, 78.7612, 78.7912, EMaterial::kPrepreg3),
    Cylinder(-252.1, 252.1, 78.7912, 78.8412, EMaterial::kPrepreg2),
    Cylinder(-252.1, 252.1, 78.8412, 78.845, EMaterial::kTedlar),
    Cylinder(-253.6, -252.1, 76.6774, 78.845, EMaterial::kAl),  // TPC_IFC2 ring, muon
    Cylinder(252.1, 253.6, 76.6774, 78.845, EMaterial::kAl),    // TPC_IFC2 ring, shaft
    // -- Drift volume (TPC_Drift, Ne-CO2-N2) -- //
    Cylinder(-253.6, 253.6, 78.845, 258.0, EMaterial::kDriftGas),
    // -- Outer field cage (TPC_TOFC): tedlar/prepreg3/nomex sandwich + Al end flanges -- //
    // (sandwich actually extends to |z| < 252.1, carving into the flanges; ignored)
    Cylinder(-250.6, 250.6, 258.0, 258.0038, EMaterial::kTedlar),
    Cylinder(-250.6, 250.6, 258.0038, 258.0338, EMaterial::kPrepreg3),
    Cylinder(-250.6, 250.6, 258.0338, 260.0338, EMaterial::kNomex),
    Cylinder(-250.6, 250.6, 260.0338, 260.0638, EMaterial::kPrepreg3),
    Cylinder(-250.6, 250.6, 260.0638, 260.0676, EMaterial::kTedlar),
    Cylinder(-253.6, -250.6, 258.0, 269.6, EMaterial::kAl),  // flange, muon
    Cylinder(250.6, 253.6, 258.0, 275.5, EMaterial::kAl),    // flange, shaft
    // -- Outer insulator (TPC_OI, CO2) -- //
    Cylinder(-253.6, -250.6, 269.6, 274.8124, EMaterial::kCO2),
    Cylinder(-250.6, 247.6, 260.0676, 274.8124, EMaterial::kCO2),
    Cylinder(247.6, 250.6, 260.0676, 270.4, EMaterial::kCO2),
    // -- Outer containment vessel (TPC_OCV): Al skins + epoxy/tedlar/prepreg2/nomex sandwich -- //
    Cylinder(-253.6, 250.6, 274.8124, 274.8174, EMaterial::kAl),
    Cylinder(-253.6, 250.6, 274.8174, 274.8274, EMaterial::kEpoxy),
    Cylinder(-253.6, 250.6, 274.8274, 274.8312, EMaterial::kTedlar),
    Cylinder(-253.6, 250.6, 274.8312, 274.9062, EMaterial::kPrepreg2),
    Cylinder(-253.6, 250.6, 274.9062, 277.9062, EMaterial::kNomex),
    Cylinder(-253.6, 250.6, 277.9062, 277.9812, EMaterial::kPrepreg2),
    Cylinder(-253.6, 250.6, 277.9812, 277.985, EMaterial::kTedlar),
    Cylinder(-253.6, 250.6, 277.985, 277.995, EMaterial::kEpoxy),
    Cylinder(-253.6, 250.6, 277.995, 278.0, EMaterial::kAl),
    Cylinder(247.6, 250.6, 270.4, 274.8124, EMaterial::kAl),  // flange, shaft
});
// clang-format on

// ## Ray Tracing ## //

// A stretch of one material along a ray, `t` in units of the ray's direction vector.
struct Segment {
    double t_enter;
    double t_exit;
    EMaterial material;
};

// The full, gapless list of segments a ray from the origin traverses until it exits the tracked region
// (`|z| <= Boundary_HalfZ`, `r <= Boundary_R`), sorted by `t_enter`; the gaps between the solids are air.
// `dir` must be nonzero: its magnitude cancels out, `t` is in units of `|dir|`.
inline std::vector<Segment> TraceRay(const ROOT::Math::XYZVector& dir) {

    const double dx = dir.X();
    const double dy = dir.Y();
    const double dz = dir.Z();
    const double rho_perp = std::hypot(dx, dy);

    // exit of the tracked region: everything is clipped to [0, t_bound]
    double t_bound = std::numeric_limits<double>::infinity();
    if (std::abs(dz) > 0.) t_bound = Boundary_HalfZ / std::abs(dz);
    if (rho_perp > 0.) t_bound = std::min(t_bound, Boundary_R / rho_perp);

    std::vector<Segment> hits;
    for (const auto& pc : Polycones) {
        double t_lo = 0.;
        double t_hi = t_bound;
        // z-bounds: z(t) = t * dz
        if (std::abs(dz) < 1e-12) {
            // a purely transverse ray only hits the volumes that span z = 0
            if (pc.z_min > 0. || pc.z_max < 0.) continue;
        } else {
            double t0 = pc.z_min / dz;
            double t1 = pc.z_max / dz;
            if (t0 > t1) std::swap(t0, t1);
            t_lo = std::max(t_lo, t0);
            t_hi = std::min(t_hi, t1);
        }
        if (t_hi <= t_lo) continue;
        // r-bounds: surface r(z) = offset + slope * z vs ray r(t) = rho_perp * t, z(t) = dz * t.
        // The condition r_ray >=/<= r_surface is linear in t: a * t >=/<= offset with a = rho_perp - slope * dz,
        // i.e. a half-line whose side depends on the sign of a. Returns false if the volume is missed.
        const auto clip_to_surface = [&](double r_at_zmin, double r_at_zmax, bool keep_above) -> bool {
            const double slope = (r_at_zmax - r_at_zmin) / (pc.z_max - pc.z_min);
            const double offset = r_at_zmin - slope * pc.z_min;
            const double a = rho_perp - slope * dz;
            if (std::abs(a) < 1e-15) return keep_above ? offset <= 0. : offset >= 0.;  // ray parallel to the surface: fully on one side
            const double t_cross = offset / a;
            if (keep_above == (a > 0.)) {
                t_lo = std::max(t_lo, t_cross);
            } else {
                t_hi = std::min(t_hi, t_cross);
            }
            return true;
        };
        if (!clip_to_surface(pc.r_inner_at_zmin, pc.r_inner_at_zmax, true)) continue;   // stay outside the inner surface
        if (!clip_to_surface(pc.r_outer_at_zmin, pc.r_outer_at_zmax, false)) continue;  // stay inside the outer surface
        if (t_hi <= t_lo + 1e-15) continue;
        hits.push_back({t_lo, t_hi, pc.material});
    }
    std::ranges::sort(hits, {}, &Segment::t_enter);

    // fill the gaps between the solids with air (the solids are mutually disjoint by construction)
    std::vector<Segment> segments;
    segments.reserve(2 * hits.size() + 1);
    double cursor = 0.;
    for (const auto& h : hits) {
        if (h.t_enter > cursor + 1e-12) segments.push_back({cursor, h.t_enter, EMaterial::kAir});
        segments.push_back(h);
        cursor = std::max(cursor, h.t_exit);
    }
    if (cursor < t_bound) segments.push_back({cursor, t_bound, EMaterial::kAir});
    return segments;
}

struct InteractionPoint {
    ROOT::Math::XYZPoint pos;
    EMaterial material;
};

// An interaction point along a ray from the origin in direction `dir`: each segment is weighted by
// `density * path_length` (thin-target approximation), then the point is placed uniformly within the chosen segment.
template <std::uniform_random_bit_generator G>
[[nodiscard]] InteractionPoint SampleInteractionPoint(G& rng, const ROOT::Math::XYZVector& dir) {

    const std::vector<Segment> segments = TraceRay(dir);
    if (segments.empty()) return {{0., 0., 0.}, EMaterial::kAir};  // defensive: cannot happen for a nonzero direction

    std::uniform_real_distribution<double> uniform{0., 1.};

    // `|dir|` cancels in the ratio of weights, so `delta t` stands for the path length
    const auto weight = [](const Segment& s) { return MaterialOf(s.material).density * (s.t_exit - s.t_enter); };
    double total_weight = 0.;
    for (const auto& s : segments) total_weight += weight(s);

    // weighted selection (fallback: last segment)
    double roll = uniform(rng) * total_weight;
    std::size_t selected = segments.size() - 1;
    for (std::size_t i = 0; i < segments.size(); ++i) {
        roll -= weight(segments[i]);
        if (roll <= 0.) {
            selected = i;
            break;
        }
    }

    // uniform `t` within the selected segment
    const Segment& seg = segments[selected];
    const double t = seg.t_enter + uniform(rng) * (seg.t_exit - seg.t_enter);
    return {{t * dir.X(), t * dir.Y(), t * dir.Z()}, seg.material};
}

}  // namespace FastSim::Geometry
