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

// Channel D: antisexaquark + proton -> (anti)lambda + charged kaon
struct ChannelD : Sexaquark {

    ChannelD(const POD::Sexaquark& sexa, const POD::V0& v0, const ROOT::Math::XYZPoint& ref,
             const std::array<float, Common::NCovMatrixComponents_State3>& ref_cov)
        : Sexaquark(sexa, DB::ReactionChannels::StruckNucleonMass('D'), ref, ref_cov),
          fit_v0{v0.Fit_Neg_Px, v0.Fit_Neg_Py, v0.Fit_Neg_Pz, v0.Fit_Neg_Energy,  //
                 v0.Fit_Pos_Px, v0.Fit_Pos_Py, v0.Fit_Pos_Pz, v0.Fit_Pos_Energy,  //
                 v0.Fit_X,      v0.Fit_Y,      v0.Fit_Z,      Common::Math::PositionCov(v0.CovMatrix)} {}

    // (anti)lambda's fit vs antisexaquark fit //
    [[nodiscard]] double V0_FlightLength_WithRespectToSV() const { return fit_v0.FlightLength_WithRespectTo(fit.vtx); }
    [[nodiscard]] double V0_FlightLength_DivByErr_WithRespectToSV() const { return fit_v0.FlightLength_DivByErr_WithRespectTo(fit.vtx, fit.cov_vtx); }
    [[nodiscard]] double V0_DCA_WithRespectToSV() const { return fit_v0.DCA_WithRespectTo(fit.vtx); }
    [[nodiscard]] double V0_CPA_WithRespectToSV() const { return fit_v0.CPA_WithRespectTo(fit.vtx); }

   private:
    Unit::PostFit fit_v0;  // the (anti)lambda as a leg of the closed tree
};

}  // namespace Cached
