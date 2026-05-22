#pragma once

#include <tuple>
#include <utility>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/VectorUtil.h>

#include "Constants.hpp"

namespace Common::Math {

// Based on https://stackoverflow.com/a/64247207
template <class S>
inline std::pair<S, S> sincos(S arg) {
    return {std::sin(arg), std::cos(arg)};
}

inline double CosinePointingAngle(const ROOT::Math::XYZVector& mom_v0, const ROOT::Math::XYZPoint& pos_v0, const ROOT::Math::XYZPoint& pos_ref) {
    return std::cos(ROOT::Math::VectorUtil::Angle(mom_v0, pos_v0 - pos_ref));
}

inline double ArmenterosQt(const ROOT::Math::XYZVector& mom_v0, const ROOT::Math::XYZVector& mom_dau) {
    return ROOT::Math::VectorUtil::Perp(mom_dau, mom_v0);
}

inline double ArmenterosAlpha(const ROOT::Math::XYZVector& mom_v0, const ROOT::Math::XYZVector& mom_neg, const ROOT::Math::XYZVector& mom_pos) {
    double lQlNeg = mom_neg.Dot(mom_v0) / mom_v0.R();
    double lQlPos = mom_pos.Dot(mom_v0) / mom_v0.R();
    if (std::abs(lQlPos + lQlNeg) < Common::AbsAlmostZero) return 2.;  // protection
    return (lQlPos - lQlNeg) / (lQlPos + lQlNeg);
}

inline ROOT::Math::XYZPoint FastPCA_LineVertex(const ROOT::Math::XYZVector& mom_v0, const ROOT::Math::XYZPoint& pos_v0,
                                               const ROOT::Math::XYZPoint& pos_ref) {
    ROOT::Math::XYZVector delta = pos_ref - pos_v0;
    double t = delta.Dot(mom_v0) / mom_v0.Mag2();
    return pos_v0 + t * mom_v0;
}

static ROOT::Math::XYZPoint FastPCA_HelixVertex(const ROOT::Math::XYZVector& mom, const ROOT::Math::XYZPoint& xyz, int charge,
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

}  // namespace Common::Math
