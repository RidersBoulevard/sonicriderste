#include "dynamo.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

const s32 DynamoLevelCosts[3] = {33000, 66000, 99000};

const s32 DynamoLevelStartingAmounts[3] = {16500, 38500, 71500};

const u32 DynamoLevelRings[3] = {15, 30, 60};

// u32 airTankFrameCounter = 0;

WindmasterInfo PlayerWMInfo[8];

// const f32 WM_RailJumpDelay = toFrames(0.34);

void Player_Dynamo(Player *player) {

    u8 controllerPort = player->input->port;
    EnabledEXLoads exLoads;
    FetchEnabledEXLoadIDs(player, &exLoads);
    
    WindmasterInfo *wmInfo = &PlayerWMInfo[player->index];

    if (exLoads.gearExLoadID != WindmasterJetEXLoad) return;
    
    if (player->currentAir > 0)
    {player->superFormState = 1;} else player->superFormState = 0;

    if (player->state == StartLine) {
        wmInfo->lastShortcutType = 0;
        player->currentAir = 30000;
        wmInfo->FirstRail = FALSE;
        wmInfo->jumpedFromRail = FALSE;
        wmInfo->hasFlown = FALSE;
        wmInfo->qteEntered = FALSE;
    }
    
    player->level = 1;
    player->specialFlags |= (lightBoardEffect | tornadoBoost | ringGear | thirtyPercentAir);
    player->specialFlags &= ~(noSpeedLossChargingJump | iceImmunity);
    player->gearStats[player->level].maxAir = 100000;
    player->gearStats[player->level].baseTopSpeed = pSpeed(230.0f);
    player->gearStats[player->level].airDrain = 0x12;
    player->gearStats[player->level].boostCost = 5000;
    player->gearStats[player->level].driftCost = 0xA;
    player->gearStats[player->level].tornadoCost = 5000;
    player->gearStats[player->level].boostSpeed = pSpeed(240.0f);
    // if (!(player->movementFlags & boosting)) player->gearStats[player->level].baseAccel = (f32)0x3FAED098;
    player->gearStats[player->level].driftDashSpeed = pSpeed(100.0f);

    if (player->input->toggleFaceButtons & (LButton & RButton) && player->state == Cruise)
    {player->gearStats[player->level].boostSpeed = player->speed;}

    if (player->input->toggleFaceButtons & DPadUp && wmInfo->lastShortcutType != 0) 
    {
        PlayAudioFromDAT(Sound::SFX::EmerlLoseType);
        wmInfo->railJumpDelay = 0.0f;
        wmInfo->lastShortcutType = 0;
        wmInfo->FirstRail = FALSE;
        wmInfo->hasFlown = FALSE;
        wmInfo->jumpedFromRail = FALSE;
    }


    switch (wmInfo->lastShortcutType)
    {
        case 0: // round start or DPAD-UP
            player->typeAttributes |= 0x3;
            break;

        case 1: // DS type: RAIL
            player->typeAttributes = FlyType;
            break;
        
        case 2: // DS type: FLY
            player->typeAttributes = SpeedType;
            break;
    }
    

    switch (player->previousState)
    {
        case Fly:
        if (wmInfo->hasFlown == TRUE) wmInfo->lastShortcutType = 2;
        wmInfo->FirstRail = FALSE;
        break;

        case RailGrind:
        if (player->state == Cruise || player->state == Fall) 
        {
            wmInfo->lastShortcutType = 1;
            wmInfo->FirstRail = TRUE;
        } else if (player->state == Jump) wmInfo->jumpedFromRail = TRUE;
        break;
    }
    
    switch (player->state)
    {
        case RailGrind:
        if (wmInfo->lastShortcutType == 2) break;
        break;

        case Fly:
        wmInfo->hasFlown = TRUE;
        if (wmInfo->lastShortcutType == 2) 
        {
            if (wmInfo->FirstRail == TRUE || wmInfo->jumpedFromRail == TRUE) wmInfo->lastShortcutType = 1;
        } else if (wmInfo->lastShortcutType == 1) break;
        break;

        case Jump:
            if (wmInfo->jumpedFromRail == TRUE && player->input->holdFaceButtons & (LButton | RButton)) {
                if (wmInfo->railJumpDelay < 20.0f) {
                    wmInfo->railJumpDelay += 1.0f;
                } else 
                {
                    wmInfo->railJumpDelay = 0.0f;
                    wmInfo->lastShortcutType = 1;
                }   
            }
            break;

        case QTE:
        case QTE2:
        if (player->previousState == Fly) wmInfo->qteEntered = TRUE;
        if (player->previousState == Cruise && wmInfo->hasFlown == TRUE) wmInfo->qteEntered = TRUE;
        break;

        case Cruise:
        if (wmInfo->jumpedFromRail == TRUE) {
            if (player->previousState == Jump || player->previousState == Fall) 
            {
                if (wmInfo->qteEntered == TRUE)
                {
                    wmInfo->lastShortcutType = 2;
                } else wmInfo->lastShortcutType = 1;
            } else if (wmInfo->hasFlown == TRUE) 
            {
                wmInfo->lastShortcutType = 2;
            }
        } else if (wmInfo->qteEntered == TRUE && wmInfo->hasFlown == TRUE) wmInfo->lastShortcutType = 2;

        wmInfo->jumpedFromRail = FALSE;
        wmInfo->hasFlown = FALSE;
        wmInfo->railJumpDelay = 0.0f;
        wmInfo->qteEntered = FALSE;
        break;
    }

    // if (player->state == RailGrind && wmInfo->lastShortcutType == (u8)2) return;
    // if (player->state == RailGrind) wmInfo->lastShortcutType = (u8)1;
    // if (player->state == Fly && wmInfo->lastShortcutType == (u8)1) return;
    // if (player->state == Fly) wmInfo->lastShortcutType = (u8)2;

    // if (player->extremeGear != AirTank) return;
    // if (player->currentAir < 500 && player->rings > 0) {
    //     player->currentAir = 500;
    //     if (player->state == Cruise) {
    //         if (airTankFrameCounter % 30 == 0) {
    //             player->rings -= 1;
    //         }
    //         airTankFrameCounter++;
    //     }
    // }

    // if (player->currentAir >= 60000) {
    //     player->level = 0;
    // } else if (player->currentAir >= 30000 && player->currentAir < 60000) {
    //     player->level = 1;
    // } else if (player->currentAir > 500 && player->currentAir < 30000) player->level = 2;

    // if (player->currentAir < 500 && player->rings > 0) {
    //     player->currentAir = 500;
    //     if (player->state == Cruise) {
    //         if (airTankFrameCounter % 30 == 0) {
    //             player->rings -= 1;
    //         }
    //         airTankFrameCounter++;
    //     }
    // }


    // if (!(player->specialFlags & ringGear)) 
    // {
    //     player->gearStats[player->level].maxAir = player->gearptr->levelStats[player->level].maxAir / 2;
    //     player->gearStats[player->level].airDrain = player->gearptr->levelStats[player->level].passiveAirDrain * 6;
    //     player->gearStats[player->level].driftCost = player->gearptr->levelStats[player->level].driftingAirCost;
    //     player->gearStats[player->level].tornadoCost = player->gearptr->levelStats[player->level].tornadoCost;
    //     player->gearStats[player->level].boostCost = player->gearptr->levelStats[player->level].boostCost;
    //     player->gearStats[player->level].boostSpeed = player->gearptr->levelStats[player->level].boostSpeed;
    // } else 
    // {
    //     player->gearStats[player->level].maxAir = 110000;
    //     player->gearStats[player->level].airDrain = 0x2;
    //     player->gearStats[player->level].driftCost = 0xA;
    //     player->gearStats[player->level].tornadoCost = 5500;
    //     player->gearStats[player->level].boostCost = 16500;
    //     player->gearStats[player->level].boostSpeed = pSpeed(265.0f);
    //     SNDF_SeqIdStop(player->index + 50);
    //     SNDF_SeqIdStop(player->index + 46);
    // }
    
    // if (player->currentAir < 500 && !(player->specialFlags & ringGear)) {
    //     player->specialFlags |= (ringGear | noPits | thirtyPercentAir);
    //     if (player->currentAir < DynamoLevelStartingAmounts[player->level]) player->currentAir = DynamoLevelStartingAmounts[player->level];
    // } else if (player->currentAir >= DynamoLevelCosts[player->level] && player->specialFlags & ringGear) {
    //     player->currentAir = player->gearStats[player->level].maxAir;
    //     player->specialFlags &= ~(ringGear | noPits | thirtyPercentAir);
    // }
}