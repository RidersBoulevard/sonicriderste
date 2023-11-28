#pragma once

#include "globalDefs.hpp"
#include "types.hpp"

struct MaterialRGBKeyframe {
	f32 frame;
	f32 r;
	f32 g;
	f32 b;
};

struct AnimationInfo {
	fillerData<0x24> filler;
	MaterialRGBKeyframe *keyframes;
};

struct MaterialAnimationInfo {
	fillerData<0x10> filler;
	AnimationInfo *animationInfoStart;
};

ASMUsed void Player_JumpChargeParticleColor(MaterialAnimationInfo *info);