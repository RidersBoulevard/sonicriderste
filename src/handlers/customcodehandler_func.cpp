#include "customcodehandler_func.hpp"
#include "files/filehandler_dat.hpp"
#include "lib/stdlib.hpp"
#include "main.hpp"
#include "music/custom_music.hpp"
#include "player/SetPlayerState.hpp"
#include "riders/gamemode.hpp"
#include "riders/stage.hpp"

const void* CustomCodehandler_Retrieve(u32 input) {
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
            return (const void *) &InGamePlayerCount; // Todo: Figure out what C++ cast is needed to do this
        case CCH_RuleSettings:
            return (const void *) &RuleSettings; // Todo: Figure out what C++ cast is needed to do this
        case CCH_CurrentStage:
            return (const void *) &CurrentStage; // Todo: Figure out what C++ cast is needed to do this
        case CCH_CurrentGameMode:
            return (const void *) &CurrentGameMode; // Todo: Figure out what C++ cast is needed to do this
        case CCH_IsGamePausedFlag:
            return (const void *) &gu32Game_PauseFlag; // Todo: Figure out what C++ cast is needed to do this
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