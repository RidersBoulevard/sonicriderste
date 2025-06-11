#include "riders/player.hpp"

ASMUsed f32 Player_TurbulenceTrickSpeed(Player *player) {
    f32 newTrickSpeed;
    if (player->turbulenceTrickType > 2) {
        newTrickSpeed = player->speed + pSpeed(50);
    } else {
        newTrickSpeed = player->speed + pSpeed(100);
    }

    // if (player->characterArchetype == CharacterArchetype::Trickster)
    // 	newTrickSpeed += pSpeed(50);

    return newTrickSpeed;
}