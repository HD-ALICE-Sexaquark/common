#pragma once

#include <cstddef>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>

#include <ROOT/RNTupleModel.hxx>

#include "Constants.hpp"
#include "E2R_Track.hpp"
#include "Math.hpp"

namespace E2R {

struct TrackView {

    TrackView() = delete;
    TrackView(const E2R::Track* source, std::size_t entry) : Source{source}, Entry{entry} {}

    // Manipulation //
    void Update(std::size_t entry, const ROOT::Math::XYZPoint& pv, double bz) {
        namespace CMath = Common::Math;
        Entry = entry;
        // -- update momentum
        mom_vec.SetCoordinates(GetPx(), GetPy(), GetPz());
        // -- update pca w.r.t. pv
        pca_wrt_pv = CMath::FastPCA_HelixVertex(mom_vec, {GetX(), GetY(), GetZ()}, GetCharge(), pv, bz);
        // -- update difference between pca and pv
        dist_pca2pv = pca_wrt_pv - pv;
    }
    [[nodiscard]] std::size_t GetSize() const { return (*Source->X).size(); }

    // Getters //
    [[nodiscard]] float GetX() const { return (*Source->X)[Entry]; }
    [[nodiscard]] float GetY() const { return (*Source->Y)[Entry]; }
    [[nodiscard]] float GetZ() const { return (*Source->Z)[Entry]; }
    [[nodiscard]] float GetPx() const { return (*Source->Px)[Entry]; }
    [[nodiscard]] float GetPy() const { return (*Source->Py)[Entry]; }
    [[nodiscard]] float GetPz() const { return (*Source->Pz)[Entry]; }
    [[nodiscard]] int GetCharge() const { return (*Source->Charge)[Entry]; }
    [[nodiscard]] float GetPreDCAxy() const { return (*Source->PreDCAxy)[Entry]; }
    [[nodiscard]] float GetPreDCAz() const { return (*Source->PreDCAz)[Entry]; }
    [[nodiscard]] float GetTPC_Signal() const { return (*Source->TPC_Signal)[Entry]; }
    [[nodiscard]] float GetNSigmaPion() const { return (*Source->NSigmaPion)[Entry]; }
    [[nodiscard]] float GetNSigmaKaon() const { return (*Source->NSigmaKaon)[Entry]; }
    [[nodiscard]] float GetNSigmaProton() const { return (*Source->NSigmaProton)[Entry]; }

    [[nodiscard]] std::array<float, Common::NCovMatrixComponents>& GetCovMatrix() const { return (*Source->CovMatrix)[Entry]; }
    [[nodiscard]] float GetSigmaX2() const { return (*Source->CovMatrix)[Entry][Common::ESigma::X2]; }
    [[nodiscard]] float GetSigmaXY() const { return (*Source->CovMatrix)[Entry][Common::ESigma::XY]; }
    [[nodiscard]] float GetSigmaY2() const { return (*Source->CovMatrix)[Entry][Common::ESigma::Y2]; }
    [[nodiscard]] float GetSigmaXZ() const { return (*Source->CovMatrix)[Entry][Common::ESigma::XZ]; }
    [[nodiscard]] float GetSigmaYZ() const { return (*Source->CovMatrix)[Entry][Common::ESigma::YZ]; }
    [[nodiscard]] float GetSigmaZ2() const { return (*Source->CovMatrix)[Entry][Common::ESigma::Z2]; }
    [[nodiscard]] float GetSigmaXPx() const { return (*Source->CovMatrix)[Entry][Common::ESigma::XPx]; }
    [[nodiscard]] float GetSigmaYPx() const { return (*Source->CovMatrix)[Entry][Common::ESigma::YPx]; }
    [[nodiscard]] float GetSigmaZPx() const { return (*Source->CovMatrix)[Entry][Common::ESigma::ZPx]; }
    [[nodiscard]] float GetSigmaPx2() const { return (*Source->CovMatrix)[Entry][Common::ESigma::Px2]; }
    [[nodiscard]] float GetSigmaXPy() const { return (*Source->CovMatrix)[Entry][Common::ESigma::XPy]; }
    [[nodiscard]] float GetSigmaYPy() const { return (*Source->CovMatrix)[Entry][Common::ESigma::YPy]; }
    [[nodiscard]] float GetSigmaZPy() const { return (*Source->CovMatrix)[Entry][Common::ESigma::ZPy]; }
    [[nodiscard]] float GetSigmaPxPy() const { return (*Source->CovMatrix)[Entry][Common::ESigma::PxPy]; }
    [[nodiscard]] float GetSigmaPy2() const { return (*Source->CovMatrix)[Entry][Common::ESigma::Py2]; }
    [[nodiscard]] float GetSigmaXPz() const { return (*Source->CovMatrix)[Entry][Common::ESigma::XPz]; }
    [[nodiscard]] float GetSigmaYPz() const { return (*Source->CovMatrix)[Entry][Common::ESigma::YPz]; }
    [[nodiscard]] float GetSigmaZPz() const { return (*Source->CovMatrix)[Entry][Common::ESigma::ZPz]; }
    [[nodiscard]] float GetSigmaPxPz() const { return (*Source->CovMatrix)[Entry][Common::ESigma::PxPz]; }
    [[nodiscard]] float GetSigmaPyPz() const { return (*Source->CovMatrix)[Entry][Common::ESigma::PyPz]; }
    [[nodiscard]] float GetSigmaPz2() const { return (*Source->CovMatrix)[Entry][Common::ESigma::Pz2]; }

#if E2R_TPC_EXTRA
    [[nodiscard]] float GetTPC_DCAxy() const { return (*Source->TPC_DCAxy)[Entry]; }
    [[nodiscard]] float GetTPC_DCAz() const { return (*Source->TPC_DCAz)[Entry]; }
    [[nodiscard]] float GetTPC_NCrossedRows() const { return (*Source->TPC_NCrossedRows)[Entry]; }
    [[nodiscard]] unsigned int GetTPC_NClusters() const { return (*Source->TPC_NClusters)[Entry]; }
    [[nodiscard]] unsigned int GetTPC_NClustersLC() const { return (*Source->TPC_NClustersLC)[Entry]; }
    [[nodiscard]] unsigned int GetTPC_NClustersFound() const { return (*Source->TPC_NClustersFound)[Entry]; }
    [[nodiscard]] unsigned int GetTPC_NClustersShared() const { return (*Source->TPC_NClustersShared)[Entry]; }
    [[nodiscard]] float GetTPC_Chi2() const { return (*Source->TPC_Chi2)[Entry]; }
    [[nodiscard]] float GetTPC_Chi2Constrained() const { return (*Source->TPC_Chi2Constrained)[Entry]; }
    [[nodiscard]] float GetTPC_Chi2TCVG() const { return (*Source->TPC_Chi2TCVG)[Entry]; }
    [[nodiscard]] float GetTPC_SignalSigma() const { return (*Source->TPC_SignalSigma)[Entry]; }
    [[nodiscard]] float GetTPC_SignalCorrected() const { return (*Source->TPC_SignalCorrected)[Entry]; }
    [[nodiscard]] float GetTPC_ESignalPion() const { return (*Source->TPC_ESignalPion)[Entry]; }
    [[nodiscard]] float GetTPC_ESigmaPion() const { return (*Source->TPC_ESigmaPion)[Entry]; }
    [[nodiscard]] float GetTPC_ESignalKaon() const { return (*Source->TPC_ESignalKaon)[Entry]; }
    [[nodiscard]] float GetTPC_ESigmaKaon() const { return (*Source->TPC_ESigmaKaon)[Entry]; }
    [[nodiscard]] float GetTPC_ESignalProton() const { return (*Source->TPC_ESignalProton)[Entry]; }
    [[nodiscard]] float GetTPC_ESigmaProton() const { return (*Source->TPC_ESigmaProton)[Entry]; }
    [[nodiscard]] unsigned int GetTPC_SignalN() const { return (*Source->TPC_SignalN)[Entry]; }
    [[nodiscard]] float GetTPC_PointsFirst() const { return (*Source->TPC_PointsFirst)[Entry]; }
    [[nodiscard]] float GetTPC_PointsIndexMax() const { return (*Source->TPC_PointsIndexMax)[Entry]; }
    [[nodiscard]] float GetTPC_PointsLast() const { return (*Source->TPC_PointsLast)[Entry]; }
    [[nodiscard]] float GetTPC_PointsMaxDens() const { return (*Source->TPC_PointsMaxDens)[Entry]; }
    [[nodiscard]] int GetTPC_FirstRow() const { return (*Source->TPC_FirstRow)[Entry]; }
#endif

    [[nodiscard]] int GetMcEntry() const { return (*Source->McEntry)[Entry]; }
#if E2R_TPC_EXTRA
    [[nodiscard]] float GetTPC_SignalTunedOnData() const { return (*Source->TPC_SignalTunedOnData)[Entry]; }
#endif

    // Derived Info //
    [[nodiscard]] double DeriveP() const { return mom_vec.R(); }
    [[nodiscard]] double DerivePt() const { return mom_vec.Rho(); }
    [[nodiscard]] double DeriveEta() const { return mom_vec.Eta(); }
    [[nodiscard]] double DeriveAbsEta() const { return std::abs(DeriveEta()); }
    [[nodiscard]] double DerivePhi() const { return mom_vec.Phi(); }
    [[nodiscard]] double DeriveDCAxy() const { return dist_pca2pv.Rho(); }
    [[nodiscard]] double DeriveDCAz() const { return dist_pca2pv.Z(); }
    [[nodiscard]] double DeriveDCAxyz() const { return dist_pca2pv.R(); }
    [[nodiscard]] double DerivePCA_X() const { return pca_wrt_pv.X(); }
    [[nodiscard]] double DerivePCA_Y() const { return pca_wrt_pv.Y(); }
    [[nodiscard]] double DerivePCA_Z() const { return pca_wrt_pv.Z(); }
    [[nodiscard]] double DerivePCA_Phi() const { return dist_pca2pv.Phi(); }
    [[nodiscard]] double DerivePCA_Theta() const { return dist_pca2pv.Theta(); }

    // cached variables //
    ROOT::Math::XYZVector mom_vec;
    ROOT::Math::XYZVector dist_pca2pv;
    ROOT::Math::XYZPoint pca_wrt_pv;
    // main variables //
    const E2R::Track* Source;
    std::size_t Entry{};
};

}  // namespace E2R
