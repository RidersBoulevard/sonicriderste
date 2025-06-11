#include "airoutbutton.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"
#include "cosmetics/player/exloads.hpp"
#include "gears/hypersonic.hpp"
#include "gears/blastGaugeGears.hpp"

constexpr auto CHEATCODE_LBUTTONSTART = 1;
constexpr auto CHEATCODE_RBUTTONSTART = 2;
constexpr auto CHEATCODE_NONEBUTTONSTART = 0;

std::array<CheatCodeInput, MaxPlayerCount> PlayerAirOutCheatCode;

inline void CheatCodeAdvanceState(CheatCodeInput *cheatCodeInfo) {
	cheatCodeInfo->buttonCount += 1;
	cheatCodeInfo->delayFrames = 30;
}

void CheatCodeDetermineStartingButton(Player *player, CheatCodeInput *cheatCodeInfo) {
	if(cheatCodeInfo->startingButton != CHEATCODE_NONEBUTTONSTART) { return; }

	u8 startingButton = CHEATCODE_NONEBUTTONSTART;

	if(player->input->toggleFaceButtons.hasAny(Buttons::L)) {
		startingButton = CHEATCODE_LBUTTONSTART;
	} else if(player->input->toggleFaceButtons.hasAny(Buttons::R)) {
		startingButton = CHEATCODE_RBUTTONSTART;
	}

	if(startingButton != CHEATCODE_NONEBUTTONSTART) {
		cheatCodeInfo->startingButton = startingButton;
		CheatCodeAdvanceState(cheatCodeInfo);
	}
}

/*
void Player_AirOutButton(Player *player) {
    // L + R + any of the 3 face buttons in quick succession

    if (!DebugMenu_CheckOption(ExtremeDetach)) return;
    if (player->flags & InAPit || player->state == QTE || player->state == RailGrind || player->state == Fly) return;
    if (player->extremeGear == AdvantageS || player->extremeGear == Beginner) return;

    CheatCodeInput *cheatCodeInfo = &PlayerAirOutCheatCode[player->index];

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

void Player_AirOutButton(Player *player) {
	// L or R + Z + C stick up
	if(!DebugMenu_CheckOption(DebugMenuOptions::ExtremeDetach)) { return; }
	if(player->flags.hasAny(PlayerFlags::InAPit) ||
	   player->state == PlayerState::QTE ||
	   player->state == PlayerState::RailGrind ||
	   player->state == PlayerState::Fly) {
		return;
	}
    HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];
	if(player->extremeGear == ExtremeGear::AdvantageS ||
	   player->extremeGear == ExtremeGear::Beginner ||
       (player->character == Character::SuperSonic && player->gearExload().exLoadID == EXLoad::HyperSonic
       && hsInfo->hyperdriveEnabled)) {
		return;
	}

	const auto &holdButtons = player->input->holdFaceButtons;
	if(holdButtons.hasAny(Buttons::L, Buttons::R) && holdButtons.hasAny(Buttons::Z) && player->input->rightStickVertical > 80) {
		player->currentAir = 0;
	}
}