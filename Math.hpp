#pragma once

#include <cmath>

#include <Math/GenVector/VectorUtil.h>
#include <Math/Point3D.h>
#include <Math/Vector3D.h>

namespace Common::Math {

namespace RMath = ROOT::Math;

// Calculate the cosine of the pointing angle of a particle with momentum Px,Py,Pz and vertex X,Y,Z w.r.t. to a reference point.
inline double CosinePointingAngle(const RMath::XYZVector& mom_v0, const RMath::XYZPoint& pos_v0, const RMath::XYZPoint& pos_ref) {
    return RMath::VectorUtil::CosTheta(mom_v0, pos_v0 - pos_ref);
}

// Overload of `CosinePointingAngle(...)`, using scalars instead of vectors.
inline double CosinePointingAngle(double v0_px, double v0_py, double v0_pz,  //
                                  double v0_x, double v0_y, double v0_z,     //
                                  double ref_x, double ref_y, double ref_z) {
    return CosinePointingAngle({v0_px, v0_py, v0_pz}, {v0_x, v0_y, v0_z}, {ref_x, ref_y, ref_z});
}

inline RMath::XYZPoint FastPCA_LineVertex(const RMath::XYZVector& mom_v0, const RMath::XYZPoint& pos_v0, const RMath::XYZPoint& pos_ref) {
    RMath::XYZVector delta = pos_ref - pos_v0;
    double t = delta.Dot(mom_v0) / mom_v0.Mag2();
    return pos_v0 + t * mom_v0;
}

inline double FastDCA_LineVertex(const RMath::XYZVector& mom_v0, const RMath::XYZPoint& pos_v0, const RMath::XYZPoint& pos_ref) {
    return std::sqrt((FastPCA_LineVertex(mom_v0, pos_v0, pos_ref) - pos_ref).Mag2());
}

}  // namespace Common::Math
