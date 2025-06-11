#pragma once

#include "types.hpp"

using NNS_SubMotion = void;
struct NNS_Motion {
    u32 type;
    f32 startFrame;
    f32 endFrame;
    u32 subMotionCount;
    NNS_SubMotion *subMotions;
    f32 framerate;
};