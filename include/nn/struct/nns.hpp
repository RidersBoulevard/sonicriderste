#pragma once

#include "containers/vector3.hpp"

using NNS_Matrix = Matrix3x4F;
using NNS_Matrix44 = Matrix4x4F;
using Angle16 = s16;
using Angle32 = s32;

namespace nn {
    [[nodiscard]] inline constexpr Angle32 DegToA32(const f32 deg) {
        return static_cast<Angle32>(deg * 182.04444f);
    }

    [[nodiscard]] inline constexpr Angle16 DegToA16(const f32 deg) {
        return static_cast<Angle16>(deg * 182.04444f);
    }

    [[nodiscard]] inline constexpr f32 AngleToDeg(const Angle32 angle) {
        return static_cast<f32>(angle) * 0.0054931641f;
    }

    [[nodiscard]] inline constexpr f32 AngleToDeg(const Angle16 angle) {
        return static_cast<f32>(angle) * 0.0054931641f;
    }
}

struct NNS_RGB {
    f32 r;
    f32 g;
    f32 b;
};

struct NNS_TexCoord {
    f32 u;
    f32 v;
};
