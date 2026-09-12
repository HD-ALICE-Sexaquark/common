#pragma once

#include <array>
#include <cstdint>

#include "Constants.hpp"

namespace POD {

// NOTES:
// - the (anti)h-dibaryon's 4-momentum is not stored: it is `Fit_Lambda1_* + Fit_Lambda2_*` exactly (closure). It is
//   never pinned, so `Chi2` is the vertex chi2 alone (`NDF = Common::NDF::kVertex`) and `Fit_MassErr` is legitimate.
// - `Seed_Lambda{1,2}_P*` equal each (anti)lambda's own fitted momentum (the seeder does not rotate lines); kept for
//   symmetry with `POD::Sexaquark`, whose legs can be charged
// - not added: `Fit_Lambda{1,2}_{X,Y,Z}`, the same as the candidate's `Fit_{X,Y,Z}`
// - `PVC_*` are the production-vertex constraint's own (`at_pv`): `PVC_Chi2` has `Common::NDF::kProdVertex` d.o.f. and
//   adds to `Chi2`; `PVC_DecayLength` is the signed projection of `dv - pca` onto the constrained momentum
struct Hdibaryon {
    // post-fit
    // -- (anti)h-dibaryon
    std::array<float, Common::NCovMatrixComponents_State3> DV_CovMatrix;
    float Fit_X;  // decay vertex
    float Fit_Y;
    float Fit_Z;
    // -- (anti)lambda 1
    float Fit_Lambda1_Px;
    float Fit_Lambda1_Py;
    float Fit_Lambda1_Pz;
    float Fit_Lambda1_Energy;
    // -- (anti)lambda 2
    float Fit_Lambda2_Px;
    float Fit_Lambda2_Py;
    float Fit_Lambda2_Pz;
    float Fit_Lambda2_Energy;
    // post-seed/pre-fit
    // -- (anti)lambda 1
    float Seed_Lambda1_X;
    float Seed_Lambda1_Y;
    float Seed_Lambda1_Z;
    float Seed_Lambda1_Px;
    float Seed_Lambda1_Py;
    float Seed_Lambda1_Pz;
    float Seed_Lambda1_Energy;
    // -- (anti)lambda 2
    float Seed_Lambda2_X;
    float Seed_Lambda2_Y;
    float Seed_Lambda2_Z;
    float Seed_Lambda2_Px;
    float Seed_Lambda2_Py;
    float Seed_Lambda2_Pz;
    float Seed_Lambda2_Energy;
    // extra
    // -- production vertex constraint
    float PVC_DecayLength;  // `Common::DummyFloat` if not production vertex constraint
    float PVC_DecayLengthErr;
    float PVC_Chi2;
    float Fit_MassErr;
    float Fit_MassPull;  // against 2.234 GeV (the injected mass); m^2 units
    float Chi2;
    std::uint8_t FitFlags;  // `Common::FitFlag` mask
    bool Lambda1_IsAntiLambda;
    bool Lambda2_IsAntiLambda;
};

}  // namespace POD
