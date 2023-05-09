#include "turbostar.hpp"
#include "handlers/player/initgeardata.hpp"

void Player_TurboStar(struct Player *player) {
    if (player->extremeGear != TurboStar) return;
    if (player->characterArchetype == BoostArchetype && player->level4 == 1) {
        player->gearStats[player->level].boostSpeed = pSpeed(263.0f);
    } else if (player->characterArchetype == BoostArchetype) player->gearStats[player->level].boostSpeed = player->gearptr->levelStats[player->level].boostSpeed + BoostArchetypeBoostSpeeds[player->level];
}