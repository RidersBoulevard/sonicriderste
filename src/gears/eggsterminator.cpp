#include "eggsterminator.hpp"
#include "cosmetics/player/exloads.hpp"
#include "supertails.hpp"

EggInfo PlayerEggsterminator[8];

void Player_Eggsterminator(Player *player) {
    u8 controllerPort = player->input->port;
    EnabledEXLoads exLoads;
    FetchEnabledEXLoadIDs(player, &exLoads);
    EggInfo *EggInfo = &PlayerEggsterminator[player->index];

    if (exLoads.gearExLoadID != E99EXLoad) return;

    switch (EggInfo->emeraldsInserted)
    {
        case 0:
        if (!(player->rings >= 20)) 
        {
            player->specialFlags |= noPits;
        } else player->specialFlags &= ~noPits;
        break;

        case 1:
        if (!(player->rings >= 40)) 
        {
            player->specialFlags |= noPits;
        } else player->specialFlags &= ~noPits;
        break;

        case 2:
        if (!(player->rings >= 60)) 
        {
            player->specialFlags |= noPits;
        } else player->specialFlags &= ~noPits;
        break;

        case 3:
        if (!(player->rings >= 90)) 
        {
            player->specialFlags |= noPits;
        } else player->specialFlags &= ~noPits;
        break;

        case 4:
        if (!(player->rings >= 100)) 
        {
            player->specialFlags |= noPits;
        } else player->specialFlags &= ~noPits;
        break;
    }

    if (player->state == StartLine && !(player->specialFlags & ringGear)) // level 1
    {
        player->level = 0;
        player->typeAttributes = 0;
        player->gearStats[player->level].baseTopSpeed = pSpeed(180.0f);
        player->gearStats[player->level].topSpeed = pSpeed(180.0f);
        player->specialFlags |= noBoosting;
        EggInfo->extraLevel = 0;
        EggInfo->emeraldsInserted = 0;
        EggInfo->magnetShieldEnabled = FALSE;
        EggInfo->refillAir = 0;
        player->empty[0] = '\0';
    }

    if (player->rings >= 20 && EggInfo->emeraldsInserted == 1 && !(player->specialFlags & ringGear)) // level 2
    {
        player->level = 1;
        if (EggInfo->refillAir == 0)
        {
            player->currentAir = player->gearStats[player->level].maxAir;
            EggInfo->refillAir = 1;
        }
        player->typeAttributes = 0;
        player->gearStats[player->level].baseTopSpeed = pSpeed(190.0f);
        player->gearStats[player->level].topSpeed = pSpeed(190.0f);
        player->gearStats[player->level].boostSpeed = pSpeed(200.0f);
        player->specialFlags &= ~noBoosting;
        player->specialFlags |= noSpeedLossChargingJump;
        player->gearStats[player->level].airDrain = 13;
    }

    if (player->rings >= 40 && EggInfo->emeraldsInserted == 2 && !(player->specialFlags & ringGear)) // level 3
    {
        player->level = 2;
        if (EggInfo->refillAir == 1)
        {
            player->currentAir = player->gearStats[player->level].maxAir;
            EggInfo->refillAir = 2; 
        }
        player->typeAttributes = PowerType;
        player->gearStats[player->level].baseTopSpeed = pSpeed(200.0f);
        player->gearStats[player->level].topSpeed = pSpeed(200.0f);
        player->gearStats[player->level].boostSpeed = pSpeed(220.0f);
        player->specialFlags |= lightBoardEffect;
        player->gearStats[player->level].airDrain = 13;
    }

    if (player->rings >= 60 && EggInfo->emeraldsInserted == 3 && !(player->specialFlags & ringGear)) // level 4
    {
        player->level = 2;
        player->level4 = 1;
        player->typeAttributes = PowerType;
        player->gearStats[player->level].baseTopSpeed = pSpeed(220.0f);
        player->gearStats[player->level].topSpeed = pSpeed(220.0f);
        player->gearStats[player->level].boostSpeed = pSpeed(240.0f);
        player->gearStats[player->level].airDrain = 25;
        player->specialFlags |= legendEffect;
    }

    if (player->rings >= 90 && EggInfo->emeraldsInserted == 4 && !(player->specialFlags & ringGear)) // level 5
    {
        player->level = 2;
        player->level4 = 1;
        EggInfo->extraLevel = 1;
        player->typeAttributes = PowerType;
        player->gearStats[player->level].baseTopSpeed = pSpeed(230.0f);
        player->gearStats[player->level].topSpeed = pSpeed(230.0f);
        player->gearStats[player->level].boostSpeed = pSpeed(260.0f);
        player->gearStats[player->level].boostCost = (player->gearStats[2].maxAir * 25) / 100;
        player->gearStats[player->level].airDrain = 80;
        if (player->movementFlags & drifting)
        {
            player->speed += pSpeed(0.008f * player->driftDashFrames);
        }
    }

    if (player->rings >= 100 && EggInfo->emeraldsInserted == 5 || player->specialFlags & ringGear) // level 6
    {
        if (EggInfo->magnetShieldEnabled == FALSE)
        {
            lbl_0008CC74(player->index); // spawn magnet
            EggInfo->magnetShieldEnabled = TRUE;
            player->empty[0] = '\1';
        } 
        if (player->statusEffectFlags != 4)
        {
            EggInfo->magnetShieldEnabled = FALSE;
        }
        player->level = 2;
        player->level4 = 1;
        EggInfo->extraLevel = 2;
        player->typeAttributes = AllType;
        player->gearStats[player->level].baseTopSpeed = pSpeed(280.0f);
        player->gearStats[player->level].topSpeed = pSpeed(280.0f);
        player->gearStats[player->level].boostSpeed = pSpeed(280.0f);
        player->gearStats[player->level].airDrain = 200;
        player->gearStats[player->level].driftCost = 20;
        player->gearStats[player->level].boostCost = 0;
        player->gearStats[player->level].tornadoCost = 10000;
        // player->gearStats[2].boostSpeed = pSpeed(280.0f);
        player->specialFlags |= ringGear;
        player->specialFlags |= berserkerEffect;
        player->specialFlags |= noBoosting;
        if (player->movementFlags & drifting)
        {
            player->speed += pSpeed(0.008f * player->driftDashFrames);
        }
    }
}