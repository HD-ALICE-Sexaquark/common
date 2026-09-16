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

    // (anti)proton
    [[nodiscard]] double Seed_Proton_Pz() const { return is_anti_lambda ? seed.Dau1_Pz() : seed.Dau2_Pz(); }
    [[nodiscard]] double Seed_Proton_Pt() const { return is_anti_lambda ? seed.Dau1_Pt() : seed.Dau2_Pt(); }
    [[nodiscard]] double Seed_Proton_Eta() const { return is_anti_lambda ? seed.Dau1_Eta() : seed.Dau2_Eta(); }
    [[nodiscard]] double Fit_Proton_Pz() const { return is_anti_lambda ? fit.Dau1_Pz() : fit.Dau2_Pz(); }
    [[nodiscard]] double Fit_Proton_Pt() const { return is_anti_lambda ? fit.Dau1_Pt() : fit.Dau2_Pt(); }
    [[nodiscard]] double Fit_Proton_Eta() const { return is_anti_lambda ? fit.Dau1_Eta() : fit.Dau2_Eta(); }
    [[nodiscard]] double Proton_CosThetaStar() const { return proton_cos_theta_star; }  // helicity angle w.r.t. the (anti)lambda's lab direction

    // pi(plus/minus)
    [[nodiscard]] double Seed_Pion_Pz() const { return is_anti_lambda ? seed.Dau2_Pz() : seed.Dau1_Pz(); }
    [[nodiscard]] double Seed_Pion_Pt() const { return is_anti_lambda ? seed.Dau2_Pt() : seed.Dau1_Pt(); }
    [[nodiscard]] double Seed_Pion_Eta() const { return is_anti_lambda ? seed.Dau2_Eta() : seed.Dau1_Eta(); }
    [[nodiscard]] double Fit_Pion_Pz() const { return is_anti_lambda ? fit.Dau2_Pz() : fit.Dau1_Pz(); }
    [[nodiscard]] double Fit_Pion_Pt() const { return is_anti_lambda ? fit.Dau2_Pt() : fit.Dau1_Pt(); }
    [[nodiscard]] double Fit_Pion_Eta() const { return is_anti_lambda ? fit.Dau2_Eta() : fit.Dau1_Eta(); }

   private:
    double proton_cos_theta_star;
    bool is_anti_lambda;
};

}  // namespace Cached
