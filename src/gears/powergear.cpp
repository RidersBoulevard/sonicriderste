#include "powergear.hpp"

void Player_PowerGear(Player *player) {
    if (player->extremeGear != PowerGear) return;
    if (player->characterArchetype == DriftArchetype)
    {
       player->gearStats[player->level].driftCost = (player->gearptr->levelStats[player->level].driftingAirCost * 90) / 100;
    }
    else player->gearStats[player->level].driftCost = (player->gearptr->levelStats[player->level].driftingAirCost * 95) / 100;
}