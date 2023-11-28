#pragma once

#include "riders/player.hpp"
#include "riders/object.hpp"

/// All floats in this struct range from 0.0 to 255.0. (per RGB values)
struct RGBAFloat {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
};

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

struct SuperAuraDetail {
    ParticleDetails* particles;
    void** texList;
};

ASMUsed void SuperSonicAuraCXX(Player* player);
SuperAuraDetail GetSuperAuraDetail(Player* player);
ASMUsed void UpdateAuraMatrices(Player* player);