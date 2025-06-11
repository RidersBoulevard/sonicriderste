/** @file gamemode.hpp
 * Header file for dealing with anything related to gamemodes.
 */

#pragma once

#include "../globalDefs.hpp"
#include "../types.hpp"

enum GameModes : u32 {
    TitleScreen    = 0x1,
    StoryMode      = 100,
    MissionMode    = 200,
    TagMode        = 300,
    EmeraldChase   = 400,
    BattleMode     = 500,
    TimeTrial      = 600,
    FreeRace       = 700,
    WorldGrandPrix = 800,
    UnkMode        = 900, // Only god knows, maybe a menu or something
    CutsceneMode   = 1000
};

enum class ExitMethod : u32 {
    Retry = 2,
};

enum class RaceState { Init, OpeningCutscene, Countdown, Active, End };

ASMDefined const volatile GameModes CurrentGameMode;
ASMDefined volatile u32 geGame_ModeDetail;       ///< Variable that determines sort of the gamemode's "state" in a weird way.
ASMDefined const volatile u32 gu32EndOfGameFlag; // aka RaceExitMethod

struct RuleSettings {
    bool unk31 : 1;     // 31
    bool unk30 : 1;     // 30
    bool unk29 : 1;     // 29
    bool unk28 : 1;     // 28
    bool unk27 : 1;     // 27
    bool announcer : 1; // 26
    bool unk25 : 1;     // 25
    bool unk24 : 1;     // 24

    bool unk23 : 1;          // 23
    bool unk22 : 1;          // 22
    bool cpu_racers : 1;     // 21
    bool unk20 : 1;          // 20
    bool disable_retire : 1; // 19
    bool unk18 : 1;          // 18
    bool unk17 : 1;          // 17
    bool unk16 : 1;          // 16

    bool unk15 : 1;           // 15
    bool unk14 : 1;           // 14
    bool unk13 : 1;           // 13
    bool unk12 : 1;           // 12
    bool unk11 : 1;           // 11
    bool air_pits : 1;        // 10
    bool items : 1;           // 9
    bool japanese_voices : 1; // 8

    bool unk7 : 1;  // 7
    u8 max_lap : 7; // 0-6
};
static_assert(sizeof(RuleSettings) == sizeof(u32));

ASMDefined RuleSettings ruleSettings;

[[nodiscard]] inline bool isInGame() {
    const auto currentGameMode = CurrentGameMode;
    switch (currentGameMode) {
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

[[nodiscard]] inline bool isRetry() {
    return gu32EndOfGameFlag == std::to_underlying(ExitMethod::Retry);
}

[[nodiscard]] inline RaceState getRaceState() {
    return static_cast<RaceState>(geGame_ModeDetail - CurrentGameMode);
}

