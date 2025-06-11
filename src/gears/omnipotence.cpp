#include "omnipotence.hpp"

OmnipotenceInfo PlayerOMNInfo[8];

void Player_Omnipotence(Player *player) {
    if(player->extremeGear != ExtremeGear::Omnipotence) return;
    OmnipotenceInfo *OMNInfo = &PlayerOMNInfo[player->index];

    if (player->state == PlayerState::StartLine) {
        OMNInfo->lastShortcutType = 0;
        OMNInfo->FirstRail = FALSE;
        OMNInfo->jumpedFromRail = FALSE;
        OMNInfo->hasFlown = FALSE;
        OMNInfo->qteEntered = FALSE;
        OMNInfo->firstPowerObj = FALSE;
    }
    
    if(player->character == Character::E10G) return;

    

    // if(player->state == RailGrind
	//    || player->state == Fly
	//    || player->unkD90 >= 1) {
	// 	player->omnipotence_Timer = 180;
	// } else if (player->omnipotence_Timer != 0 
    //             && (player->state == Cruise || player->state == Run)
    //             && !player->movementFlags.hasAny(boosting)) {player->omnipotence_Timer -= 1;}

    if (player->unkD90 != 0 && (player->state == PlayerState::Cruise)) {
        OMNInfo->firstPowerObj = TRUE;
    } else if (OMNInfo->firstPowerObj == true && (player->state != PlayerState::Cruise || player->unkD90 == 0)) {
        OMNInfo->lastShortcutType = 3;
        OMNInfo->firstPowerObj = FALSE;
    }

    // If power link timer is written and not grinding or flying, you took power type shortcut last.
    // if (player->unkD90 > 0 && (player->state != RailGrind || player->state != Fly)) {
        // if (player->omnipotence_Timer == 0) OMNInfo->lastShortcutType = 3;
    else {
        switch (player->previousState)
        {
            case PlayerState::Fly:
            if (OMNInfo->hasFlown == TRUE) OMNInfo->lastShortcutType = 2;
            OMNInfo->FirstRail = FALSE;
            break;

            case PlayerState::RailGrind:
            if (player->state == PlayerState::Cruise || player->state == PlayerState::Fall) 
            {
                OMNInfo->lastShortcutType = 1;
                OMNInfo->FirstRail = TRUE;
            } else if (player->state == PlayerState::Jump) OMNInfo->jumpedFromRail = TRUE;
            break;
        }
        
        switch (player->state)
        {
            case PlayerState::RailGrind:
            if (OMNInfo->lastShortcutType == 2) break;
            break;

            case PlayerState::Fly:
            OMNInfo->hasFlown = TRUE;
            if (OMNInfo->lastShortcutType == 2) 
            {
                if (OMNInfo->FirstRail == TRUE || OMNInfo->jumpedFromRail == TRUE) OMNInfo->lastShortcutType = 1;
            } else if (OMNInfo->lastShortcutType == 1) break;
            break;

            // Was used to switch types for WMJ
            // case Jump:
                // if (OMNInfo->jumpedFromRail == TRUE && player->input->holdFaceButtons & (AButton)) {
                //     if (OMNInfo->railJumpDelay < 20.0f) {
                //         OMNInfo->railJumpDelay += 1.0f;
                //     } else 
                //     {
                //         OMNInfo->railJumpDelay = 0.0f;
                //         OMNInfo->lastShortcutType = 1;
                //     }   
                // }
                // break;

            case PlayerState::QTE:
            case PlayerState::QTE2:
            if (player->previousState == PlayerState::Fly) OMNInfo->qteEntered = TRUE;
            if (player->previousState == PlayerState::Cruise && OMNInfo->hasFlown == TRUE) OMNInfo->qteEntered = TRUE;
            if (OMNInfo->jumpedFromRail == TRUE) OMNInfo->lastShortcutType = 1;
            break;

            case PlayerState::Cruise:
            if (OMNInfo->jumpedFromRail == TRUE) {
                if (player->previousState == PlayerState::Jump || player->previousState == PlayerState::Fall) 
                {
                    if (OMNInfo->qteEntered == TRUE)
                    {
                        OMNInfo->lastShortcutType = 2;
                    } 
                    else OMNInfo->lastShortcutType = 1;
                } else if (OMNInfo->hasFlown == TRUE) 
                {
                    OMNInfo->lastShortcutType = 2;
                }
            } else if (OMNInfo->qteEntered == TRUE && OMNInfo->hasFlown == TRUE) OMNInfo->lastShortcutType = 2;

            OMNInfo->jumpedFromRail = FALSE;
            OMNInfo->hasFlown = FALSE;
            // OMNInfo->railJumpDelay = 0.0f;
            OMNInfo->qteEntered = FALSE;
            break;
        }
    }

}