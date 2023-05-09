#include "ringgearlevels.hpp"
#include "cosmetics/player/exloads.hpp"

u8 const extraLap = 3;

void Player_RingGearLevels(Player *player) {
    EnabledEXLoads exLoads;
    FetchEnabledEXLoadIDs(player, exLoads);
    if (!(player->specialFlags & ringGear) || player->extremeGear == HangOn || player->extremeGear == ChaosEmerald) return;

    switch (player->currentLap) {
        case 0:
        case 1:
        player->level = 0;
        break;

        case 2:
        player->level = 1;
        break;

        case 3:
        default:
        player->level = 2;
        break;
    }

    switch (player->extremeGear) {
        case AdvantageF:
        if (exLoads.gearExLoadID == TheProfessionalEXLoad) return;
        player->gearStats[player->level].boostCost = (player->gearStats[player->level].maxAir * 5) / 100;
        player->gearStats[player->level].topSpeed = player->gearStats[0].topSpeed;
        player->gearStats[player->level].airDrain = 0;
        player->gearStats[player->level].driftCost = 38; // 0.2 rings/sec
        player->gearStats[player->level].tornadoCost = (player->gearStats[player->level].maxAir * 5) / 100;
        player->gearStats[player->level].maxAir = 100000;
        player->gearStats[player->level].driftDashSpeed = player->gearStats[2].driftDashSpeed;
        break;

        case TheCrazy:
        player->gearStats[player->level].topSpeed = player->gearStats[0].topSpeed;
        player->gearStats[player->level].offroadSpeedCap = player->gearStats[0].offroadSpeedCap;
        player->gearStats[player->level].maxAir = player->gearStats[0].maxAir;
        player->gearStats[player->level].airDrain = player->gearStats[0].airDrain;
        player->gearStats[player->level].tornadoCost = player->gearStats[0].tornadoCost;
        player->gearStats[player->level].driftDashSpeed = player->gearStats[0].driftDashSpeed;
        break;

        case Accelerator:
        player->gearStats[player->level].baseTopSpeed = player->gearStats[0].baseTopSpeed;
        player->gearStats[player->level].baseAccel = player->gearStats[0].baseAccel;
        player->gearStats[player->level].topSpeed = player->gearStats[0].topSpeed;
        player->gearStats[player->level].tier1Accel = player->gearStats[0].tier1Accel;
        player->gearStats[player->level].tier2Accel = player->gearStats[0].tier2Accel;
        player->gearStats[player->level].tier3Accel = player->gearStats[0].tier3Accel;
        player->gearStats[player->level].offroadSpeedCap = player->gearStats[0].offroadSpeedCap;
        player->gearStats[player->level].maxAir = player->gearStats[0].maxAir;
        player->gearStats[player->level].airDrain = player->gearStats[0].airDrain;
        player->gearStats[player->level].driftCost = player->gearStats[0].driftCost;
        player->gearStats[player->level].boostCost = player->gearStats[0].boostCost;
        player->gearStats[player->level].tornadoCost = player->gearStats[0].tornadoCost;
        player->gearStats[player->level].driftDashSpeed = player->gearStats[0].driftDashSpeed;
        break;

        default:
        player->gearStats[player->level] = player->gearStats[0];
        break;
    }
}