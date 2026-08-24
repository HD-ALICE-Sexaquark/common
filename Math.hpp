#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <optional>
#include <tuple>
#include <utility>

#include <Math/GenVector/Boost.h>
#include <Math/GenVector/VectorUtil.h>
#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Constants.hpp"

namespace Common::Math {

namespace RMath = ROOT::Math;

// Fused call of `std::sin` and `std::cos`.
// Based on https://stackoverflow.com/a/64247207
template <class S>
inline std::pair<S, S> sincos(S arg) {
    return {std::sin(arg), std::cos(arg)};
}

// Calculate the cosine of the pointing angle of a particle with momentum Px,Py,Pz and vertex X,Y,Z w.r.t. to a reference point.
inline double CosinePointingAngle(const RMath::XYZVector& mom_v0, const RMath::XYZPoint& pos_v0, const RMath::XYZPoint& pos_ref) {
    return RMath::VectorUtil::CosTheta(mom_v0, pos_v0 - pos_ref);
}

// Overload of `CosinePointingAngle(...)`, using scalars instead of vectors.
inline double CosinePointingAngle(double v0_px, double v0_py, double v0_pz,  //
                                  double v0_x, double v0_y, double v0_z,     //
                                  double ref_x, double ref_y, double ref_z) {
    return CosinePointingAngle({v0_px, v0_py, v0_pz}, {v0_x, v0_y, v0_z}, {ref_x, ref_y, ref_z});
}

// Calculate Armenteros-Podolanski qT.
// Based on https://github.com/alisw/AliRoot (`STEER/ESD/AliESDv0::PtArmV0()`)
inline double ArmenterosQt(const RMath::XYZVector& mom_v0, const RMath::XYZVector& mom_dau) { return RMath::VectorUtil::Perp(mom_dau, mom_v0); }

// Overload of `ArmenterosQt(...)` using scalars instead of vectors.
inline double ArmenterosQt(double v0_px, double v0_py, double v0_pz,  //
                           double dau_px, double dau_py, double dau_pz) {
    return ArmenterosQt({v0_px, v0_py, v0_pz}, {dau_px, dau_py, dau_pz});
}

// Calculate Armenteros-Podolanski alpha.
// Based on https://github.com/alisw/AliRoot (`STEER/ESD/AliESDv0::AlphaV0()`)
inline std::optional<double> ArmenterosAlpha(const RMath::XYZVector& mom_v0, const RMath::XYZVector& mom_neg, const RMath::XYZVector& mom_pos) {
    double p_v0 = mom_v0.R();
    if (p_v0 < Common::AbsAlmostZero) return std::nullopt;
    double lQlNeg = mom_neg.Dot(mom_v0) / p_v0;
    double lQlPos = mom_pos.Dot(mom_v0) / p_v0;
    if (std::abs(lQlPos + lQlNeg) < Common::AbsAlmostZero) return std::nullopt;  // protection
    return (lQlPos - lQlNeg) / (lQlPos + lQlNeg);
}

// Overload of `ArmenterosAlpha()` using scalars instead of vectors.
inline std::optional<double> ArmenterosAlpha(double v0_px, double v0_py, double v0_pz,     //
                                             double neg_px, double neg_py, double neg_pz,  //
                                             double pos_px, double pos_py, double pos_pz) {
    return ArmenterosAlpha({v0_px, v0_py, v0_pz}, {neg_px, neg_py, neg_pz}, {pos_px, pos_py, pos_pz});
}

// normalised elliptic radius: == 1 exactly on the kinematic locus
// - mass arguments are PDG values
// - sA and sQ are scaling values
// recommended: veto photon conversions
// if (qt < 0.02 && std::abs(alpha) < 0.6) return kNone;
// usage:
// const bool onL  = std::abs(radius - 1.) < tolerance;
// const bool onAL = std::abs(radius - 1.) < tolerance;
// const bool onK  = std::abs(radius - 1.) < tolerance;
inline double ArmenterosEllipticRadius(double alpha, double qt, double M, double mass_neg, double mass_pos, double pV0, double sA = 1.,
                                       double sQ = 1.) {
    auto p_star = [](double mass, double m1, double m2) -> double {
        const double s1 = mass * mass - (m1 + m2) * (m1 + m2);
        const double s2 = mass * mass - (m1 - m2) * (m1 - m2);
        return (s1 > 0.) ? std::sqrt(s1 * s2) / (2. * mass) : 0.;
    };
    const double beta = (pV0 > 0.) ? pV0 / std::sqrt(pV0 * pV0 + M * M) : 1.;
    const double ps = p_star(M, mass_pos, mass_neg);
    const double alpha_0 = (mass_pos * mass_pos - mass_neg * mass_neg) / (M * M);
    const double alpha_semi = 2. * ps / (beta * M);
    const double dx = (alpha - alpha_0) / (alpha_semi * sA);
    const double dy = qt / (ps * sQ);
    return std::sqrt(dx * dx + dy * dy);
}

inline RMath::XYZPoint FastPCA_LineVertex(const RMath::XYZVector& mom_v0, const RMath::XYZPoint& pos_v0, const RMath::XYZPoint& pos_ref) {
    RMath::XYZVector delta = pos_ref - pos_v0;
    double t = delta.Dot(mom_v0) / mom_v0.Mag2();
    return pos_v0 + t * mom_v0;
}

inline double FastDCA_LineVertex(const RMath::XYZVector& mom_v0, const RMath::XYZPoint& pos_v0, const RMath::XYZPoint& pos_ref) {
    return std::sqrt((FastPCA_LineVertex(mom_v0, pos_v0, pos_ref) - pos_ref).Mag2());
}

inline RMath::XYZPoint FastPCA_HelixVertex(const RMath::XYZVector& mom, const RMath::XYZPoint& xyz, int charge, const RMath::XYZPoint& ref,
                                           double bz) {

    double bq = bz * static_cast<double>(charge) * Common::Kappa;

    double px0 = mom.X();
    double py0 = mom.Y();
    double pz0 = mom.Z();
    double pt2 = px0 * px0 + py0 * py0;
    double x0 = xyz.X();
    double y0 = xyz.Y();
    double z0 = xyz.Z();

    double dx = ref.X() - x0;
    double dy = ref.Y() - y0;
    double dz = ref.Z() - z0;
    double a = dx * px0 + dy * py0;

    double abq = bq * a;
    double bbq = bq * (dx * py0 - dy * px0) - pt2;

    // 1.a -- get solution and update cache properties //

    double theta = std::atan2(abq, -bbq);
    auto [sin, cos] = Math::sincos(theta);
    double sB = sin / bq;
    double cB = (1. - cos) / bq;

    double ds = theta / bq;

    RMath::XYZPoint pca{x0 + sB * px0 + cB * py0,  //
                        y0 - cB * px0 + sB * py0,  //
                        z0 + ds * pz0};

    // 2 -- add z-component as small correction //

    double cbq = bbq * cos - abq * sin - pz0 * pz0;
    if (std::abs(cbq) < Common::AbsAlmostZero) return pca;  // protection

    double sz = (ds * pz0 - dz) * pz0 / cbq;

    // 2.b -- update ds //

    ds += sz;

    // 2.c -- update rest of cache properties //

    theta = bq * ds;
    std::tie(sin, cos) = Math::sincos(theta);
    sB = sin / bq;
    cB = (1. - cos) / bq;

    pca.SetXYZ(x0 + sB * px0 + cB * py0,  //
               y0 - cB * px0 + sB * py0,  //
               z0 + ds * pz0);

    return pca;
}

// Calculate the middle-point between two points in 3D space.
inline RMath::XYZPoint MiddlePoint(const RMath::XYZPoint& p1, const RMath::XYZPoint& p2) {
    return {(p1.X() + p2.X()) / 2., (p1.Y() + p2.Y()) / 2., (p1.Z() + p2.Z()) / 2.};
}

// Calculate square of the distance between two points in 3D space.
inline double SquaredDistance(const RMath::XYZPoint& xyz1, const RMath::XYZPoint& xyz2) { return (xyz1 - xyz2).Mag2(); }

// Calculate square of the distance between two points in 3D space.
inline double SquaredDistance(const std::array<double, 3>& xyz1, const std::array<double, 3>& xyz2) {
    double dx = xyz2[0] - xyz1[0];
    double dy = xyz2[1] - xyz1[1];
    double dz = xyz2[2] - xyz1[2];
    return dx * dx + dy * dy + dz * dz;
}
inline double SquaredDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dz = z2 - z1;
    return dx * dx + dy * dy + dz * dz;
}

inline double Distance(const RMath::XYZPoint& xyz1, const RMath::XYZPoint& xyz2) { return std::sqrt(SquaredDistance(xyz1, xyz2)); }
inline double Distance(const std::array<double, 3>& xyz1, const std::array<double, 3>& xyz2) { return std::sqrt(SquaredDistance(xyz1, xyz2)); }

inline double Distance(double x1, double y1, double z1, double x2, double y2, double z2) {
    return std::sqrt(SquaredDistance(x1, y1, z1, x2, y2, z2));
}

inline RMath::PxPyPzEVector CreateLorentzVector(double px, double py, double pz, double mass) {
    return {px, py, pz, std::sqrt(px * px + py * py + pz * pz + mass * mass)};
}

inline double Hypot4(double a, double b, double c, double d) { return std::sqrt(a * a + b * b + c * c + d * d); }

// Tracking can split one physical particle into two ESD entries.
// To be used with raw kinematic information (i.e., not from fits).
inline bool IsSameHelix_Impl(const RMath::XYZVector& p_a, const RMath::XYZVector& p_b,  //
                             double max_delta_r, double max_rel_delta_p) {
    const double p_ref = p_a.R();
    if (p_ref < Common::AbsAlmostZero || std::abs(p_ref - p_b.R()) >= max_rel_delta_p * p_ref) return false;  // early return; small performance boost
    return RMath::VectorUtil::DeltaR(p_a, p_b) <= max_delta_r;
}
inline bool IsSameHelix(const std::array<float, 6>& state_a, const std::array<float, 6>& state_b,  //
                        double max_delta_r, double max_rel_delta_p) {
    return IsSameHelix_Impl(RMath::XYZVector{static_cast<double>(state_a[3]), static_cast<double>(state_a[4]), static_cast<double>(state_a[5])},
                            RMath::XYZVector{static_cast<double>(state_b[3]), static_cast<double>(state_b[4]), static_cast<double>(state_b[5])},
                            max_delta_r, max_rel_delta_p);
}
inline bool IsSameHelix(double px_a, double py_a, double pz_a,  //
                        double px_b, double py_b, double pz_b,  //
                        double max_delta_r, double max_rel_delta_p) {
    return IsSameHelix_Impl(RMath::XYZVector{px_a, py_a, pz_a}, RMath::XYZVector{px_b, py_b, pz_b}, max_delta_r, max_rel_delta_p);
}

// Rebuild a two-body decay against a new mother: keep the daughters' directions as measured in the old mother's
// rest frame (where they are exactly back-to-back, since old_mother == d1 + d2), then set the momentum to what the
// new mother's mass demands. Both daughters land exactly on their shells and their sum is exactly `new_mother`,
// for any change of mother mass.
inline void ReattachTo(RMath::PxPyPzEVector& d1, RMath::PxPyPzEVector& d2, double m1, double m2, const RMath::PxPyPzEVector& old_mother,
                       const RMath::PxPyPzEVector& new_mother) {
    const double mm = new_mother.M();
    const double p_star = std::sqrt(std::max(0., (mm * mm - (m1 + m2) * (m1 + m2)) * (mm * mm - (m1 - m2) * (m1 - m2)))) / (2. * mm);
    const auto u = RMath::Boost(old_mother.BoostToCM())(d1).Vect().Unit();
    const RMath::Boost back(-new_mother.BoostToCM());
    d1 = back(RMath::PxPyPzEVector(p_star * u.X(), p_star * u.Y(), p_star * u.Z(), std::hypot(p_star, m1)));
    d2 = back(RMath::PxPyPzEVector(-p_star * u.X(), -p_star * u.Y(), -p_star * u.Z(), std::hypot(p_star, m2)));
}

// `ReattachTo(...)` applied straight to a V0-like POD -- anything carrying `Neg_Fit_*`, `Pos_Fit_*` and its own
// (Px,Py,Pz,Energy), i.e. `POD::V0` and `POD::Extended::PreFoundLambda`.
// Return legs as {neg, pos}.
template <typename V0Like>
inline std::pair<RMath::PxPyPzEVector, RMath::PxPyPzEVector> CloseDecay(const V0Like& v0, double m_neg, double m_pos,
                                                                        const RMath::PxPyPzEVector& new_mother) {
    std::pair<RMath::PxPyPzEVector, RMath::PxPyPzEVector> legs{
        {v0.Neg_Fit_Px, v0.Neg_Fit_Py, v0.Neg_Fit_Pz, v0.Neg_Fit_Energy},
        {v0.Pos_Fit_Px, v0.Pos_Fit_Py, v0.Pos_Fit_Pz, v0.Pos_Fit_Energy},
    };
    ReattachTo(legs.first, legs.second, m_neg, m_pos, {v0.Px, v0.Py, v0.Pz, v0.Energy}, new_mother);
    return legs;
}

}  // namespace Common::Math
