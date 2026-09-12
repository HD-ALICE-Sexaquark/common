#pragma once

#include <algorithm>
#include <cmath>
#include <utility>

#include <Math/GenVector/Boost.h>
#include <Math/GenVector/VectorUtil.h>
#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Constants.hpp"
#include "Math.hpp"
#include "POD_V0.hpp"

namespace Common::PostProcessing {

namespace RMath = ROOT::Math;

// == Closure == //

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

// `ReattachTo(...)` applied straight to a `POD::V0`: the legs sum to the V0's own fit, so `new_mother` is that same
// V0 after a later fit's kick, e.g. `Fit_Lambda1_*` of a `POD::Hdibaryon`.
// Return legs as {neg, pos}.
inline std::pair<RMath::PxPyPzEVector, RMath::PxPyPzEVector> CloseDecay(const POD::V0& v0, double m_neg, double m_pos,
                                                                        const RMath::PxPyPzEVector& new_mother) {
    std::pair<RMath::PxPyPzEVector, RMath::PxPyPzEVector> legs{
        {v0.Fit_Neg_Px, v0.Fit_Neg_Py, v0.Fit_Neg_Pz, v0.Fit_Neg_Energy},
        {v0.Fit_Pos_Px, v0.Fit_Pos_Py, v0.Fit_Pos_Pz, v0.Fit_Pos_Energy},
    };
    ReattachTo(legs.first, legs.second, m_neg, m_pos, legs.first + legs.second, new_mother);
    return legs;
}

}  // namespace Common::PostProcessing
