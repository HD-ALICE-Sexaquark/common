#pragma once

#include <cmath>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

#include "Constants.hpp"
#include "Math.hpp"
#include "POD_McParticle.hpp"

namespace Cached {

struct McParticle : POD::Extended::McParticle {

    McParticle(const POD::Extended::McParticle& e_mc, const ROOT::Math::XYZPoint& ref, double bz)  //
        : POD::Extended::McParticle(e_mc),
          lv{e_mc.Px, e_mc.Py, e_mc.Pz, Common::Math::Hypot4(e_mc.Px, e_mc.Py, e_mc.Pz, e_mc.Energy)},
          origin{e_mc.Origin_X, e_mc.Origin_Y, e_mc.Origin_Z},
          decay{e_mc.Decay_X, e_mc.Decay_Y, e_mc.Decay_Z},
          pv{ref} {
        if (Charge != 0) {
            pca_wrt_pv = Common::Math::FastPCA_HelixVertex(lv.Vect(), decay, e_mc.Charge, pv, bz);
        } else {
            pca_wrt_pv = Common::Math::FastPCA_LineVertex(lv.Vect(), decay, pv);
        }
    }

    // kinematics
    [[nodiscard]] double Pt() const { return lv.Pt(); }
    [[nodiscard]] double P() const { return lv.P(); }
    [[nodiscard]] double Mass() const { return lv.M(); }
    [[nodiscard]] double Eta() const { return lv.Eta(); }
    [[nodiscard]] double Rapidity() const { return lv.Rapidity(); }
    [[nodiscard]] double Phi() const { return lv.Phi(); }
    // geometry
    [[nodiscard]] double Origin_Radius2D() const { return origin.Rho(); }
    [[nodiscard]] double Origin_Radius3D() const { return origin.R(); }
    [[nodiscard]] double Decay_Radius2D() const { return decay.Rho(); }
    [[nodiscard]] double Decay_Radius3D() const { return decay.R(); }
    [[nodiscard]] double DCAxy_wrt_PV() const { return (pv - pca_wrt_pv).Rho(); }
    [[nodiscard]] double DCAz_wrt_PV() const { return std::abs((pv - pca_wrt_pv).Z()); }
    [[nodiscard]] double DCA_wrt_PV() const { return (pv - pca_wrt_pv).R(); }
    [[nodiscard]] double CPA_wrt_PV() const { return Charge == 0 ? Common::Math::CosinePointingAngle(lv.Vect(), decay, pv) : Common::DummyFloat; }
    // -- kinematics + geometry
    [[nodiscard]] double DecayLength() const { return (decay - origin).R(); }
    // -- aliases
    [[nodiscard]] bool IsLogicalPrimary() const { return Mother_McEntry == Common::DummyInt; }
    [[nodiscard]] int ReactionID() const { return SignalID; }

   private:
    ROOT::Math::PxPyPzEVector lv;
    ROOT::Math::XYZPoint origin;
    ROOT::Math::XYZPoint decay;
    ROOT::Math::XYZPoint pv;
    ROOT::Math::XYZPoint pca_wrt_pv;
};

}  // namespace Cached
