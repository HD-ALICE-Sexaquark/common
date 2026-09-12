#pragma once

#include <array>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Cached_V0.hpp"
#include "Constants.hpp"
#include "Math.hpp"
#include "POD_V0.hpp"

namespace Cached {

// Similar to `Cached::V0`, but this one contains:
// - spin-correlations info
// - pid-hypothesis-based properties besides neg/pos
struct Lambda : Cached::V0 {

    // NOTE: mass(lambda) = mass(antilambda)
    Lambda(const POD::V0& lambda, bool anti_lambda, const ROOT::Math::XYZPoint& ref,
           const std::array<float, Common::NCovMatrixComponents_State3>& ref_cov)
        : Cached::V0(lambda, ref, ref_cov),
          proton_cos_theta_star{Common::Math::CosThetaStar(fit.lv, anti_lambda ? fit.lv_dau1 : fit.lv_dau2)},
          is_anti_lambda{anti_lambda} {}

    [[nodiscard]] bool IsAntiLambda() const { return is_anti_lambda; }

    // (anti)proton: the negative leg of an antilambda, the positive one of a lambda
    [[nodiscard]] double Proton_Fit_Pt() const { return is_anti_lambda ? fit.Dau1_Pt() : fit.Dau2_Pt(); }
    [[nodiscard]] double Proton_Fit_Pz() const { return is_anti_lambda ? fit.Dau1_Pz() : fit.Dau2_Pz(); }
    [[nodiscard]] double Proton_CosThetaStar() const { return proton_cos_theta_star; }  // helicity angle w.r.t. the (anti)lambda's lab direction

    // pi(plus/minus): the other leg
    [[nodiscard]] double Pion_Fit_Pt() const { return is_anti_lambda ? fit.Dau2_Pt() : fit.Dau1_Pt(); }
    [[nodiscard]] double Pion_Fit_Pz() const { return is_anti_lambda ? fit.Dau2_Pz() : fit.Dau1_Pz(); }

   private:
    double proton_cos_theta_star;
    bool is_anti_lambda;
};

}  // namespace Cached
