#pragma once

#include "context.hpp"

enum DebugMenuOptionsPage1 {
    DisableMusic,
    MagneticImpulse,
    Autopilot,
    ExtremeDetach,
    TornadoIgnore,
    TimerActivity_ActiveInSingleplayer,
    TimerActivity_ActiveIn1v1,
    TimerActivity_ActiveIn1v1Middle,
    TimerActivity_ActiveIn3OrMorePlayers,
};
#define DEBUGMENU_DEFAULTPAGE1OPTIONS \
((1 << MagneticImpulse) | \
(1 << Autopilot) | \
(1 << TornadoIgnore) | \
(1 << TimerActivity_ActiveInSingleplayer) | \
(1 << ExtremeDetach))

typedef struct DebugOptions {
    u32 optionCount;
    u8 *options;
} DebugOptions;

typedef struct DebugMenuData {
    u8 state;
    u8 selectedItemRow;
    u8 selectedItemColumn;
    u8 maximumItems;

    DebugOptions page1Options[6];

    // bit field that corresponds to enum DebugMenuOptions
    u32 toggledPageOptions;
} DebugMenuData;

typedef struct Text2dFileData {
    void *textData[2];
    void *extraTextData[2];
} Text2dFileData;

struct AllPlayerInputs {
    u32 holdButtons;
    u32 toggleButtons;
};

struct TitleSequenceObject1 {
    char filler[0xD];
    s8 currentButtonIndex;
    s8 lastButtonIndex;
    s8 currentMode;
};

struct Text2dFileHeader {
    u16 width;
    u16 height;
    u32 unknown;
    u32 textCount;
};

extern Text2dFileData DebugMenu_TextData;
extern DebugMenuData DebugMenu_Data;

bool DebugMenu_CheckOption(u32 option);
void DebugMenu_ToggleOption(DebugOptions *options);
void DebugMenu_ToggleOptionSet(DebugOptions *options, u32 direction);
void DebugMenu_HandleAllToggles(DebugOptions *options, u32 direction);
u32 DebugMenu_FetchTextIDAllToggles(DebugOptions *options);
extern "C" void DebugMenu_Handler(struct Object *object, struct AllPlayerInputs *inputs);
s32 DebugMenu_FetchOptionFromOptionSet(DebugOptions *options);
u32 DebugMenu_FetchTextID(u32 option);
extern "C" void DebugMenu_RenderDescription();
