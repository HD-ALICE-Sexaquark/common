#pragma once

#include <array>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Cached_Sexaquark.hpp"
#include "Cached_Unit.hpp"
#include "Constants.hpp"
#include "DB_ReactionChannels.hpp"
#include "Math.hpp"
#include "POD_Sexaquark.hpp"
#include "POD_V0.hpp"

namespace Cached {

// Channel A: antisexaquark + neutron -> (anti)lambda + kaon zero short
struct ChannelA : Sexaquark {

    ChannelA(const POD::Sexaquark& sexa, const POD::V0& v0a, const POD::V0& v0b, const ROOT::Math::XYZPoint& ref,
             const std::array<float, Common::NCovMatrixComponents_State3>& ref_cov)
        : Sexaquark(sexa, DB::ReactionChannels::StruckNucleonMass('A'), ref, ref_cov),
          fit_v0a{v0a.Fit_Neg_Px, v0a.Fit_Neg_Py, v0a.Fit_Neg_Pz, v0a.Fit_Neg_Energy,  //
                  v0a.Fit_Pos_Px, v0a.Fit_Pos_Py, v0a.Fit_Pos_Pz, v0a.Fit_Pos_Energy,  //
                  v0a.Fit_X,      v0a.Fit_Y,      v0a.Fit_Z,      Common::Math::PositionCov(v0a.CovMatrix)},
          fit_v0b{v0b.Fit_Neg_Px,     v0b.Fit_Neg_Py, v0b.Fit_Neg_Pz,
                  v0b.Fit_Neg_Energy,  //
                  v0b.Fit_Pos_Px,     v0b.Fit_Pos_Py, v0b.Fit_Pos_Pz,
                  v0b.Fit_Pos_Energy,  //
                  v0b.Fit_X,          v0b.Fit_Y,      v0b.Fit_Z,      Common::Math::PositionCov(v0b.CovMatrix)} {}

    // (anti)lambda's fit vs antisexaquark fit //
    [[nodiscard]] double V0A_FlightLength_WithRespectToSV() const { return fit_v0a.FlightLength_WithRespectTo(fit.vtx); }
    [[nodiscard]] double V0A_FlightLength_DivByErr_WithRespectToSV() const {
        return fit_v0a.FlightLength_DivByErr_WithRespectTo(fit.vtx, fit.cov_vtx);
    }
    [[nodiscard]] double V0A_DCA_WithRespectToSV() const { return fit_v0a.DCA_WithRespectTo(fit.vtx); }
    [[nodiscard]] double V0A_CPA_WithRespectToSV() const { return fit_v0a.CPA_WithRespectTo(fit.vtx); }

    // kaon zero short's fit vs antisexaquark fit //
    [[nodiscard]] double V0B_FlightLength_WithRespectToSV() const { return fit_v0b.FlightLength_WithRespectTo(fit.vtx); }
    [[nodiscard]] double V0B_FlightLength_DivByErr_WithRespectToSV() const {
        return fit_v0b.FlightLength_DivByErr_WithRespectTo(fit.vtx, fit.cov_vtx);
    }
    [[nodiscard]] double V0B_DCA_WithRespectToSV() const { return fit_v0b.DCA_WithRespectTo(fit.vtx); }
    [[nodiscard]] double V0B_CPA_WithRespectToSV() const { return fit_v0b.CPA_WithRespectTo(fit.vtx); }

    // (anti)lambda's fit vs kaon zero short's fit ~ duplicate test //
    [[nodiscard]] double Distance_BetweenV0sDVs() const { return fit_v0a.DistanceFrom(fit_v0b.vtx); }

   private:
    Unit::PostFit fit_v0a;  // the (anti)lambda as a leg of the closed tree
    Unit::PostFit fit_v0b;  // the kaon zero short as a leg of the closed tree
};

}  // namespace Cached
