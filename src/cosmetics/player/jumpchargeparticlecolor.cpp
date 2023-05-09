#include "jumpchargeparticlecolor.hpp"

ASMUsed void Player_JumpChargeParticleColor(MaterialAnimationInfo *info) {
    u32 i;
    AnimationInfo *animationInfoStart = info->animationInfoStart;
    AnimationInfo *animationInfo = animationInfoStart + 3;
    MaterialRGBKeyframe *keyframe = animationInfo->keyframes;
    keyframe += 11;

    for (i = 0; i < 10; i++, keyframe++) {
        keyframe->r = 0.0f;
        keyframe->g = 1.0f;
        keyframe->b = 0.0f;
    }
}