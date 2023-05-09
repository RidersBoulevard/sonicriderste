#include "lcanceling.hpp"

PlayerLCancelInfo playerLCancelInfo[8];

void Player_LCancelHandler(Player *player) {
    PlayerLCancelInfo &info = playerLCancelInfo[player->index];

    // deduct timers
    if (info.frameWindow > 0) info.frameWindow -= 1;
    if (info.lockoutTimer > 0) info.lockoutTimer -= 1;

    if (player->state >= FrontflipRamp && player->state <= TurbulenceTrick) {
        if (player->input->toggleFaceButtons & (LButton | RButton) && (info.lockoutTimer == 0) ? TRUE : FALSE) {
            if (player->characterArchetype == TricksterArchetype) 
            info.frameWindow = 7;

            else info.frameWindow = 5;
            
            info.lockoutTimer = 30;
        }
    }
}

ASMUsed u32 Player_CheckLCancel(Player *player, u32 currentTrickStatus) {
	PlayerLCancelInfo &info = playerLCancelInfo[player->index];
    OffroadParticleObject1 *object;
    s32 newAir;

    if (info.frameWindow > 0) {
        if (currentTrickStatus == 2 && !(player->specialFlags & ringGear)) {
            player->changeInAir_loss += (player->gearStats[player->level].maxAir / 20); // removes 5 percent of air for a successful L-cancel, don't remove air otherwise
        }
            object = (OffroadParticleObject1 *)SetTask(&OffroadJumpParticleTask, 0xB0F4, 2)->object;
            object->playerIndex = player->index;
            currentTrickStatus = 0; // make sure not to fail the player's trick
    }
    return currentTrickStatus;
}