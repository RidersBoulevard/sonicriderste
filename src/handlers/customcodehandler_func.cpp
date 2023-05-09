#include "customcodehandler_func.hpp"
#include "music/custom_music.hpp"
#include "player/SetPlayerState.hpp"
#include "files/filehandler_dat.hpp"

void* CustomCodehandler_Retrieve(u32 input) {
    switch (input) {
        case CCH_PlayerPtr:
            return players.data();
        case CCH_GearDataPtr:
            return Gears.data();
        case CCH_CharacterDataPtr:
            return Characters.data();
        case CCH_CurrentlyPlayingMusicID:
            return &bss_CustomMusicID;
        case CCH_RandFunction:
            return reinterpret_cast<void *>(&lbl_RNG_Number);
        case CCH_InGamePlayerCount:
            return &InGamePlayerCount;
        case CCH_RuleSettings:
            return &RuleSettings;
        case CCH_CurrentStage:
            return &CurrentStage;
        case CCH_CurrentGameMode:
            return &CurrentGameMode;
        case CCH_IsGamePausedFlag:
            return &gu32Game_PauseFlag;
        case CCH_CharacterHUDColorPtr:
            return &AllCharacterHUDColors;
        case CCH_DumpFileFunction:
            return reinterpret_cast<void *>(&DumpFile);
        case CCH_DumpPackManFileFunction:
            return reinterpret_cast<void *>(&DumpPackManFile);
        case CCH_UpdatePlayerStateFunction:
            return reinterpret_cast<void *>(&func_SetPlayerActionType);
        default:
            return nullptr;
    }
}