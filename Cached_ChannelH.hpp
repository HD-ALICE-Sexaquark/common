#pragma once

#include <array>

#include <Math/Point3D.h>

#include "Cached_Sexaquark.hpp"
#include "Constants.hpp"
#include "DB_ReactionChannels.hpp"
#include "POD_Sexaquark.hpp"

namespace Cached {

// Channel H: antisexaquark + proton -> charged kaon+ charged kaon
struct ChannelH : Sexaquark {
    ChannelH(const POD::Sexaquark& sexa, const ROOT::Math::XYZPoint& ref, const std::array<float, Common::NCovMatrixComponents_State3>& ref_cov)
        : Sexaquark(sexa, DB::ReactionChannels::StruckNucleonMass('H'), ref, ref_cov) {}
};

}  // namespace Cached
