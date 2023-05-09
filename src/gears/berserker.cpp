#include "berserker.hpp"

u8 Player_BerserkerStatIncreaseMultipliers[8];

void ClearBerserkerStatGainData() {
    TRK_memset(&Player_BerserkerStatIncreaseMultipliers, 0, 8);
}

void Player_BerserkerStatGain(Player *attackedPlayer, Player *attackingPlayer) {
    u32 i, intMultiplier;
    f32 multiplier;

    if (attackedPlayer->extremeGear == Berserker && attackedPlayer->playerType == 0) {
        // berserker gets attacked, stat reset

        multiplier = Player_BerserkerStatIncreaseMultipliers[attackedPlayer->index];

        for (i=0; i<3; i++) {
            attackedPlayer->gearStats[i].boostCost = attackedPlayer->gearptr->levelStats[i].boostCost;
            attackedPlayer->gearStats[i].boostSpeed = attackedPlayer->gearptr->levelStats[i].boostSpeed;

            attackedPlayer->gearStats[i].topSpeed -= multiplier * pSpeed(3.0f); // +3 top speed
        }

        Player_BerserkerStatIncreaseMultipliers[attackedPlayer->index] = 0;
    }

    if (attackingPlayer->extremeGear == Berserker && attackingPlayer->playerType == 0) {
        // berserker attacks, stat increase

        intMultiplier = Player_BerserkerStatIncreaseMultipliers[attackingPlayer->index] += 1;

        for (i=0; i<3; i++) {
            attackingPlayer->gearStats[i].boostSpeed += pSpeed(1.0f);
            attackingPlayer->gearStats[i].topSpeed += pSpeed(3.0f);

            if (100 - intMultiplier * 5 > 0) {
                attackingPlayer->gearStats[i].boostCost =
						s32((static_cast<u32>(attackingPlayer->gearptr->levelStats[i].boostCost / 100)) * (100 - intMultiplier * 5));
            }
        }
    }
}

void Player_BerserkerStatResetTornado(Player *player) {
    u32 i;
    f32 multiplier;

    if (player->extremeGear == Berserker && player->playerType == 0) {
        // berserker gets attacked, stat reset

        multiplier = Player_BerserkerStatIncreaseMultipliers[player->index];

        
        for (i=0; i<3; i++) {
            player->gearStats[i].boostCost = player->gearptr->levelStats[i].boostCost;
            if (player->characterArchetype != BoostArchetype) player->gearStats[i].boostSpeed = player->gearptr->levelStats[i].boostSpeed;

            player->gearStats[i].topSpeed -= multiplier * pSpeed(3.0f); // +3 top speed
        }
        
        if (player->characterArchetype == BoostArchetype)
        {
            player->gearStats[0].boostSpeed = pSpeed(203.0f);
            player->gearStats[1].boostSpeed = pSpeed(234.0f);
            player->gearStats[2].boostSpeed = pSpeed(255.0f);
        }
        

        if (player->characterArchetype == BoostArchetype)
        {
            player->gearStats[0].boostSpeed = pSpeed(203.0f);
            player->gearStats[1].boostSpeed = pSpeed(234.0f);
            player->gearStats[2].boostSpeed = pSpeed(255.0f);
        }

        Player_BerserkerStatIncreaseMultipliers[player->index] = 0;
    }
}