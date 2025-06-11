#include "riders/player.hpp"
#include "riders/stage.hpp"
#include "shared-definitions.h"

ASMUsed u8 Player_GetItemBoxCooldown(const Player &player) {
    u8 cooldown = ITEMBOX_DEFAULT_COOLDOWN;
    bool toSetTimer = true;

    switch (CurrentStage) {
        case SegaIllusion:
            if (player.stage_subState == 2) { toSetTimer = false; }
            break;
        case GreenCave:
            if (player.greenCave_subState == 2) { cooldown = 7; }
            break;
        case IceFactory:
            if (player.death_spawnPoint >= 0x22 && player.death_spawnPoint <= 0x2A) { toSetTimer = false; }
            break;
        default:
            break;
    }

    return toSetTimer ? cooldown : 0;
}