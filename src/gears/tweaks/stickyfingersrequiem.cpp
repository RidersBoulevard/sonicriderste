#include "context.hpp"

ASMUsed void Player_StickyFingersRequiem(Player *attackedPlayer, Player *attackingPlayer) {
    s32 airToAdd, extraAir, thirtyPercentCurrent, thirtyPercentMax;
    f32 scalingMultiplier;
    
    if (attackingPlayer->extremeGear == Accelerator) {
        airToAdd = (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 20) / 100;
        thirtyPercentCurrent = (attackedPlayer->currentAir * 30) / 100;
        thirtyPercentMax = (attackedPlayer->gearStats[attackedPlayer->level].maxAir * 30) / 100;

        if (thirtyPercentCurrent > thirtyPercentMax) extraAir = thirtyPercentMax;
        else extraAir = thirtyPercentCurrent;

        scalingMultiplier = static_cast<f32>(attackingPlayer->gearStats[attackingPlayer->level].maxAir) /
                            static_cast<f32>(attackedPlayer->gearStats[attackedPlayer->level].maxAir);

        airToAdd += static_cast<s32>(static_cast<f32>(extraAir) * scalingMultiplier);

        attackingPlayer->currentAir += airToAdd;

    } 
    else if (isSuperCharacter(*attackingPlayer, Shadow)) {
        attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 30) / 100;
    }
    else {
        attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 15) / 100;
    }
}