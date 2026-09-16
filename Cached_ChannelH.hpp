#pragma once

#include <array>

#include <Math/Point3D.h>

#include "Cached_Sexaquark.hpp"
#include "Constants.hpp"
#include "DB_ReactionChannels.hpp"
#include "Math.hpp"
#include "POD_Sexaquark.hpp"
#include "POD_Track.hpp"

namespace Cached {

// Channel H: antisexaquark + proton -> antiproton + charged kaon + charged kaon + pi0; only the kaon pair is reconstructed
struct ChannelH : Sexaquark {

    ChannelH(const POD::Sexaquark& sexa, const POD::Track& kaon1, const POD::Track& kaon2, const ROOT::Math::XYZPoint& ref,
             const std::array<float, Common::NCovMatrixComponents_State3>& ref_cov, double bz)
        : Sexaquark(sexa, DB::ReactionChannels::StruckNucleonMass('H'), ref, ref_cov),
          kaon1_pca_wrt_sv{
              Common::Math::FastPCA_HelixVertex({kaon1.Px, kaon1.Py, kaon1.Pz}, {kaon1.X, kaon1.Y, kaon1.Z}, kaon1.Charge, fit.vtx, bz)},
          kaon2_pca_wrt_sv{
              Common::Math::FastPCA_HelixVertex({kaon2.Px, kaon2.Py, kaon2.Pz}, {kaon2.X, kaon2.Y, kaon2.Z}, kaon2.Charge, fit.vtx, bz)} {}

    // charged kaons' tracks vs antisexaquark fit //
    // -- the fitted kaons pass through the SV by construction, so these are the tracks' own helices
    [[nodiscard]] double Kaon1_DCA_WithRespectToSV() const { return (kaon1_pca_wrt_sv - fit.vtx).R(); }
    [[nodiscard]] double Kaon2_DCA_WithRespectToSV() const { return (kaon2_pca_wrt_sv - fit.vtx).R(); }

   private:
    ROOT::Math::XYZPoint kaon1_pca_wrt_sv;  // each kaon's helix at its closest approach to the SV
    ROOT::Math::XYZPoint kaon2_pca_wrt_sv;
};

}  // namespace Cached
