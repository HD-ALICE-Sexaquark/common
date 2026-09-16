#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <optional>
#include <tuple>
#include <utility>

#include <TMath.h>

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

// Signed flight length of a line particle w.r.t. a reference point: `(pos - ref) . p_hat`, i.e. the component of the
// displacement along the momentum. Together with `FastDCA_LineVertex` (the transverse component) it decomposes
// `|pos - ref|` orthogonally: `L^2 = L_par^2 + DCA^2`, and `CPA = L_par / L`. Negative when the particle "decayed
// behind" its origin.
inline double SignedFlightLength(const RMath::XYZVector& mom, const RMath::XYZPoint& pos, const RMath::XYZPoint& ref) {
    return (pos - ref).Dot(mom.Unit());
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

// Squared 2D radius of the middle-point between two points; the cheap `radius 2d` gate between seed and fit.
inline double Midpoint_SquaredRadius2D(const std::array<double, 3>& xyz1, const std::array<double, 3>& xyz2) {
    const double mid_x = 0.5 * (xyz1[0] + xyz2[0]);
    const double mid_y = 0.5 * (xyz1[1] + xyz2[1]);
    return mid_x * mid_x + mid_y * mid_y;
}

// Division guarded against a positively-defined denominator = 0.
inline double DivByErr(double value, double err) { return err > 0. ? value / err : Common::DummyDouble; }

// Momentum-sharing asymmetry `(a - b) / (a + b)`, guarded against `a + b = 0`.
inline double Asymmetry(double a, double b) { return (a + b) > Common::AbsAlmostZero ? (a - b) / (a + b) : Common::DummyDouble; }

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

// Error of a displacement `d = a - b` projected along the unit vector `n`: var = n' (C_a + C_b) n, with `a` and `b`
// taken as independent. Both covariances are packed lower-triangular with the position block first
// (`ESigma::X2 .. Z2`), so a 7x7 or 8x8 packed `CovMatrix` can be passed as-is.
// - `n = d / |d|` gives the error of `|d|` (`DistanceErr`)
// - `n = p_hat` gives the error of the signed flight length `d . p_hat` (`SignedFlightLength`); the error of `p_hat`
//   itself enters only as `DCA * sigma_theta`, negligible for a pointing particle, and is not included
//
// NOTE: when `b` is a vertex that was fitted from `a`'s particle, the two are correlated and this is only the
//       first-order estimate. For a line leg the correlation is suppressed along `n`: the PCA of a line does not
//       depend on where along the line its base point sits, so the vertex's position along the leg's own flight
//       direction is set by the other leg. Treat the result as approximate, not as a pull.
template <std::size_t N1, std::size_t N2>
    requires(N1 >= 6 && N2 >= 6)
inline std::optional<double> ProjectedErr(const RMath::XYZVector& n, const std::array<float, N1>& cov_a, const std::array<float, N2>& cov_b) {
    const auto quad = [&n](const auto& c) {  // n' C n, C read from the packed layout
        const auto at = [&c](ESigma ij) { return static_cast<double>(c[ij]); };
        return n.X() * n.X() * at(X2) + n.Y() * n.Y() * at(Y2) + n.Z() * n.Z() * at(Z2) +
               2. * (n.X() * n.Y() * at(XY) + n.X() * n.Z() * at(XZ) + n.Y() * n.Z() * at(YZ));
    };
    const double var = quad(cov_a) + quad(cov_b);
    if (var <= 0.) return std::nullopt;  // protection
    return std::sqrt(var);
}

// Error of the flight length `L = |d|`, `d = dv - ref`, i.e. `ProjectedErr` along `d` itself.
template <std::size_t N1, std::size_t N2>
    requires(N1 >= 6 && N2 >= 6)
inline std::optional<double> DistanceErr(const RMath::XYZVector& d, const std::array<float, N1>& cov_dv, const std::array<float, N2>& cov_ref) {
    if (d.Mag2() < Common::AbsAlmostZero) return std::nullopt;  // protection
    return ProjectedErr(d.Unit(), cov_dv, cov_ref);
}

// The position block (`ESigma::X2 .. Z2`) of a packed lower-triangular covariance: the first six entries.
template <std::size_t N>
    requires(N >= 6)
inline std::array<float, Common::NCovMatrixComponents_State3> PositionCov(const std::array<float, N>& cov) {
    return {cov[X2], cov[XY], cov[Y2], cov[XZ], cov[YZ], cov[Z2]};
}

// `d' (C_a + C_b)^-1 d` for a 3-vector `d = a - b` and the two packed lower-triangular position covariances of its
// end points (`ESigma::X2 .. Z2`, position block first, so any packed `CovMatrix` can be passed as-is): the chi2 of
// "the two points coincide", 3 d.o.f. `a` and `b` are taken as independent. Closed-form symmetric 3x3 inverse via
// cofactors; `std::nullopt` when the summed covariance is not positive definite.
// NOTE: covariances are in cm^2, so `det` is in cm^6 and any absolute threshold is wrong here -- a PV with
//       sigma ~ 10 um has `det ~ 1e-18`. Positive-definiteness is the only meaningful test.
template <std::size_t N1, std::size_t N2>
    requires(N1 >= 6 && N2 >= 6)
inline std::optional<double> Chi2_PointCoincidence(const RMath::XYZVector& d, const std::array<float, N1>& cov_a,
                                                   const std::array<float, N2>& cov_b) {
    const auto at = [&](ESigma ij) { return static_cast<double>(cov_a[ij]) + static_cast<double>(cov_b[ij]); };
    const double sxx = at(X2), sxy = at(XY), syy = at(Y2), sxz = at(XZ), syz = at(YZ), szz = at(Z2);
    // signed cofactors of the symmetric 3x3
    const double cxx = syy * szz - syz * syz;
    const double cxy = sxz * syz - sxy * szz;
    const double cxz = sxy * syz - sxz * syy;
    const double cyy = sxx * szz - sxz * sxz;
    const double cyz = sxy * sxz - sxx * syz;
    const double czz = sxx * syy - sxy * sxy;
    const double det = sxx * cxx + sxy * cxy + sxz * cxz;
    if (det <= 0.) return std::nullopt;  // protection: not positive definite
    const double x = d.X(), y = d.Y(), z = d.Z();
    const double quad = x * x * cxx + y * y * cyy + z * z * czz + 2. * (x * y * cxy + x * z * cxz + y * z * cyz);
    if (quad < 0.) return std::nullopt;  // protection: rounding on a near-singular sum
    return quad / det;
}

inline RMath::PxPyPzEVector CreateLorentzVector(double px, double py, double pz, double mass) {
    return {px, py, pz, std::sqrt(px * px + py * py + pz * pz + mass * mass)};
}

inline double InvariantMass(double px1, double py1, double pz1, double m1,  //
                            double px2, double py2, double pz2, double m2) {
    return (CreateLorentzVector(px1, py1, pz1, m1) + CreateLorentzVector(px2, py2, pz2, m2)).M();
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

// == Struck nucleon subtraction (antisexaquark + nucleon -> products) == //

// The reaction products sum to `P_tot = P_S + P_N`, with the struck nucleon carrying an unknown Fermi momentum
// (`|p_N| <~ 250 MeV/c`). Two estimators of the antisexaquark 4-vector:
// - `NucleonAtRest`: `p_N = 0`, `E_N = m_N`.
// - `NucleonAlongAxis`: the antisexaquark flew from the primary to the secondary vertex, so its direction is the
//   flight axis `a_hat` and the transverse part of `P_tot` is entirely the nucleon's: `p_N = P_tot - (P_tot . a) a`,
//   `E_N = sqrt(m_N^2 + |p_N|^2)`; only the longitudinal Fermi component is still assumed zero.
// Neither fixes the mass: the dominant smearing is `-2 |p_S| p_N_par` (only the longitudinal Fermi component enters
// `p_S . p_N`), ~150 MeV at `p_S ~ 2 GeV/c`, present in both. The axis version removes the `O(p_N^2)` terms only
// (~20-30 MeV). What the axis buys is elsewhere: `Qt = |P_tot x a_hat| = |p_N_perp|` is measured, bounded by the
// Fermi momentum for signal and unbounded for background.
inline RMath::PxPyPzEVector NucleonAtRest(const RMath::PxPyPzEVector& lv_tot, double m_nucleon) {
    return lv_tot - RMath::PxPyPzEVector{0., 0., 0., m_nucleon};
}

inline RMath::PxPyPzEVector NucleonAlongAxis(const RMath::PxPyPzEVector& lv_tot, const RMath::XYZVector& axis, double m_nucleon) {
    const double ql = lv_tot.Vect().Dot(axis);  // `axis` is a unit vector
    const RMath::XYZVector p_s = ql * axis;
    const double qt2 = (lv_tot.Vect() - p_s).Mag2();
    return {p_s.X(), p_s.Y(), p_s.Z(), lv_tot.E() - std::sqrt(m_nucleon * m_nucleon + qt2)};
}

// Under an antisexaquark mass hypothesis `m_s` and with the flight axis fixing the nucleon's transverse momentum, the
// nucleon's longitudinal momentum `x` is the root of
//     E_tot - sqrt(m_N^2 + Qt^2 + x^2) = sqrt(m_s^2 + (Ql - x)^2)
// which squares to `4 (E_tot^2 - Ql^2) x^2 - 4 B Ql x + 4 E_tot^2 (m_N^2 + Qt^2) - B^2 = 0`,
// `B = E_tot^2 + m_N^2 + Qt^2 - m_s^2 - Ql^2`. Return `|p_N| = sqrt(Qt^2 + x^2)` for the smaller-|x| physical root:
// the nucleon momentum this candidate needs to be an antisexaquark of mass `m_s`. For signal it follows the Fermi
// distribution, for background it is arbitrary; it is a mass window that scales with `p_S` (the Fermi smearing of
// `M` does), unlike a fixed MeV window. `Dummy` when no real root exists.
inline double RequiredNucleonMomentum(const RMath::PxPyPzEVector& lv_tot, const RMath::XYZVector& axis, double m_nucleon, double m_s) {
    const double e_tot = lv_tot.E();
    const double ql = lv_tot.Vect().Dot(axis);
    const double qt2 = lv_tot.Vect().Mag2() - ql * ql;
    const double b = e_tot * e_tot + m_nucleon * m_nucleon + qt2 - m_s * m_s - ql * ql;
    const double qa = 4. * (e_tot * e_tot - ql * ql);
    const double qb = -4. * b * ql;
    const double qc = 4. * e_tot * e_tot * (m_nucleon * m_nucleon + qt2) - b * b;
    const double disc = qb * qb - 4. * qa * qc;
    if (qa < Common::AbsAlmostZero || disc < 0.) return Common::DummyDouble;
    const double sq = std::sqrt(disc);
    // -- both roots; keep those that survive the two un-squarings (`0 < E_N <= E_tot`), then the smaller |x|
    double best = Common::DummyDouble;
    for (const double x : {(-qb - sq) / (2. * qa), (-qb + sq) / (2. * qa)}) {
        const double e_n = (b + 2. * ql * x) / (2. * e_tot);
        if (e_n <= 0. || e_n > e_tot) continue;
        if (best == Common::DummyDouble || std::abs(x) < std::abs(best)) best = x;
    }
    if (best == Common::DummyDouble) return Common::DummyDouble;
    return std::sqrt(qt2 + best * best);
}

// helicity angle: daughter's direction in the mother's rest frame vs. the mother's lab direction
inline double CosThetaStar(const RMath::PxPyPzEVector& mother, const RMath::PxPyPzEVector& dau) {
    return RMath::Boost(mother.BoostToCM())(dau).Vect().Unit().Dot(mother.Vect().Unit());
}

// p-value of `chi2` with `ndf` degrees of freedom; `Dummy` on unphysical input.
inline double Chi2Prob(double chi2, int ndf) {  //
    return (chi2 >= 0. && ndf > 0) ? TMath::Prob(chi2, ndf) : Common::DummyDouble;
}

}  // namespace Common::Math
