#pragma once
#include "main.hpp"

struct BannedStages {
	BOOL heroesStage : 1;
	BOOL babylonStage : 1;
	u8 heroesStage_playerIndex : 1; // index of the player that banned the heroes stage (0 or 1)
	u8 babylonStage_playerIndex : 1; // index of the player that banned the babylon stage (0 or 1)
};

struct BSS_StageBan {
	u8 tournamentRace; // this is 1 if it's tournament race gamemode
	u8 player1_banCount;
	u8 player2_banCount;
	u8 currentRound; // starts off with 1 for round 1, so it's easier
	u8 roundState; // depicts the state the round is in
	u8 player1Port; // saves the controller port of player 1 here
	u8 player2Port; // saves the controller port of player 2 here
	u8 player1Score : 4;
	u8 player2Score : 4;
	u32 playerControl; // which player can currently control menus
	BannedStages bannedStages[8]; // array for each Heroes stage
};

struct NeutralStages {
	u8 selectedStageSquare;
	u8 selectedSubMenuStage;
};

struct StageSelectObject2 {
	char filler[0xD];
	u8 selectedStageSquare; // which stage square portrait is selected
	u8 subMenuControl;
	u8 subMenuStatus;
	char filler2[0xA];
	u8 selectedSubMenuStage;
	char filler3[0x2];
	u8 subMenuDelayFrames;
};

struct PauseScreenObject1 {
	char filler[0x2];
	u8 pausedPlayerIndex; // player index of the player who paused
};

struct HeroesStageButton_GraphicalData {
	char filler[0x20];
	u32 inactiveButtonRGBA_1;
	u32 inactiveButtonRGBA_2;
};

struct BabylonStageButton_GraphicalData {
	char filler[0x3C];
	u32 inactiveButtonRGBA_1;
	u32 inactiveButtonRGBA_2;
};

struct Controller_ {
	char filler[0xA];
	u8 status;
	char filler2[0x1];
};

constexpr u32 BannedStageGraphicColors[2] = {
		// player 1, player 2
		0x3232FFFF, 0xFFFF32FF
};

#define NeutralStageAmount 5

extern BSS_StageBan bss_StageBans;
extern u32 bss_StageBans_isBanned;