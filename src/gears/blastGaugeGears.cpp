#include "blastGaugeGears.hpp"
#include "cosmetics/player/exloads.hpp"
#include "riders/gamemode.hpp"

std::array<BlastGaugeInfo, MaxPlayerCount> PlayerBlastGaugeInfo;

ASMUsed void Player_checkGearIfUsesBlastGauge(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	if(CurrentGameMode != FreeRace &&
	   CurrentGameMode != WorldGrandPrix &&
	   CurrentGameMode != StoryMode &&
	   CurrentGameMode != TimeTrial) { return; }
	if(player->specialFlags.hasAny(ringGear)) {
		switch(player->extremeGear) {
			using namespace ExtremeGear;
			case ChaosEmerald:
				if(player->character == SuperSonic) {
					player->isBlastGaugeGear = TRUE;
				}
				break;

			default:
				player->isBlastGaugeGear = FALSE;
				break;
		}
	}
}

ASMUsed void Player_resetGauge(Player *player) {
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
	if(player->state == StartLine && player->isBlastGaugeGear) {
		bgInfo->currentGauge = 0;
		// player->shortcutAirGainMultiplier = 0;
		return;
	}
}

ASMUsed s32 Player_blastGaugeHandler(Player *player)// TODO: remove or place somewhere else, now exists in lcanceling.cpp
{
	// if (player->isBlastGaugeGear == TRUE)
	// {
	//     player->shortcutAirGainMultiplier += (s32)player->trickCount * 10000;
	//     if (player->shortcutAirGainMultiplier >= 100000) player->shortcutAirGainMultiplier = 100000;
	// }
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
	return bgInfo->currentGauge;
}

#include "macros.h"
ASMUsed void Player_BlastGaugeAirTask() {
	asm volatile(
	"   stwu r1, -0x90 (r1)\n"
	"   mflr r0\n"
	"   stmw r3, 0x8 (r1)\n"
	"   stw r0, 0x94 (r1)\n"
	"   mr r31, r7\n"

	"   mr r3, r7\n"
	"   bl Player_checkGearIfUsesBlastGauge\n"
	"   lbz r3, 0x107D(r31)\n"
	"   cmpwi r3, 1\n"// if true, gear is a blast gear. Draw gauge.
	"   bne lbl_airGearDrawAirGauge\n"

	"   mr r3, r31\n"
	"   bl Player_resetGauge\n"
	"   bl Player_blastGaugeHandler\n"
	"   mr r0, r3\n"
	/*
    "   lwz r0, 0x9CC(r31)\n"
    "   lbz r0, 0x109(r7)\n" // unk0x106D
    "   lis r12, 0x0000\n"
    "   ori r12, r12, 0x1500\n"
    "   mullw r0, r0, r12\n"
    */

	"   b lbl_drawDynamicGauge\n"

	"lbl_airGearDrawAirGauge:\n"
	"   lwz r0, 0x984(r31)\n"

	"lbl_drawDynamicGauge:\n"

	"   lwz r3, 0x94 (r1)\n"
	"   mtlr r3\n"
	"   lmw r3, 0x8 (r1)\n"
	"   addi r1, r1, 0x90\n"
	);
}