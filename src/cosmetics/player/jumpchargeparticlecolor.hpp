#pragma once

#include "context.hpp"

struct MaterialRGBKeyframe {
    f32 frame;
    f32 r;
    f32 g;
    f32 b;
};

struct AnimationInfo {
	char filler[0x24];
	MaterialRGBKeyframe *keyframes;
};

struct MaterialAnimationInfo {
    char filler[0x10];
	AnimationInfo *animationInfoStart;
};

ASMUsed void Player_JumpChargeParticleColor(MaterialAnimationInfo *info);