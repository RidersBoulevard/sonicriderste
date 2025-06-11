#pragma once

#include "containers/rgba.hpp"

extern RGBA32 AllCharacterHUDColors[];

const void* CustomCodehandler_Retrieve(u32 input);

enum CustomCodehandlerValues {
	CCH_PlayerPtr,
	CCH_GearDataPtr,
	CCH_CharacterDataPtr,
	CCH_CurrentlyPlayingMusicID,
	CCH_RandFunction,
	CCH_InGamePlayerCount,
	CCH_ruleSettings,
	CCH_CurrentStage,
	CCH_CurrentGameMode,
	CCH_IsGamePausedFlag,
	CCH_CharacterHUDColorPtr,
	CCH_DumpFileFunction,
	CCH_DumpPackManFileFunction,
	CCH_UpdatePlayerStateFunction
};