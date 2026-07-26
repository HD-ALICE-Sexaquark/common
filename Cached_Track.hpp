#pragma once

#include <cmath>

#include <Math/Point3Dfwd.h>
#include <Math/Vector3Dfwd.h>

#include "Constants.hpp"
#include "Math.hpp"
#include "POD_Track.hpp"

namespace Cached {

struct Track : POD::Track {

    Track(const POD::Track& track, const ROOT::Math::XYZPoint& ref, double bz)  //
        : POD::Track(track),
          mom{track.Px, track.Py, track.Pz},
          pv{ref},
          pca_wrt_pv{Common::Math::FastPCA_HelixVertex(mom, {track.X, track.Y, track.Z}, track.Charge, pv, bz)} {}

    // kinematics
    [[nodiscard]] double Pt() const { return mom.Rho(); }
    [[nodiscard]] double P() const { return mom.R(); }
    [[nodiscard]] double Eta() const { return mom.Eta(); }
    [[nodiscard]] double AbsEta() const { return std::abs(Eta()); }
    [[nodiscard]] double Phi() const { return mom.Phi(); }
    // geometry
    [[nodiscard]] double DCAxy() const { return (pca_wrt_pv - pv).Rho(); }
    [[nodiscard]] double DCAz() const { return std::abs((pca_wrt_pv - pv).Z()); }
    [[nodiscard]] double DCAxyz() const { return (pca_wrt_pv - pv).R(); }
#if E2T_TPC_EXTRA
    // tpc info
    [[nodiscard]] double TPC_Chi2_NCls() const {
        return TPC_NClusters > 0 ? static_cast<double>(TPC_Chi2) / static_cast<double>(TPC_NClusters)  //
                                 : Common::DummyDouble;
    }
    [[nodiscard]] double TPC_NCrossedRows_NClsF() const {
        return TPC_NClustersFindable > 0 ? static_cast<double>(TPC_NCrossedRows) / static_cast<double>(TPC_NClustersFindable) : Common::DummyDouble;
    }
#endif

   private:
    ROOT::Math::XYZVector mom;
    ROOT::Math::XYZPoint pv;
    ROOT::Math::XYZPoint pca_wrt_pv;
};

}  // namespace Cached
