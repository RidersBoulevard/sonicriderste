#include "afterburner.hpp"

void Player_Afterburner(struct Player *player) {
    if (player->characterArchetype != TricksterArchetype) return;
    
    if (player->state == StartLine) magneticImpulseData[player->index].afterburnerTimer = 0;

    if ((magneticImpulseData[player->index].afterburnerTimer % 60 == 0) && 
    magneticImpulseData[player->index].afterburnerTimer != 0 &&
    ((player->state >= Cruise && player->state <= Fall) || player->state == Run)) {
        if (player->extremeGear == SuperHangOn) player->currentAir += 1000;
        else if (player->extremeGear == AdvantageF) player->currentAir += (player->gearStats[player->level].maxAir * 1) / 100;
        else if (player->specialFlags & ringGear) player->currentAir += (player->gearStats[player->level].maxAir * 3) / 100;
        else player->currentAir += 10000;
    }
}