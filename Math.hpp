#pragma once

#include <cmath>
#include <optional>
#include <tuple>
#include <utility>

#include <Math/GenVector/VectorUtil.h>
#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Constants.hpp"

namespace Common::Math {

// Fused call of `std::sin` and `std::cos`.
// Based on https://stackoverflow.com/a/64247207
template <class S>
inline std::pair<S, S> sincos(S arg) {
    return {std::sin(arg), std::cos(arg)};
}

// Calculate the cosine of the pointing angle of a particle with momentum Px,Py,Pz and vertex X,Y,Z w.r.t. to a reference point.
inline double CosinePointingAngle(const ROOT::Math::XYZVector& mom_v0, const ROOT::Math::XYZPoint& pos_v0, const ROOT::Math::XYZPoint& pos_ref) {
    return ROOT::Math::VectorUtil::CosTheta(mom_v0, pos_v0 - pos_ref);
}

// Overload of `CosinePointingAngle(...)`, using scalars instead of vectors.
inline double CosinePointingAngle(double v0_px, double v0_py, double v0_pz,  //
                                  double v0_x, double v0_y, double v0_z,     //
                                  double ref_x, double ref_y, double ref_z) {
    return CosinePointingAngle({v0_px, v0_py, v0_pz}, {v0_x, v0_y, v0_z}, {ref_x, ref_y, ref_z});
}

// Calculate Armenteros-Podolanski qT.
// Based on https://github.com/alisw/AliRoot (`STEER/ESD/AliESDv0::PtArmV0()`)
inline double ArmenterosQt(const ROOT::Math::XYZVector& mom_v0, const ROOT::Math::XYZVector& mom_dau) {
    return ROOT::Math::VectorUtil::Perp(mom_dau, mom_v0);
}

// Overload of `ArmenterosQt(...)` using scalars instead of vectors.
inline double ArmenterosQt(double v0_px, double v0_py, double v0_pz,  //
                           double dau_px, double dau_py, double dau_pz) {
    return ArmenterosQt({v0_px, v0_py, v0_pz}, {dau_px, dau_py, dau_pz});
}

// Calculate Armenteros-Podolanski alpha.
// Based on https://github.com/alisw/AliRoot (`STEER/ESD/AliESDv0::AlphaV0()`)
inline std::optional<double> ArmenterosAlpha(const ROOT::Math::XYZVector& mom_v0, const ROOT::Math::XYZVector& mom_neg,
                                             const ROOT::Math::XYZVector& mom_pos) {
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

inline ROOT::Math::XYZPoint FastPCA_LineVertex(const ROOT::Math::XYZVector& mom_v0, const ROOT::Math::XYZPoint& pos_v0,
                                               const ROOT::Math::XYZPoint& pos_ref) {
    ROOT::Math::XYZVector delta = pos_ref - pos_v0;
    double t = delta.Dot(mom_v0) / mom_v0.Mag2();
    return pos_v0 + t * mom_v0;
}

inline double FastDCA_LineVertex(const ROOT::Math::XYZVector& mom_v0, const ROOT::Math::XYZPoint& pos_v0, const ROOT::Math::XYZPoint& pos_ref) {
    return std::sqrt((FastPCA_LineVertex(mom_v0, pos_v0, pos_ref) - pos_ref).Mag2());
}

inline ROOT::Math::XYZPoint FastPCA_HelixVertex(const ROOT::Math::XYZVector& mom, const ROOT::Math::XYZPoint& xyz, int charge,
                                                const ROOT::Math::XYZPoint& ref, double bz) {

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

    ROOT::Math::XYZPoint pca{x0 + sB * px0 + cB * py0,  //
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
inline ROOT::Math::XYZPoint MiddlePoint(const ROOT::Math::XYZPoint& p1, const ROOT::Math::XYZPoint& p2) {
    return {(p1.X() + p2.X()) / 2., (p1.Y() + p2.Y()) / 2., (p1.Z() + p2.Z()) / 2.};
}

// Calculate square of the distance between two points in 3D space.
constexpr double SquaredDistance(const ROOT::Math::XYZPoint& xyz1, const ROOT::Math::XYZPoint& xyz2) { return (xyz1 - xyz2).Mag2(); }

// Calculate square of the distance between two points in 3D space.
constexpr double SquaredDistance(const std::array<double, 3>& xyz1, const std::array<double, 3>& xyz2) {
    double dx = xyz2[0] - xyz1[0];
    double dy = xyz2[1] - xyz1[1];
    double dz = xyz2[2] - xyz1[2];
    return dx * dx + dy * dy + dz * dz;
}

constexpr double Distance(const ROOT::Math::XYZPoint& xyz1, const ROOT::Math::XYZPoint& xyz2) { return std::sqrt(SquaredDistance(xyz1, xyz2)); }
constexpr double Distance(const std::array<double, 3>& xyz1, const std::array<double, 3>& xyz2) { return std::sqrt(SquaredDistance(xyz1, xyz2)); }

inline ROOT::Math::PxPyPzEVector CreateLorentzVector(double px, double py, double pz, double mass) {
    return {px, py, pz, std::sqrt(px * px + py * py + pz * pz + mass * mass)};
}

inline double Hypot4(double a, double b, double c, double d) { return std::sqrt(a * a + b * b + c * c + d * d); }

}  // namespace Common::Math
