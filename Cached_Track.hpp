#pragma once

#include <cmath>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Math.hpp"
#include "POD_Track.hpp"

namespace Cached {

struct Track : POD::Track {

    Track(const POD::Track& track, const ROOT::Math::XYZPoint& ref, double bz, double mass)  //
        : POD::Track(track),
          lv{track.Px, track.Py, track.Pz, Common::Math::Hypot4(track.Px, track.Py, track.Pz, mass)},
          pv{ref},
          pca_wrt_pv{Common::Math::FastPCA_HelixVertex(lv.Vect(), {track.X, track.Y, track.Z}, track.Charge, pv, bz)} {}

    // kinematics
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double AbsEta() const { return std::abs(Eta()); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    // geometry
    [[nodiscard]] double DCAxy() const { return (pca_wrt_pv - pv).Rho(); }
    [[nodiscard]] double DCAz() const { return std::abs((pca_wrt_pv - pv).Z()); }
    [[nodiscard]] double DCAxyz() const { return (pca_wrt_pv - pv).R(); }

   private:
    ROOT::Math::PxPyPzEVector lv;
    ROOT::Math::XYZPoint pca_wrt_pv;
    ROOT::Math::XYZPoint pv;
};

}  // namespace Cached
