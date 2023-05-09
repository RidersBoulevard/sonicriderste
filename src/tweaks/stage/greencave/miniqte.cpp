#include "context.hpp"

ASMUsed void Player_ApplyAirGain_GreenCaveMiniQTE(Player *player, s32 airGain) {
    if (CurrentStage == GreenCave) {
        airGain /= 8;
    }

    player->changeInAir_gain += airGain;
}