#include "customhud.hpp"
#include "customtext.hpp"
#include "lib/lib.hpp"
#include "lib/stdlib.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "riders/gamemode.hpp"
#include "gears/advantagep.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"

extern const vu32 gu32ViewportNum;

namespace {
    constinit HUDStruct EmerlTypeHUD_1P{
            0x2C,
            0x74,
            0,
            0,
            0x2B,
            0x2B,
            0.0234375f
    };

    constinit std::array<HUDStruct, 2> EmerlTypeHUD_2P = {
            {
                    {
                            0x24,
                            0x54,
                            0,
                            0,
                            0x2B,
                            0x2B,
                            0.0234375f
                    },

                    {
                            0x24,
                            0x144,
                            0,
                            0,
                            0x2B,
                            0x2B,
                            0.0234375f
                    }
            }
    };

    constinit std::array<HUDStruct, 4> EmerlTypeHUD_4P = {
            {
                    {
                            0x3A,
                            0x80,
                            0,
                            0,
                            0x2B,
                            0x2B,
                            0.0234375f
                    },

                    {
                            0x21C,
                            0x80,
                            0,
                            0,
                            0x2B,
                            0x2B,
                            0.0234375f
                    },

                    {
                            0x3A,
                            0x162,
                            0,
                            0,
                            0x2B,
                            0x2B,
                            0.0234375f
                    },

                    {
                            0x21C,
                            0x162,
                            0,
                            0,
                            0x2B,
                            0x2B,
                            0.0234375f
                    }
            }
    };

    constinit HUDStruct AdvantagePTopHUD_1P{
            .screenX = 0x30,
            .screenY = 0xA4,
            .textureX = 0,
            .textureY = 0,
            .textureXLength = 0x5E,
            .textureYLength = 0x15,
            .scale = 0.0534375f
    };

    constinit std::array<HUDStruct, 2> AdvantagePTopHUD_2P = {
            {
                    {
                            .screenX = 0x4C,
                            .screenY = 0x5C,
                            .textureX = 0,
                            .textureY = 0,
                            .textureXLength = 0x42,
                            .textureYLength = 0x12,
                            .scale = 0.0756250023841858f
                    },

                    {
                            .screenX = 0x4C,
                            .screenY = 0x14C,
                            .textureX = 0,
                            .textureY = 0,
                            .textureXLength = 0x42,
                            .textureYLength = 0x12,
                            .scale = 0.0756250023841858f
                    }
            }
    };

    constinit std::array<HUDStruct, 4> AdvantagePTopHUD_4P = {
            {
                    {
                            .screenX = 0x40,
                            .screenY = 0x58,
                            .textureX = 0,
                            .textureY = 0,
                            .textureXLength = 0x3C,
                            .textureYLength = 0x10,
                            .scale = 0.0834375023841858f
                    },

                    {
                            .screenX = 0x204,
                            .screenY = 0x58,
                            .textureX = 0,
                            .textureY = 0,
                            .textureXLength = 0x3C,
                            .textureYLength = 0x10,
                            .scale = 0.0834375023841858f
                    },

                    {
                            .screenX = 0x40,
                            .screenY = 0x13B,
                            .textureX = 0,
                            .textureY = 0,
                            .textureXLength = 0x3C,
                            .textureYLength = 0x10,
                            .scale = 0.0834375023841858f
                    },

                    {
                            .screenX = 0x204,
                            .screenY = 0x13B,
                            .textureX = 0,
                            .textureY = 0,
                            .textureXLength = 0x3C,
                            .textureYLength = 0x10,
                            .scale = 0.0834375023841858f
                    }
            }
    };

    constinit std::array<HUDStruct, 4> PausedPlayersHUDInfo = {{
                                                                       {220,
                                                                        162,
                                                                        0,
                                                                        0,
                                                                        0x20,
                                                                        0x20,
                                                                        0.015625f},

                                                                       {220,
                                                                        162,
                                                                        0x20,
                                                                        0,
                                                                        0x20,
                                                                        0x20,
                                                                        0.015625f},

                                                                       {220,
                                                                        162,
                                                                        0,
                                                                        0x20,
                                                                        0x20,
                                                                        0x20,
                                                                        0.015625f},

                                                                       {220,
                                                                        162,
                                                                        0x20,
                                                                        0x20,
                                                                        0x20,
                                                                        0x20,
                                                                        0.015625f}
                                                               }};

    constexpr std::array<u32, 3> EmerlTypeColors = {
            0x0080FF00,// speed
            0xFFFF0000,// fly
            0xFF200000 // power
    };

    constinit HUDStruct ClutchHUD_1P{
            .screenX = 0x23A,
            .screenY = 0xC4,
            .textureX = 0,
            .textureY = 0,
            .textureXLength = 32,
            .textureYLength = 32,
            .scale = 1.f / 32.f
    };

    constinit std::array<HUDStruct, 2> ClutchHUD_2P = {
            {
                    {
                            .screenX = 0x23C,
                            .screenY = 0x8,
                            .textureX = 0,
                            .textureY = 0,
                            .textureXLength = 28,
                            .textureYLength = 28,
                            .scale = 1.f / 28.f
                    },

                    {
                            .screenX = 0x23C,
                            .screenY = 0xF8,
                            .textureX = 0,
                            .textureY = 0,
                            .textureXLength = 28,
                            .textureYLength = 28,
                            .scale = 1.f / 28.f
                    }
            }
    };

    constinit std::array<HUDStruct, 4> ClutchHUD_4P = {
            {
                    {
                            .screenX = 0x64,
                            .screenY = 0xC4,
                            .textureX = 0,
                            .textureY = 0,
                            .textureXLength = 16,
                            .textureYLength = 16,
                            .scale = 1.f / 16.f
                    },

                    {
                            .screenX = 0x20C,
                            .screenY = 0xC4,
                            .textureX = 0,
                            .textureY = 0,
                            .textureXLength = 16,
                            .textureYLength = 16,
                            .scale = 1.f / 16.f
                    },

                    {
                            .screenX = 0x64,
                            .screenY = 0x1A8,
                            .textureX = 0,
                            .textureY = 0,
                            .textureXLength = 16,
                            .textureYLength = 16,
                            .scale = 1.f / 16.f
                    },

                    {
                            .screenX = 0x20C,
                            .screenY = 0x1A8,
                            .textureX = 0,
                            .textureY = 0,
                            .textureXLength = 16,
                            .textureYLength = 16,
                            .scale = 1.f / 16.f
                    }
            }
    };

    const HUDStruct &
    GetIconHUDStruct(const Player &player, const HUDStruct &hud1P, const std::array<HUDStruct, 2> &hud2P,
                     const std::array<HUDStruct, 4> &hud4P) {
        const u32 playerAmount = clamp(gu32ViewportNum, 0U, 4U);

        switch (playerAmount) {
            case 1:
                return hud1P;
            case 2:
                return hud2P[player.index];
            case 3 ... 4:
            default:
                return hud4P[player.index];
        }
    }

    void RenderEmerlTypeIcon(const Player &player, HUDIconObject1 *object1) {
        const auto &playerIndex = player.index;

        u8 currOpacity = object1->opacity[playerIndex];

        u32 textureRGBA;
        const auto &type = player.typeAttributes;
        if (type.hasAny(Type::Power)) {
            textureRGBA = EmerlTypeColors[2];
        } else if (type.hasAny(Type::Fly)) {
            textureRGBA = EmerlTypeColors[1];
        } else if (type.hasAny(Type::Speed)) {
            textureRGBA = EmerlTypeColors[0];
        } else {
            textureRGBA = 0xFFFFFF00;
        }

        const GizoidReplicationInfo &grInfo = PlayerGizoidReplication[playerIndex];

        if (grInfo.timer > 0 && grInfo.timer < toFrames(2)) {
            if ((gu32GameCnt % 15) == 0u) {
                object1->isFullyOpaque[playerIndex] ^= 1;
            }

            if (!object1->isFullyOpaque[playerIndex]) {
                currOpacity = 0;
            }
        }

        const HUDStruct &hud = GetIconHUDStruct(player, EmerlTypeHUD_1P, EmerlTypeHUD_2P, EmerlTypeHUD_4P);
        textureRGBA |= currOpacity;
        Custom_CreateHUDElement(texList_GTSE_0, 8, textureRGBA, &hud);
    }

    void RenderAdvantagePTopIn(const Player &player, HUDIconObject1 *object1) {
        if (PlayerAdvantagePInfo[player.index].isInTopMode) {
            const auto &hud = GetIconHUDStruct(player, AdvantagePTopHUD_1P, AdvantagePTopHUD_2P, AdvantagePTopHUD_4P);
            Custom_CreateHUDElement(texList_GTSE_0, 10, 0xFFFFFF00 | object1->opacity[player.index], &hud, 256 / 64);
        }
    }

    [[nodiscard]] bool IsHUDVisibleWithTricking(const Player &player) {
        return (geGame_ModeDetail - CurrentGameMode) >= 2 &&
               gu32Game_PauseFlag == 0u &&
               !ruleSettings.unk7 &&
               (!player.flags.hasAny(static_cast<PlayerFlags>(0x80000)));
    }

    void RenderClutchHUD(const Player &player, HUDIconObject1 *object1) {
        if (!DebugMenu_CheckOption(DebugMenuOptions::ClutchAsTexture)) { return; }

        const auto isClutchOn = static_cast<u8>(player.ignoreTurbulence);
        const auto &hud = GetIconHUDStruct(player, ClutchHUD_1P, ClutchHUD_2P, ClutchHUD_4P);
        Custom_CreateHUDElement(texList_GTSE_0, 11 + isClutchOn,
                                0xFFFFFF00 | object1->opacityWithTricking[player.index], &hud);
    }
}

inline void HandleHUDOpacity(const Player &player, u8 *opacity, HUDVisibleFunc isHudVisible) {
    if (isHudVisible == nullptr) { std::unreachable(); }
    if (opacity == nullptr) { std::unreachable(); }

    const auto &playerIndex = player.index;

    s32 newOpacity;
    if (isHudVisible(player)) {
        newOpacity = opacity[playerIndex] + 8;
    } else {
        if (gu32Game_PauseFlag) {
            newOpacity = opacity[playerIndex] - 32;
        } else {
            newOpacity = opacity[playerIndex] - 8;
        }
    }

    newOpacity = clamp(newOpacity, 0x0, 0xFF);
    opacity[playerIndex] = static_cast<u8>(newOpacity);
}

ASMUsed void Player_RenderHUDIcons() {
    if (CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != TimeTrial) {
        return;
    }

    ObjectNode *object = gpsCurrentTask;
    auto *object1 = static_cast<HUDIconObject1 *>(object->object);

    switch (object->state) {
        default:
            break;
        case 2:
            std::ranges::fill(object1->opacity, 0);
            std::ranges::fill(object1->opacityWithTricking, 0);
            std::ranges::fill(object1->isFullyOpaque, 0);
            object->state += 1;
            [[fallthrough]];
        case 3:
            for (const auto ref: getConnectedHumanPlayers()) {
                const Player &player = ref.get();

                HandleHUDOpacity(player, object1->opacityWithTricking.data(), IsHUDVisibleWithTricking);
                RenderClutchHUD(player, object1);

                const auto isEmerl = player.character == Character::Emerl;
                const auto isAdvP = player.extremeGear == ExtremeGear::AdvantageP;

                if (isEmerl || isAdvP) {
                    HandleHUDOpacity(player, object1->opacity.data(), Player_IsHUDVisible);

                    if (isEmerl) {
                        RenderEmerlTypeIcon(player, object1);
                    }

                    if (isAdvP) {
                        RenderAdvantagePTopIn(player, object1);
                    }
                }
            }
            break;
    }
}

ASMUsed void RenderPausedPlayerIcon(const u8 alpha) {
    const ObjectNode *gameCtrlObject = nullptr;
    for (const auto &currentObject: getObjectList()) {
        // try to find GameCtrl task
        if (currentObject.object_group == 1) {
            gameCtrlObject = &currentObject;
            break;
        }
    }

    if (gameCtrlObject == nullptr) {
        return;
    }

    const auto *object1 = static_cast<GameCtrlObject1 *>(gameCtrlObject->object);

    HUDStruct &hud = PausedPlayersHUDInfo[object1->pausedPlayerIndex];
    Custom_CreateHUDElement(texList_GTSE_0, 9, 0xFFFFFF00 | alpha, &hud);
}
