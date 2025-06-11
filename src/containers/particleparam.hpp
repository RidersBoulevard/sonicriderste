#pragma once

#include "types.hpp"
#include "containers/rgba.hpp"
#include "containers/rotation.hpp"
#include "riders/object.hpp"

enum class ParticleDraw : u8 {
    POINT,
    POINT2D,
    LINE,
    LINE2D,
    RADILINE,
    TRIANGLE,
    TRIANGLE2D,
    POLYGON,
    POLYGON2D,
    UNBILL_POLYGON
};

enum class ParticleType : u8 {
    RADIATE,
    SPIRAL,
    ABSORB,
    RANDOM
};

enum ParticleCtrl : u16 {
    ALWAYS_FOLLOW_ME = 1 << 0,
    ROLL = 1 << 1,
    RANGE_SPHERE = 1 << 2,
    DISTCLIP = 1 << 3,
    COL_RETURN = 1 << 4,
    RANDSIZE_ONLYX = 1 << 5,
    DISABLE_Z_COMPARE = 1 << 6,
    DISABLE_Z_WRITE = 1 << 7,
};
ENUM_FLAG_OPERATORS(ParticleCtrl)

enum class NNEPrimBlend : u8 {
    ADD,
    BLEND
};

[[maybe_unused]] constexpr auto TASK_PRIO_PARTICLE_DEF = 0xB3B0;

/// A struct used for the Particle task. Defines the specifics of the particles you wish to spawn. (Most variable names are taken from debug strings, so they're accurate.)
struct ParticleParam {
    ParticleDraw drawType;
    ParticleType ctrlType; ///< seems to be something like particle count
    Flag<ParticleCtrl> ctrlFlag; ///< for super sonic aura, can cut off the top of the aura if you make this value smaller
    NNEPrimBlend blendMode;
    u8 textureID;
    ObjectGroups taskPriority; ///< usually always 0xB3B0 object group (TASK_PRIO_PARTICLE_DEF)
    f32 halfSizeX; ///< texture width normalized to 1.0f
    f32 halfSizeY; ///< texture height normalized to 1.0f
    f32 halfSizeZ;
    u16 num; ///< particle count
    s8 randSize;
    u8 randAppear;
    f32 speed; ///< upwards velocity
    f32 randSpeedXZ; ///< outwards velocity
    f32 randSpeedY; ///< distribution velocity
    f32 gravitation;
    s16 lifeFrame; ///< lifetime in frames
    s16 loops; ///< frequency
    RGBAFloat startColor;
    RGBAFloat endColor;
    f32 randSizeX; ///< extra X distribution
    f32 randSizeY; ///< extra Y distribution
    f32 randSizeZ; ///< extra Z distribution
    BAMSRotation rot;
    BAMSRotation rotRand;
    f32 rotRandSize;
};
static_assert(sizeof(ParticleParam) == 0x74);