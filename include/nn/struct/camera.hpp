#pragma once

#include "nns.hpp"
#include "types.hpp"

struct NNS_CameraTargetUpVector {
    u32 User;
    Angle32 Fov;
    f32 Aspect;
    f32 ZNear;
    f32 ZFar;
    Vector3F Position;
    Vector3F Target;
    Vector3F UpVector;
};