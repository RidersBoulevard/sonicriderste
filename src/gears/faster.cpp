#include "faster.hpp"

void Player_Faster(struct Player *player) {
    if (player->extremeGear != Faster) return;
    if (player->characterArchetype == BoostArchetype)
    {
        player->gearStats[0].boostSpeed = pSpeed(240.0f);
        player->gearStats[1].boostSpeed = pSpeed(255.0f);
        player->gearStats[2].boostSpeed = pSpeed(270.0f);
    }

    if (player->characterArchetype == TricksterArchetype)
    {
        player->shortcutAirGainMultiplier = (s32)0x3F800000;
        player->unk9C8 = (u32)0x3F800000;
    }
}