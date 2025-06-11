#pragma once

#include "riders/gamemode.hpp"
#include "riders/controller.hpp"
#include "riders/object.hpp"
#include "riders/stage.hpp"

struct BannedStages {
	bool heroesStage : 1{};
	bool babylonStage : 1{};
	u8 heroesStage_playerIndex : 1{}; // index of the player that banned the heroes stage (0 or 1)
	u8 babylonStage_playerIndex : 1{};// index of the player that banned the babylon stage (0 or 1)
	u8 : 4;
};

struct BSS_StageBan {
	u8 tournamentRace{};// this is 1 if it's tournament race gamemode
	u8 currentRound{};// starts off with 1 for round 1, so it's easier
	u8 roundState{};  // depicts the state the round is in
	std::array<u8, 2> player_banCount{};
	std::array<u8, 2> player_port{}; // saves the controller port of player 1 and 2 here
	u8 player1Score : 4{};
	u8 player2Score : 4{};
	u32 playerControl{};                       // which player can currently control menus
	std::array<BannedStages, TotalHeroStageCount> bannedStages{};// array for each Heroes stage
    bool isTestLap{};
    bool hasTestLapOccurred{};
};

struct NeutralStages {
	u8 selectedStageSquare;
	u8 selectedSubMenuStage;
};

struct StageSelectObject2 {
    u32 unk0;
    u32 cursorPosition; ///< goes from 0 to 4
	u32 unk8;
    u8 unkC;
	u8 selectedStageSquare; // which stage square portrait is selected
	u8 subMenuControl;
	u8 subMenuStatus;
	fillerData<0x8> filler2;
    u8 currentPage; ///< goes from 0 to 3
    u8 pageTimer;
	u8 selectedSubMenuStage;
	fillerData<0x2> filler3;
	u8 subMenuDelayFrames;
	fillerData<0x2> filler4;
};
static_assert(sizeof(StageSelectObject2) == 0x20);

struct PauseScreenObject1 {
	fillerData<0x2> filler;
	u8 pausedPlayerIndex;// player index of the player who paused
};

struct HeroesStageButton_GraphicalData {
	fillerData<0x20> filler;
	u32 inactiveButtonRGBA_1;
	u32 inactiveButtonRGBA_2;
};

struct BabylonStageButton_GraphicalData {
	fillerData<0x3C> filler;
	u32 inactiveButtonRGBA_1;
	u32 inactiveButtonRGBA_2;
};

struct Controller_ {
	fillerData<0xA> filler;
	u8 status;
	fillerData<0x1> filler2;
};

constexpr std::array<u32, 2> BannedStageGraphicColors = {
        // player 1, player 2
        0x3232FFFF, 0xFFFF32FF
};

constexpr auto NeutralStageAmount = 5;

extern BSS_StageBan bss_StageBans;

ASMDefined void SetGenericPopup2dObjectTbl(void* object1);

struct ActivePad {
    Flag<Buttons> holdButtons;
	Flag<Buttons> toggleButtons;
};
extern ActivePad gsActivePad;

struct TournamentRaceObject1 {
    const void* text;
    fillerData<0x6> filler;
    u16 buttonsPresetID;
    fillerData<0x3> filler2;
    u8 selectedButton;
    fillerData<0x1> filler3;
    u8 totalButtonCount;
    fillerData<0x4> filler4;
    u8 menuState;
    fillerData<0x8> filler5;
    u8 structDeliminator;
};
static_assert(sizeof(TournamentRaceObject1) == 0x20);

struct SaveData {
    char filler[0x6cc];
    RuleSettings ruleSettings;
};
extern SaveData GameData;