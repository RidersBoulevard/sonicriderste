/** @file gamemode.hpp
 * Header file for dealing with anything related to gamemodes.
 */

#pragma once

#include "../globalDefs.hpp"
#include "../types.hpp"

enum GameModes : u32 {
	TitleScreen = 0x1,
	StoryMode = 100,
	MissionMode = 200,
	TagMode = 300,
	EmeraldChase = 400,
	BattleMode = 500,
	TimeTrial = 600,
	FreeRace = 700,
	WorldGrandPrix = 800,
	UnkMode = 900, // Only god knows, maybe a menu or something
	CutsceneMode = 1000
};

ASMDefined const volatile GameModes CurrentGameMode;
ASMDefined const volatile u32 geGame_ModeDetail; ///< Variable that determines sort of the gamemode's "state" in a weird way.
ASMDefined const volatile u32 RaceExitMethod;

[[nodiscard]] inline bool isInGame() {
	const auto currentGameMode = CurrentGameMode;
	switch(currentGameMode) {
		case StoryMode:
		case MissionMode:
		case TagMode:
		case EmeraldChase:
		case BattleMode:
		case TimeTrial:
		case FreeRace:
		case UnkMode:
		case CutsceneMode:
		case WorldGrandPrix: return true;
		default: return false;
	}
}