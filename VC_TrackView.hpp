#pragma once

#include <cstddef>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>

#include <ROOT/RNTupleModel.hxx>

#include "Constants.hpp"
#include "Math.hpp"
#include "VC_Track.hpp"

namespace Vector {

struct TrackView {

    TrackView() = delete;
    explicit TrackView(const Vector::Track* source, std::size_t entry = 0) : Source{source}, Entry{entry} {}

    // # Manipulation # //

    [[nodiscard]] std::size_t Size() const { return (*Source->X).size(); }

    void CacheCalculations(std::size_t entry, const ROOT::Math::XYZPoint& pv, double bz) {
        namespace CMath = Common::Math;
        Entry = entry;
        // -- update momentum
        mom_vec.SetCoordinates(Px(), Py(), Pz());
        // -- update pca w.r.t. pv
        pca_wrt_pv = CMath::FastPCA_HelixVertex(mom_vec, {X(), Y(), Z()}, Charge(), pv, bz);
        // -- update difference between pca and pv
        dist_pca2pv = pca_wrt_pv - pv;
    }

    // Getters //
    [[nodiscard]] unsigned int EsdEntry() const { return (*Source->EsdEntry)[Entry]; }
    [[nodiscard]] float X() const { return (*Source->X)[Entry]; }
    [[nodiscard]] float Y() const { return (*Source->Y)[Entry]; }
    [[nodiscard]] float Z() const { return (*Source->Z)[Entry]; }
    [[nodiscard]] float Px() const { return (*Source->Px)[Entry]; }
    [[nodiscard]] float Py() const { return (*Source->Py)[Entry]; }
    [[nodiscard]] float Pz() const { return (*Source->Pz)[Entry]; }
    [[nodiscard]] int Charge() const { return (*Source->Charge)[Entry]; }
    [[nodiscard]] float PreDCAxy() const { return (*Source->PreDCAxy)[Entry]; }
    [[nodiscard]] float PreDCAz() const { return (*Source->PreDCAz)[Entry]; }
    [[nodiscard]] float TPC_Signal() const { return (*Source->TPC_Signal)[Entry]; }
    [[nodiscard]] float NSigmaPion() const { return (*Source->NSigmaPion)[Entry]; }
    [[nodiscard]] float NSigmaKaon() const { return (*Source->NSigmaKaon)[Entry]; }
    [[nodiscard]] float NSigmaProton() const { return (*Source->NSigmaProton)[Entry]; }

    [[nodiscard]] std::array<float, Common::NCovMatrixComponents_State6>& CovMatrix() const { return (*Source->CovMatrix)[Entry]; }
    [[nodiscard]] float SigmaX2() const { return (*Source->CovMatrix)[Entry][Common::ESigma::X2]; }
    [[nodiscard]] float SigmaXY() const { return (*Source->CovMatrix)[Entry][Common::ESigma::XY]; }
    [[nodiscard]] float SigmaY2() const { return (*Source->CovMatrix)[Entry][Common::ESigma::Y2]; }
    [[nodiscard]] float SigmaXZ() const { return (*Source->CovMatrix)[Entry][Common::ESigma::XZ]; }
    [[nodiscard]] float SigmaYZ() const { return (*Source->CovMatrix)[Entry][Common::ESigma::YZ]; }
    [[nodiscard]] float SigmaZ2() const { return (*Source->CovMatrix)[Entry][Common::ESigma::Z2]; }
    [[nodiscard]] float SigmaXPx() const { return (*Source->CovMatrix)[Entry][Common::ESigma::XPx]; }
    [[nodiscard]] float SigmaYPx() const { return (*Source->CovMatrix)[Entry][Common::ESigma::YPx]; }
    [[nodiscard]] float SigmaZPx() const { return (*Source->CovMatrix)[Entry][Common::ESigma::ZPx]; }
    [[nodiscard]] float SigmaPx2() const { return (*Source->CovMatrix)[Entry][Common::ESigma::Px2]; }
    [[nodiscard]] float SigmaXPy() const { return (*Source->CovMatrix)[Entry][Common::ESigma::XPy]; }
    [[nodiscard]] float SigmaYPy() const { return (*Source->CovMatrix)[Entry][Common::ESigma::YPy]; }
    [[nodiscard]] float SigmaZPy() const { return (*Source->CovMatrix)[Entry][Common::ESigma::ZPy]; }
    [[nodiscard]] float SigmaPxPy() const { return (*Source->CovMatrix)[Entry][Common::ESigma::PxPy]; }
    [[nodiscard]] float SigmaPy2() const { return (*Source->CovMatrix)[Entry][Common::ESigma::Py2]; }
    [[nodiscard]] float SigmaXPz() const { return (*Source->CovMatrix)[Entry][Common::ESigma::XPz]; }
    [[nodiscard]] float SigmaYPz() const { return (*Source->CovMatrix)[Entry][Common::ESigma::YPz]; }
    [[nodiscard]] float SigmaZPz() const { return (*Source->CovMatrix)[Entry][Common::ESigma::ZPz]; }
    [[nodiscard]] float SigmaPxPz() const { return (*Source->CovMatrix)[Entry][Common::ESigma::PxPz]; }
    [[nodiscard]] float SigmaPyPz() const { return (*Source->CovMatrix)[Entry][Common::ESigma::PyPz]; }
    [[nodiscard]] float SigmaPz2() const { return (*Source->CovMatrix)[Entry][Common::ESigma::Pz2]; }

#if E2R_TPC_EXTRA
    [[nodiscard]] float TPC_DCAxy() const { return (*Source->TPC_DCAxy)[Entry]; }
    [[nodiscard]] float TPC_DCAz() const { return (*Source->TPC_DCAz)[Entry]; }
    [[nodiscard]] float TPC_NCrossedRows() const { return (*Source->TPC_NCrossedRows)[Entry]; }
    [[nodiscard]] unsigned int TPC_NClusters() const { return (*Source->TPC_NClusters)[Entry]; }
    [[nodiscard]] unsigned int TPC_NClustersLC() const { return (*Source->TPC_NClustersLC)[Entry]; }
    [[nodiscard]] unsigned int TPC_NClustersFound() const { return (*Source->TPC_NClustersFound)[Entry]; }
    [[nodiscard]] unsigned int TPC_NClustersShared() const { return (*Source->TPC_NClustersShared)[Entry]; }
    [[nodiscard]] float TPC_Chi2() const { return (*Source->TPC_Chi2)[Entry]; }
    [[nodiscard]] float TPC_Chi2Constrained() const { return (*Source->TPC_Chi2Constrained)[Entry]; }
    [[nodiscard]] float TPC_Chi2TCVG() const { return (*Source->TPC_Chi2TCVG)[Entry]; }
    [[nodiscard]] float TPC_SignalTunedOnData() const { return (*Source->TPC_SignalTunedOnData)[Entry]; }
    [[nodiscard]] float TPC_SignalSigma() const { return (*Source->TPC_SignalSigma)[Entry]; }
    [[nodiscard]] float TPC_SignalCorrected() const { return (*Source->TPC_SignalCorrected)[Entry]; }
    [[nodiscard]] float TPC_ESignalPion() const { return (*Source->TPC_ESignalPion)[Entry]; }
    [[nodiscard]] float TPC_ESigmaPion() const { return (*Source->TPC_ESigmaPion)[Entry]; }
    [[nodiscard]] float TPC_ESignalKaon() const { return (*Source->TPC_ESignalKaon)[Entry]; }
    [[nodiscard]] float TPC_ESigmaKaon() const { return (*Source->TPC_ESigmaKaon)[Entry]; }
    [[nodiscard]] float TPC_ESignalProton() const { return (*Source->TPC_ESignalProton)[Entry]; }
    [[nodiscard]] float TPC_ESigmaProton() const { return (*Source->TPC_ESigmaProton)[Entry]; }
    [[nodiscard]] unsigned int TPC_SignalN() const { return (*Source->TPC_SignalN)[Entry]; }
    [[nodiscard]] float TPC_PointsFirst() const { return (*Source->TPC_PointsFirst)[Entry]; }
    [[nodiscard]] float TPC_PointsIndexMax() const { return (*Source->TPC_PointsIndexMax)[Entry]; }
    [[nodiscard]] float TPC_PointsLast() const { return (*Source->TPC_PointsLast)[Entry]; }
    [[nodiscard]] float TPC_PointsMaxDens() const { return (*Source->TPC_PointsMaxDens)[Entry]; }
    [[nodiscard]] int TPC_FirstRow() const { return (*Source->TPC_FirstRow)[Entry]; }
#endif

    [[nodiscard]] unsigned int McEntry() const { return (*Source->McEntry)[Entry]; }
    [[nodiscard]] int PdgCode() const { return (*Source->PdgCode)[Entry]; }
    [[nodiscard]] float MC_Px() const { return (*Source->MC_Px)[Entry]; }
    [[nodiscard]] float MC_Py() const { return (*Source->MC_Py)[Entry]; }
    [[nodiscard]] float MC_Pz() const { return (*Source->MC_Pz)[Entry]; }
    [[nodiscard]] float MC_Energy() const { return (*Source->MC_Energy)[Entry]; }
    [[nodiscard]] bool IsTrue() const { return static_cast<bool>((*Source->IsTrue)[Entry]); }
    [[nodiscard]] bool IsSecondary() const { return static_cast<bool>((*Source->IsSecondary)[Entry]); }
    [[nodiscard]] bool IsSignal() const { return static_cast<bool>((*Source->IsSignal)[Entry]); }
    [[nodiscard]] int ReactionID() const { return (*Source->ReactionID)[Entry]; }
    [[nodiscard]] float Origin_X() const { return (*Source->Origin_X)[Entry]; }
    [[nodiscard]] float Origin_Y() const { return (*Source->Origin_Y)[Entry]; }
    [[nodiscard]] float Origin_Z() const { return (*Source->Origin_Z)[Entry]; }
    [[nodiscard]] int Mother_McEntry() const { return (*Source->Mother_McEntry)[Entry]; }
    [[nodiscard]] int Mother_PdgCode() const { return (*Source->Mother_PdgCode)[Entry]; }
    [[nodiscard]] int GM_McEntry() const { return (*Source->GM_McEntry)[Entry]; }
    [[nodiscard]] int GM_PdgCode() const { return (*Source->GM_PdgCode)[Entry]; }

    // Derived Info //
    // -- kinematics
    [[nodiscard]] double P() const { return mom_vec.R(); }
    [[nodiscard]] double Pt() const { return mom_vec.Rho(); }
    [[nodiscard]] double Eta() const { return mom_vec.Eta(); }
    [[nodiscard]] double AbsEta() const { return std::abs(Eta()); }
    [[nodiscard]] double Phi() const { return mom_vec.Phi(); }
    // -- geometry
    [[nodiscard]] double DCAxy() const { return dist_pca2pv.Rho(); }
    [[nodiscard]] double DCAz() const { return dist_pca2pv.Z(); }
    [[nodiscard]] double DCAxyz() const { return dist_pca2pv.R(); }
    [[nodiscard]] double PCA_X() const { return pca_wrt_pv.X(); }
    [[nodiscard]] double PCA_Y() const { return pca_wrt_pv.Y(); }
    [[nodiscard]] double PCA_Z() const { return pca_wrt_pv.Z(); }
    [[nodiscard]] double PCA_Phi() const { return dist_pca2pv.Phi(); }
    [[nodiscard]] double PCA_Theta() const { return dist_pca2pv.Theta(); }

    // cached variables //
    ROOT::Math::XYZVector mom_vec;
    ROOT::Math::XYZVector dist_pca2pv;
    ROOT::Math::XYZPoint pca_wrt_pv;
    // main variables //
    const Vector::Track* Source;
    std::size_t Entry{};
};

}  // namespace Vector
