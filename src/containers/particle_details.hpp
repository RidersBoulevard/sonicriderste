#pragma once

#include "types.hpp"
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

/// A struct used for the Particle task. Defines the specifics of the particles you wish to spawn.
struct ParticleDetails {
	u8 particleType;
	u8 particleCount; ///< i think?
	u16 rangeOpacity; ///< for super sonic aura, can cut off the top of the aura if you make this value smaller
	bool unk4;
	u8 textureID;
	ObjectGroups objectGroupID;
	f32 textureWidth;
	f32 textureHeight;
	u32 unk10;
	u16 mainParticleCount;
	u16 subParticleCount;
	f32 upwardsVelocity;
	f32 outwardsVelocity;
	f32 distributionVelocity;
	f32 downwardsVelocity;
	u16 lifetime; ///< in frames
	u16 frequency;
	RGBAFloat mainParticleColor;
	RGBAFloat subParticleColor;
	f32 extraHorizontalDistribution;
	f32 extraVerticalDistribution;
	u32 unk54;
	u32 unk58;
	u32 unk5C;
	u32 unk60;
	u32 unk64;
	u32 unk68;
	u32 unk6C;
	f32 unk70;
};
static_assert(sizeof(ParticleDetails) == 0x74);