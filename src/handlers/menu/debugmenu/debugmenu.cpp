#include "debugmenu.hpp"
#include <cstdlib>
#include "handlers/ingame/customtext.hpp"
#include "handlers/music/custom_music.hpp"
#include "lib/lib.hpp"
#include "lib/sound.hpp"
#include "main.hpp"
#include "riders/controller.hpp"

constexpr auto HISTORY_MEGACOLLECTION_ID = 15;
constexpr auto MAX_ITEM_AMOUNT_PAGE      = 13;

constexpr auto DISABLEMUSIC_OPTIONCOUNT    = 2;
constexpr auto MAGNETICIMPULSE_OPTIONCOUNT = 2;
constexpr auto AUTOPILOT_OPTIONCOUNT       = 2;
constexpr auto EXTREMEDETACH_OPTIONCOUNT   = 2;
constexpr auto TORNADOIGNORE_OPTIONCOUNT   = 2;
constexpr auto TIMERACTIVITY_OPTIONCOUNT   = 5;
constexpr auto DISABLEHUD_OPTIONCOUNT      = 3;
constexpr auto INFINITEAIR_OPTIONCOUNT     = 2;
constexpr auto INFINITERINGS_OPTIONCOUNT   = 2;
constexpr auto MAXMI_OPTIONCOUNT           = 2;
constexpr auto MUSICPLAYLIST_OPTIONCOUNT   = 2;
constexpr auto CLUTCHHUD_OPTIONCOUNT       = 3;
constexpr auto PTRMODE_OPTIONCOUNT         = 2;
constexpr auto VANILLAHAZARDS_OPTIONCOUNT  = 4;
constexpr auto DISABLEATTACKS_OPTIONCOUNT  = 3;
constexpr auto DISABLEFOG_OPTIONCOUNT      = 2;

extern u32 lbl_001F1D80[];

std::array<u8, DISABLEMUSIC_OPTIONCOUNT> DebugMenu_DisableMusicOptions       = {DebugMenuOptions::DisableMusic, 0xFF};
std::array<u8, MAGNETICIMPULSE_OPTIONCOUNT> DebugMenu_MagneticImpulseOptions = {DebugMenuOptions::MagneticImpulse, 0xFF};
std::array<u8, AUTOPILOT_OPTIONCOUNT> DebugMenu_AutoPilotOptions             = {DebugMenuOptions::Autopilot, 0xFF};
std::array<u8, EXTREMEDETACH_OPTIONCOUNT> DebugMenu_ExtremeDetachOptions     = {DebugMenuOptions::ExtremeDetach, 0xFF};
std::array<u8, TORNADOIGNORE_OPTIONCOUNT> DebugMenu_TornadoIgnoreOptions     = {DebugMenuOptions::TornadoIgnore, 0xFF};
std::array<u8, TIMERACTIVITY_OPTIONCOUNT> DebugMenu_TimerActivityOptions     = {
        DebugMenuOptions::TimerActivity_ActiveInSingleplayer,
        DebugMenuOptions::TimerActivity_ActiveIn1v1,
        DebugMenuOptions::TimerActivity_ActiveIn1v1Middle,
        DebugMenuOptions::TimerActivity_ActiveIn3OrMorePlayers,
        0xFF};
std::array<u8, DISABLEHUD_OPTIONCOUNT> DebugMenu_DisableHUDOptions       = {DebugMenuOptions::DisableHUDPartial, DebugMenuOptions::DisableHUDFull, 0xFF};
std::array<u8, INFINITEAIR_OPTIONCOUNT> DebugMenu_InfiniteAirOptions     = {DebugMenuOptions::InfiniteAir, 0xFF};
std::array<u8, INFINITERINGS_OPTIONCOUNT> DebugMenu_InfiniteRingsOptions = {DebugMenuOptions::InfiniteRings, 0xFF};
std::array<u8, MAXMI_OPTIONCOUNT> DebugMenu_MaxMIOptions                 = {DebugMenuOptions::AlwaysMaxMI, 0xFF};
std::array<u8, MUSICPLAYLIST_OPTIONCOUNT> DebugMenu_MusicPlaylistOptions = {DebugMenuOptions::MusicPlaylist, 0xFF};
std::array<u8, CLUTCHHUD_OPTIONCOUNT> DebugMenu_ClutchHUDOptions         = {DebugMenuOptions::ClutchAsAirGauge, DebugMenuOptions::ClutchAsTexture, 0xFF};
std::array<u8, PTRMODE_OPTIONCOUNT> DebugMenu_PTRModeOptions             = {DebugMenuOptions::PTRMode, 0xFF};
std::array<u8, VANILLAHAZARDS_OPTIONCOUNT> DebugMenu_VanillaHazardsOptions =
        {DebugMenuOptions::VanillaHazardsOn, DebugMenuOptions::VanillaHazardsSkyRoadTurb, DebugMenuOptions::VanillaHazardsDigiHands, 0xFF};
std::array<u8, DISABLEATTACKS_OPTIONCOUNT> DebugMenu_DisableAttacksOptions = {DebugMenuOptions::DisableAttacks, DebugMenuOptions::AttackInRun, 0xFF};
std::array<u8, DISABLEFOG_OPTIONCOUNT> DebugMenu_DisableFogOptions         = {DebugMenuOptions::DisableFog, 0xFF};

Text2dFileData DebugMenu_TextData;
DebugMenuData DebugMenu_Data = {
        {DebugMenu_DisableMusicOptions,
         DebugMenu_MagneticImpulseOptions,
         DebugMenu_AutoPilotOptions,
         DebugMenu_ExtremeDetachOptions,
         DebugMenu_TornadoIgnoreOptions,
         DebugMenu_TimerActivityOptions,
         DebugMenu_DisableHUDOptions,
         DebugMenu_InfiniteAirOptions,
         DebugMenu_InfiniteRingsOptions,
         DebugMenu_MaxMIOptions,
         DebugMenu_MusicPlaylistOptions,
         DebugMenu_ClutchHUDOptions,
         DebugMenu_PTRModeOptions,
         DebugMenu_VanillaHazardsOptions,
         DebugMenu_DisableAttacksOptions,
         DebugMenu_DisableFogOptions}};

bool DebugMenu_CheckOption(u32 option) {
    return (DebugMenu_Data.toggledPageOptions & 1 << option) != 0;
}

[[nodiscard]] bool DebugMenu_IsVanillaPlaylist() {
    return DebugMenu_CheckOption(DebugMenuOptions::MusicPlaylist);
}

void DebugMenu_ToggleOption(std::span<u8> options) {
    if (options.empty()) {
        return;
    }
    DebugMenu_Data.toggledPageOptions ^= 1 << options[0];
}

void DebugMenu_ToggleOptionSet(std::span<u8> options, u32 direction) {
    s32 toggledOptionIndex = -1;

    for (auto i = 0U; i < options.size() - 1; i++) {
        if (DebugMenu_CheckOption(options[i])) {
            toggledOptionIndex = static_cast<s32>(i);
        }

        DebugMenu_Data.toggledPageOptions &= ~(1 << options[i]);
    }

    if (toggledOptionIndex != -1) {
        if (direction == 1) {
            // user pressed right
            toggledOptionIndex += 1;
            if (static_cast<u32>(toggledOptionIndex) >= options.size()) {
                toggledOptionIndex = 0;
            }
        } else {
            // user pressed left
            toggledOptionIndex -= 1;
            if (toggledOptionIndex < 0) {
                toggledOptionIndex = -1;
            }
        }
    } else {
        if (direction == 1) {
            // user pressed right whilst being on OFF
            toggledOptionIndex = 0;
        } else {
            // user pressed left whilst being on OFF
            toggledOptionIndex = static_cast<s32>(options.size() - 2);
        }
    }

    if (toggledOptionIndex != -1) {
        DebugMenu_Data.toggledPageOptions |= 1 << options[static_cast<u32>(toggledOptionIndex)];
    }
}

void DebugMenu_HandleAllToggles(std::span<u8> options, u32 direction) {
    if (options.size() == 2) {
        // simple on/off toggle
        DebugMenu_ToggleOption(options);
    } else {
        // more toggles than on/off
        DebugMenu_ToggleOptionSet(options, direction);
    }
}

u32 DebugMenu_FetchTextIDAllToggles(std::span<u8> options) {
    u32 textID;

    if (options.size() == 2) {
        if (options[0] == DebugMenuOptions::MusicPlaylist) {
            textID = DebugMenu_CheckOption(options[0]) ? 9 : 8;
        } else {
            // simple on/off toggle
            textID = DebugMenu_CheckOption(options[0]) ? 1 : 0;
        }
    } else {
        // more toggles than on/off
        const auto currentOption = DebugMenu_FetchOptionFromOptionSet(options);
        if (currentOption.has_value()) {
            textID = DebugMenu_FetchTextID(*currentOption);
        } else {
            textID = 0;
        }
    }

    return textID;
}

std::optional<u8> DebugMenu_FetchOptionFromOptionSet(std::span<u8> options) {
    for (const auto &option: options) {
        if (DebugMenu_CheckOption(option)) {
            return option;
        }
    }

    return std::nullopt;
}

u32 DebugMenu_FetchTextID(u32 option) {
    switch (option) {
        case DebugMenuOptions::VanillaHazardsOn:
        case DebugMenuOptions::DisableAttacks: return 1;
        case DebugMenuOptions::TimerActivity_ActiveInSingleplayer: return 2;
        case DebugMenuOptions::TimerActivity_ActiveIn1v1: return 3;
        case DebugMenuOptions::TimerActivity_ActiveIn1v1Middle: return 4;
        case DebugMenuOptions::TimerActivity_ActiveIn3OrMorePlayers: return 5;
        case DebugMenuOptions::DisableHUDPartial: return 6;
        case DebugMenuOptions::DisableHUDFull: return 7;
        case DebugMenuOptions::ClutchAsAirGauge: return 10;
        case DebugMenuOptions::ClutchAsTexture: return 11;
        case DebugMenuOptions::VanillaHazardsSkyRoadTurb: return 12;
        case DebugMenuOptions::VanillaHazardsDigiHands: return 13;
        case DebugMenuOptions::AttackInRun: return 14;
        default: return 0;
    }
}

ASMUsed void DebugMenu_RenderDescription() {
    // render description in menu
    const auto textCoords = CenterText(540, 0, lbl_001F1D80[6], &gasSubFont[0].textDataHeader);
    const auto xDiff      = textCoords >> 17;
    const auto x          = 320 - xDiff;
    RenderText(static_cast<s32>(x), 374, ~0U, 540, 0, lbl_001F1D80[6], &gasSubFont[0].textDataHeader, gpasTexList_SubFont.unk0, -100.0f);
}

namespace {

    /**
     * @brief Handles scrolling freely on the debug menu.
     *
     * @param inputs All player inputs
     * @param debugMenu Debug menu data
     */
    void HandleScrollInput(AllPlayerInputs *inputs, DebugMenuData *debugMenu) {
        bool isRStickUp   = false;
        bool isRStickDown = false;
        for (const auto &controller: GameControllers) {
            if (!controller.initStatus)
                continue;
            if (!isRStickUp)
                isRStickUp = controller.rightStickVertical > 80;
            if (!isRStickDown)
                isRStickDown = controller.rightStickVertical < -80;
        }

        const auto yUpperBound = -((debugMenu->maximumItems - MAX_ITEM_AMOUNT_PAGE) * DebugMenuDef::TEXT_HEIGHT);

        if (isRStickDown && debugMenu->itemYOffset > yUpperBound) {
            debugMenu->itemYOffset -= (inputs->holdButtons.hasAny(Buttons::R)) ? 6 : 4;
        } else if (isRStickUp && debugMenu->itemYOffset < 0) {
            debugMenu->itemYOffset += (inputs->holdButtons.hasAny(Buttons::R)) ? 6 : 4;
        }
    }

    /**
     * @brief Checks if the currently selected item on the debug menu is actually visible.
     *
     * @param debugMenu Debug menu data
     * @return true if item is visible, false if is off screen
     */
    [[nodiscard]] bool IsCurrentMenuItemVisible(const DebugMenuData *debugMenu) {
        const auto currY              = std::abs(debugMenu->itemYOffset);
        const auto hiddenTopItemCount = currY / DebugMenuDef::TEXT_HEIGHT;
        const auto hiddenBottomItems  = hiddenTopItemCount + MAX_ITEM_AMOUNT_PAGE;
        return debugMenu->selectedItemRow >= hiddenTopItemCount && debugMenu->selectedItemRow < hiddenBottomItems;
    }

    /**
     * @brief Calculates the scroll Y offset for a given item. Given offset can be aligned to the top or bottom of the screen.
     * @note This function should only be used when the item is off screen!
     *
     * @param item Item to calculate Y offset for.
     * @return Calculated Y offset.
     */
    [[nodiscard]] s32 GetItemYJumpOffset(const u8 item) {
        // when item is going off screen at the top
        if (item < MAX_ITEM_AMOUNT_PAGE) {
            return -(item * DebugMenuDef::TEXT_HEIGHT);
        }

        // when item is going off screen at the bottom
        return -((item + 1 - MAX_ITEM_AMOUNT_PAGE) * DebugMenuDef::TEXT_HEIGHT);
    }

} // namespace

void DebugMenu_HandleInputs(AllPlayerInputs *inputs, DebugMenuData *debugMenu) {
    constexpr auto downInput    = Buttons::LStickDown | Buttons::DPadDown;
    constexpr auto upInput      = Buttons::LStickUp | Buttons::DPadUp;
    constexpr auto holdTimerMax = 30;

    const auto moveDown = [&] {
        PlayAudioFromDAT(Sound::VSFX::MenuScroll);

        u8 selectedItemRow = debugMenu->selectedItemRow + 1;
        if (selectedItemRow >= debugMenu->maximumItems) {
            selectedItemRow = 0;
        }

        debugMenu->selectedItemRow = selectedItemRow;

        if (!IsCurrentMenuItemVisible(debugMenu)) {
            debugMenu->itemYOffset = GetItemYJumpOffset(selectedItemRow);
        }
    };

    const auto moveUp = [&] {
        PlayAudioFromDAT(Sound::VSFX::MenuScroll);

        if (debugMenu->selectedItemRow > 0) {
            debugMenu->selectedItemRow--;
        } else {
            debugMenu->selectedItemRow = debugMenu->maximumItems - 1;
        }

        if (!IsCurrentMenuItemVisible(debugMenu)) {
            debugMenu->itemYOffset = GetItemYJumpOffset(debugMenu->selectedItemRow);
        }
    };

    bool dirPressed = false;
    bool dirIsDown  = false;

    if (inputs->toggleButtons.hasAny(downInput)) {
        moveDown();
        dirPressed = true;
        dirIsDown  = true;
    } else if (inputs->holdButtons.hasAny(downInput)) {
        dirPressed = true;
        dirIsDown  = true;
    }

    if (inputs->toggleButtons.hasAny(upInput)) {
        moveUp();
        dirPressed = true;
    } else if (inputs->holdButtons.hasAny(upInput)) {
        dirPressed = true;
    }

    if (dirPressed) {
        // Keep incrementing timer as long as a direction is still pressed down consecutively
        if (++debugMenu->dirHoldTimer > holdTimerMax) {
            debugMenu->dirHoldTimer = holdTimerMax;
        }
    } else {
        // Otherwise reset timer
        debugMenu->dirHoldTimer = 0;
    }

    // If we reach the designated timer, keep going through items in the given direction until released
    if (debugMenu->dirHoldTimer >= holdTimerMax && gu32GameCnt % 5 == 0) {
        if (dirIsDown) {
            moveDown();
        } else {
            moveUp();
        }
    }

    HandleScrollInput(inputs, debugMenu);

    if (inputs->toggleButtons.hasAny(Buttons::A, Buttons::LStickRight, Buttons::DPadRight)) {
        PlayAudioFromDAT(Sound::VSFX::MenuConfirm);
        DebugMenu_HandleAllToggles(debugMenu->page1Options[debugMenu->selectedItemRow], 1);
    } else if (inputs->toggleButtons.hasAny(Buttons::LStickLeft, Buttons::DPadLeft)) {
        PlayAudioFromDAT(Sound::VSFX::MenuConfirm);
        DebugMenu_HandleAllToggles(debugMenu->page1Options[debugMenu->selectedItemRow], 0);
    }
}

ASMUsed void DebugMenu_Handler(ObjectNode *object, AllPlayerInputs *inputs) {
    auto *header = static_cast<FontHeader *>(DebugMenu_TextData.textData[0]);

    switch (DebugMenu_Data.state) {
        case 0:
            DebugMenu_Data.selectedItemRow = 0;
            DebugMenu_Data.maximumItems    = static_cast<u8>(header->textCount);
            DebugMenu_Data.state += 1;

            if (bss_CustomMusicID != HISTORY_MEGACOLLECTION_ID || DebugMenu_IsVanillaPlaylist()) {
                PlayADX(gpasAdxtHandle_Bgm, menuMusic[HISTORY_MEGACOLLECTION_ID]);
            }

            break;
        case 1:
            if (inputs->toggleButtons.hasAny(Buttons::B)) {
                auto *object1 = static_cast<TitleSequenceObject1 *>(object->object);
                PlayAudioFromDAT(Sound::VSFX::MenuBack);
                object->state               = 0x20;
                object1->currentButtonIndex = 0x6;
                DebugMenu_Data.state        = 0;
                DebugMenu_Data.itemYOffset  = 0;

                if (DebugMenu_IsVanillaPlaylist()) {
                    PlayADX(gpasAdxtHandle_Bgm, lbl_001E99BC[19]);
                } else if (bss_CustomMusicID < MENU_MUSIC_COUNT && bss_CustomMusicID != HISTORY_MEGACOLLECTION_ID) {
                    PlayADX(gpasAdxtHandle_Bgm, menuMusic[bss_CustomMusicID]);
                }
                return;
            }

            DebugMenu_HandleInputs(inputs, &DebugMenu_Data);

            break;
        default: break;
    }
}
