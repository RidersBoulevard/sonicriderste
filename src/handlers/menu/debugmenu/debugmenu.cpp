#include "debugmenu.hpp"
#include "lib/sound.hpp"
#include "handlers/music/custom_music.hpp"
#include "lib/lib.hpp"
#include "handlers/ingame/customtext.hpp"

#define SFX_MENUSCROLL 0xA9000000
#define SFX_MENUCONFIRM 0xA9000100
#define SFX_MENUBACK 0xA9000200

#define HISTORY_MEGACOLLECTION_ID 15

#define DISABLEMUSIC_OPTIONCOUNT 2
#define MAGNETICIMPULSE_OPTIONCOUNT 2
#define AUTOPILOT_OPTIONCOUNT 2
#define EXTREMEDETACH_OPTIONCOUNT 2
#define TORNADOIGNORE_OPTIONCOUNT 2
#define TIMERACTIVITY_OPTIONCOUNT 5

extern u32 lbl_001F1D80[];

u8 DebugMenu_DisableMusicOptions[DISABLEMUSIC_OPTIONCOUNT] = {DisableMusic, 0xFF};
u8 DebugMenu_MagneticImpulseOptions[MAGNETICIMPULSE_OPTIONCOUNT] = {MagneticImpulse, 0xFF};
u8 DebugMenu_AutoPilotOptions[AUTOPILOT_OPTIONCOUNT] = {Autopilot, 0xFF};
u8 DebugMenu_ExtremeDetachOptions[EXTREMEDETACH_OPTIONCOUNT] = {ExtremeDetach, 0xFF};
u8 DebugMenu_TornadoIgnoreOptions[TORNADOIGNORE_OPTIONCOUNT] = {TornadoIgnore, 0xFF};
u8 DebugMenu_TimerActivityOptions[TIMERACTIVITY_OPTIONCOUNT] = {
TimerActivity_ActiveInSingleplayer,
TimerActivity_ActiveIn1v1,
TimerActivity_ActiveIn1v1Middle,
TimerActivity_ActiveIn3OrMorePlayers,
0xFF
};

Text2dFileData DebugMenu_TextData;
DebugMenuData DebugMenu_Data = {
    0,
    0,
    0,
    0,

    {
            {DISABLEMUSIC_OPTIONCOUNT, DebugMenu_DisableMusicOptions},
            {MAGNETICIMPULSE_OPTIONCOUNT, DebugMenu_MagneticImpulseOptions},
            {AUTOPILOT_OPTIONCOUNT, DebugMenu_AutoPilotOptions},
            {EXTREMEDETACH_OPTIONCOUNT, DebugMenu_ExtremeDetachOptions},
            {TORNADOIGNORE_OPTIONCOUNT, DebugMenu_TornadoIgnoreOptions},
            {TIMERACTIVITY_OPTIONCOUNT, DebugMenu_TimerActivityOptions}
        },
    0
};

bool DebugMenu_CheckOption(u32 option) {
    return (DebugMenu_Data.toggledPageOptions & 1 << option) ? TRUE : FALSE;
}

void DebugMenu_ToggleOption(DebugOptions *options) {
    DebugMenu_Data.toggledPageOptions ^= 1 << options->options[0];
}

void DebugMenu_ToggleOptionSet(DebugOptions *options, u32 direction) {
    s32 toggledOptionIndex = -1;
    u32 i;

    for (i = 0; i < options->optionCount - 1; i++) {
        if (DebugMenu_CheckOption(options->options[i])) {
            toggledOptionIndex = i;
        }

        DebugMenu_Data.toggledPageOptions &= ~(1 << options->options[i]);
    }

    if (toggledOptionIndex != -1) {
        if (direction == 1) {
            // user pressed right
            toggledOptionIndex += 1;
            if (toggledOptionIndex >= options->optionCount)
                toggledOptionIndex = 0;
        } else {
            // user pressed left
            toggledOptionIndex -= 1;
            if (toggledOptionIndex < 0)
                toggledOptionIndex = -1;
        }
    } else {
        if (direction == 1) {
            // user pressed right whilst being on OFF
            toggledOptionIndex = 0;
        } else {
            // user pressed left whilst being on OFF
            toggledOptionIndex = options->optionCount - 2;
        }
    }

    if (toggledOptionIndex != -1) {
        DebugMenu_Data.toggledPageOptions |= 1 << options->options[toggledOptionIndex];
    }
}

void DebugMenu_HandleAllToggles(DebugOptions *options, u32 direction) {
    if (options->optionCount == 2) {
        // simple on/off toggle
        DebugMenu_ToggleOption(options);
    } else {
        // more toggles than on/off
        DebugMenu_ToggleOptionSet(options, direction);
    }
}

u32 DebugMenu_FetchTextIDAllToggles(DebugOptions *options) {
    u32 textID;
    s32 currentOption;

    if (options->optionCount == 2) {
        // simple on/off toggle
        textID = DebugMenu_CheckOption(options->options[0]);
    } else {
        // more toggles than on/off
        currentOption = DebugMenu_FetchOptionFromOptionSet(options);
        if (currentOption == -1) textID = 0;
        else {
            textID = DebugMenu_FetchTextID(currentOption);
        }
    }

    return textID;
}

s32 DebugMenu_FetchOptionFromOptionSet(DebugOptions *options) {
    u32 i;
    s32 option = -1;

    for (i = 0; i < options->optionCount - 1; i++) {
        if (DebugMenu_CheckOption(options->options[i])) {
            option = options->options[i];
            break;
        }
    }

    return option;
}

u32 DebugMenu_FetchTextID(u32 option) {
    switch (option) {
        case TimerActivity_ActiveInSingleplayer:
            return 2;
        case TimerActivity_ActiveIn1v1:
            return 3;
        case TimerActivity_ActiveIn1v1Middle:
            return 4;
        case TimerActivity_ActiveIn3OrMorePlayers:
            return 5;
        default:
            return 0;
    }
}

void DebugMenu_RenderDescription() {
    void *textTexture = gpasTexList_SubFont;
    u32 x, xDiff, textCoords;

    // render description in menu
    textCoords = CenterText(540, 0, lbl_001F1D80[6], &gasSubFont->textDataHeader);
    xDiff = textCoords >> 17;
    x = 320 - xDiff;
    RenderText(x, 374, ~0U, 540, 0, lbl_001F1D80[6], &gasSubFont->textDataHeader, textTexture, -100.0f);
}

inline void DebugMenu_HandleInputs(struct AllPlayerInputs *inputs, DebugMenuData *debugMenu) {
    u32 selectedItemRow;

    if (inputs->toggleButtons & (LStickDown | DPadDown)) {
        PlayAudioFromDAT(SFX_MENUSCROLL);

        selectedItemRow = debugMenu->selectedItemRow + 1;
        if (selectedItemRow >= debugMenu->maximumItems)
            selectedItemRow = 0;

        debugMenu->selectedItemRow = selectedItemRow;
    }

    if (inputs->toggleButtons & (LStickUp | DPadUp)) {
        PlayAudioFromDAT(SFX_MENUSCROLL);

        if (debugMenu->selectedItemRow > 0) debugMenu->selectedItemRow -= 1;
        else debugMenu->selectedItemRow = debugMenu->maximumItems - 1;
    }

    if (inputs->toggleButtons & (AButton | LStickRight | DPadRight)) {
        DebugOptions options = debugMenu->page1Options[debugMenu->selectedItemRow];
        PlayAudioFromDAT(SFX_MENUCONFIRM);

        DebugMenu_HandleAllToggles(&options, 1);
    } else if (inputs->toggleButtons & (LStickLeft | DPadLeft)) {
        DebugOptions options = debugMenu->page1Options[debugMenu->selectedItemRow];
        PlayAudioFromDAT(SFX_MENUCONFIRM);

        DebugMenu_HandleAllToggles(&options, 0);
    }
}

void DebugMenu_Handler(struct Object *object, struct AllPlayerInputs *inputs) {
    auto *header = static_cast<struct Text2dFileHeader*>(DebugMenu_TextData.textData[0]);
    DebugMenuData *debugMenu = &DebugMenu_Data;

    switch (debugMenu->state) {
        case 0:
            debugMenu->selectedItemRow = 0;
            debugMenu->selectedItemColumn = 0;
            debugMenu->maximumItems = header->textCount;
            debugMenu->state += 1;

            if (bss_CustomMusicID != HISTORY_MEGACOLLECTION_ID) {
                PlayADX(gpasAdxtHandle_Bgm, menuMusic[HISTORY_MEGACOLLECTION_ID]);
            }

            break;
        case 1:
            if (inputs->toggleButtons & BButton) {
                auto *object1 = static_cast<struct TitleSequenceObject1*>(object->object);
                PlayAudioFromDAT(SFX_MENUBACK);
                object->state = 0x20;
                object1->currentButtonIndex = 0x6;
                debugMenu->state = 0;

                if (bss_CustomMusicID < MENU_MUSIC_COUNT) {
                    PlayADX(gpasAdxtHandle_Bgm, menuMusic[bss_CustomMusicID]);
                } else {
                    PlayADX(gpasAdxtHandle_Bgm, lbl_001E99BC[19]);
                }
                return;
            }

            DebugMenu_HandleInputs(inputs, debugMenu);

            break;
    }
}