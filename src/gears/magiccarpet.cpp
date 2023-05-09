#include "magiccarpet.hpp"

void Player_MagicCarpet(struct Player *player) {
const f32 MagicCarpet_BoostSpeeds[3] = {pSpeed(207.0f), pSpeed(237.0f), pSpeed(257.0f)};
    if (player->extremeGear != MagicCarpet) return;

    if (player->characterArchetype == BoostArchetype)
    {
        if (player->state & Jump) // attempts to fix lightboard effect for boost archetype, TODO: try a different solution, this works temporarily
        {
            player->specialFlags &= ~lightBoardEffect;
        }
        else player->specialFlags |= lightBoardEffect;
        player->specialFlags &= ~berserkerEffect;
        if (player->movementFlags & 0x0400) return;
        player->gearStats[player->level].boostSpeed = MagicCarpet_BoostSpeeds[player->level];
        return;
    }
    
    player->specialFlags |= (legendEffect | alwaysOnIce | tornadoBoost | noSpeedLossChargingJump | noSpeedLossTurning | lightBoardEffect);
    player->specialFlags &= ~lowBoost;

    if (player->currentAir >= (player->gearStats[player->level].maxAir * 50) / 100)
    {
        player->specialFlags |= berserkerEffect;
    }
    else {
        if (player->specialFlags & berserkerEffect && !(player->movementFlags & boosting) && player->state != AttackingPlayer) player->unkF40 = 0;
        player->specialFlags &= ~berserkerEffect;
    }
}