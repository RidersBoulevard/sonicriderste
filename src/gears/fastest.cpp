#include "fastest.hpp"

constexpr f32 Fastest_SuperCruiseSpeeds[2][3] = {
  // level 1      level 2      level 3
    {pSpeed(240), pSpeed(250), pSpeed(260)}, // normal
    {pSpeed(245), pSpeed(255), pSpeed(265)}, // late booster and combat archetype
};

void Player_Fastest(Player *player) {
    u32 isLateBooster, isCombat, isBerserker, isBoostArchetype;
    f32 newSpeed, cruiseSpeed;

    if (player->extremeGear != Fastest) return;

    if (player->currentAir > 0 && player->state != Run && player->input->holdFaceButtons & (BButton | XButton)) {
        if (player->state != Cruise) return;
        isLateBooster = (player->characterArchetype == LateBooster) ? TRUE : FALSE;
        isCombat = (player->characterArchetype == CombatArchetype) ? TRUE : FALSE;
        isBoostArchetype = (player->characterArchetype == BoostArchetype) ? TRUE : FALSE;
        if (player->input->toggleFaceButtons & (BButton | XButton))
        {
            if (isCombat == TRUE) {
                player->currentAir -= 4000; // 20% reduction
            } else if (isLateBooster == TRUE) {
                player->currentAir -= 3500; // 30% reduction
            } else player->currentAir -= 5000;
        }

        isBerserker = (player->fastest_timer >= 90) ? TRUE : FALSE;
        if (!isBerserker) player->fastest_timer += 1;

        player->fastest_superCruise = TRUE;
        newSpeed = player->speed + 0.0027006172839506f; // per frame

        cruiseSpeed = Fastest_SuperCruiseSpeeds[(isBoostArchetype)][player->level];
        // cruiseSpeed = Fastest_SuperCruiseSpeeds[(isCombat | isLateBooster)][player->level];
        if (newSpeed < cruiseSpeed) player->speed = newSpeed;
        player->speedCap = cruiseSpeed;

        if (isCombat == TRUE) {
            player->currentAir -= 320; // 20% reduction
        } else if (isLateBooster == TRUE) {
            player->currentAir -= 280; // 30% reduction
        } else player->currentAir -= 400;

        if (isBerserker) player->specialFlags |= berserkerEffect;
    } else {
        player->fastest_superCruise = FALSE;
        player->fastest_timer = 0;
        player->specialFlags &= ~berserkerEffect;
        if (player->state == Cruise) {
            player->unkF40 = 0;
            player->speedCap = player->gearStats[player->level].topSpeed;
        }
    }
}