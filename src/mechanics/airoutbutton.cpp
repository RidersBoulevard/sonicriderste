#include "airoutbutton.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"

#define CHEATCODE_LBUTTONSTART 1
#define CHEATCODE_RBUTTONSTART 2
#define CHEATCODE_NONEBUTTONSTART 0

struct CheatCodeInput PlayerAirOutCheatCode[8];

inline void CheatCodeAdvanceState(struct CheatCodeInput *cheatCodeInfo) {
    cheatCodeInfo->buttonCount += 1;
    cheatCodeInfo->delayFrames = 30;
}

inline void CheatCodeDetermineStartingButton(struct Player *player, struct CheatCodeInput *cheatCodeInfo) {
    if (cheatCodeInfo->startingButton != CHEATCODE_NONEBUTTONSTART) return;

    u8 startingButton = CHEATCODE_NONEBUTTONSTART;

    if (player->input->toggleFaceButtons & LButton) {
        startingButton = CHEATCODE_LBUTTONSTART;
    }
    else if (player->input->toggleFaceButtons & RButton) {
        startingButton = CHEATCODE_RBUTTONSTART;
    }

    if (startingButton != CHEATCODE_NONEBUTTONSTART) {
        cheatCodeInfo->startingButton = startingButton;
        CheatCodeAdvanceState(cheatCodeInfo);
    }
}

/*
void Player_AirOutButton(struct Player *player) {
    // L + R + any of the 3 face buttons in quick succession

    if (!DebugMenu_CheckOption(ExtremeDetach)) return;
    if (player->flags & InAPit || player->state == QTE || player->state == RailGrind || player->state == Fly) return;
    if (player->extremeGear == AdvantageS || player->extremeGear == Beginner) return;

    struct CheatCodeInput *cheatCodeInfo = &PlayerAirOutCheatCode[player->index];

    if (cheatCodeInfo->delayFrames > 0) {
        cheatCodeInfo->delayFrames -= 1;
    } else {
        cheatCodeInfo->buttonCount = 0;
        cheatCodeInfo->startingButton = CHEATCODE_NONEBUTTONSTART;
    }

    switch (cheatCodeInfo->buttonCount) {
        case 0:
            CheatCodeDetermineStartingButton(player, cheatCodeInfo);
            break;
        case 1:
            switch (cheatCodeInfo->startingButton) {
                case CHEATCODE_LBUTTONSTART:
                    if (player->input->toggleFaceButtons & RButton) {
                        CheatCodeAdvanceState(cheatCodeInfo);
                    }
                    break;
                case CHEATCODE_RBUTTONSTART:
                    if (player->input->toggleFaceButtons & LButton) {
                        CheatCodeAdvanceState(cheatCodeInfo);
                    }
                    break;
            }
            break;
        default:
            if (player->input->toggleFaceButtons & (AButton | BButton | XButton | YButton)) {
                CheatCodeAdvanceState(cheatCodeInfo);
            }
            break;
    }

    if (cheatCodeInfo->buttonCount >= 5) {
        // air out player, reset
        cheatCodeInfo->buttonCount = 0;
        cheatCodeInfo->delayFrames = 0;
        player->currentAir = 0;
    }
}
*/

void Player_AirOutButton(struct Player *player) {
    // L or R + Z + C stick up

    if (!DebugMenu_CheckOption(ExtremeDetach)) return;
    if (player->flags & InAPit || player->state == QTE || player->state == RailGrind || player->state == Fly) return;
    if (player->extremeGear == AdvantageS || player->extremeGear == Beginner) return;

    u32 holdButtons = player->input->holdFaceButtons;

    if ((holdButtons & (LButton | RButton)) && (holdButtons & ZButton) && (player->input->rightStickVertical > 80)) {
        player->currentAir = 0;
    }
}