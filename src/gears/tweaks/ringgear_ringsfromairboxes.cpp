#include "riders/player.hpp"

ASMUsed void RingGear_RingsFromAirBoxes(Player *player) {
    s32 newAir;
    if (player->specialFlags.hasAny(ringGear)) {
        newAir = player->currentAir + (player->gearStats[player->level].maxAir / 20); // 5 rings
        if (newAir > player->gearStats[player->level].maxAir) {
            newAir = player->gearStats[player->level].maxAir;
        }
        player->currentAir = newAir;
    }
}