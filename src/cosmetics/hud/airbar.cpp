#include "globalDefs.hpp"
#include "riders/player.hpp"
#include "lib/lib.hpp"
#include "handlers/ingame/customhud.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"
#include "gears/blastGaugeGears.hpp"

struct TexUVParam {
    s32 x;
    s32 y;
    s32 xLength;
    s32 yLength;
};

ASMDefined {
/**
* Renders the texture supplied via InitTextureHook() with the given parameters on the screen.
*
* @param x X position on screen (in pixels).
* @param y Y position on screen (in pixels).
* @param xLength How much of the texture to draw in the X direction (in pixels).
* @param yLength How much of the texture to draw in the Y direction (in pixels).
* @param rgba The tint of the texture.
* @param uvParam UV parameters that determine which region of the given texture to render.
* @param textureWidth The width of the supplied texture.
* @param textureHeight The height of the supplied texture.
*/
void Draw2dData(s32 x, s32 y, s32 xLength, s32 yLength, u32 rgba, s32 unk, TexUVParam *uvParam, u32 textureWidth,
                u32 textureHeight);

extern std::array<TexUVParam, 170> gsTexSrc_Game2D_00;
}

struct AirGaugeObject1 {
    s32 currAirChangeLoss; // 1 player only
    fillerData<0xc> filler1;
    s32 currAirChangeGain; // 1 player only
    fillerData<0xc> filler2;
    u8 currOpacity; // universal, exists in both sp or mp
};

namespace {
    constexpr auto RED_PARAM_IDX = 0x79;
    constexpr auto GRAY_PARAM_IDX = 0x74;
    constexpr auto BLUE_PARAM_IDX = 0x75;

    inline s32 CalculateAirBarYLength(const s32 maxYLength, const s32 currentAirScale, const s32 maxAirScale) {
        return (maxYLength * currentAirScale) / maxAirScale;
    }

    inline s32 CalculateAirScale(const s32 airValue) {
        return airValue / 1000;
    }

    inline void CheckClutch(const Player &player, const u32 breathingColor, const u32 mainUvParamIndex, u32 &color,
                            u32 &uvParamIndex) {
        constexpr u32 CLUTCH_AIRGAUGE_COLOR = 0x8B37E500;

        const auto isClutchOn = DebugMenu_CheckOption(DebugMenuOptions::ClutchAsAirGauge) && player.ignoreTurbulence;
        color = isClutchOn ? CLUTCH_AIRGAUGE_COLOR : breathingColor;
        uvParamIndex = isClutchOn ? GRAY_PARAM_IDX : mainUvParamIndex;
    }

    void DrawAirBar(const Player &player, const s32 x, const s32 y, const s32 xLength, const s32 maxYLength,
                    const u32 mainColor, const u8 currOpacity, const u32 breathingColor,
                    const u32 mainAirBarUVParamIndex, const s32 currentAirLossScale, const s32 currentAirGainScale) {
        constexpr auto TEXTURE_W_H = 512;

        const s32 maxAirScale = CalculateAirScale(player.gearStats[2].maxAir);

        // these are negative cuz we want to render it upwards
        const auto currAir = player.isBlastGaugeGear ? PlayerBlastGaugeInfo[player.index].currentGauge
                                                     : player.currentAir;
        const s32 currentAirScale = -CalculateAirScale(currAir);

        const s32 airLossLength = CalculateAirBarYLength(maxYLength, currentAirLossScale, maxAirScale);
        const s32 airLength = CalculateAirBarYLength(maxYLength, currentAirScale, maxAirScale);
        const s32 airGainLength = CalculateAirBarYLength(maxYLength, currentAirGainScale, maxAirScale);

        Draw2dData(x, y, xLength, airLossLength, 0xFFFFFF00 | currOpacity, 0,
                   &gsTexSrc_Game2D_00[RED_PARAM_IDX], TEXTURE_W_H, TEXTURE_W_H);

        Draw2dData(x, y, xLength, airLength, breathingColor | currOpacity, 0,
                   &gsTexSrc_Game2D_00[GRAY_PARAM_IDX], TEXTURE_W_H, TEXTURE_W_H);

        Draw2dData(x, y, xLength, airGainLength, mainColor | currOpacity, 0,
                   &gsTexSrc_Game2D_00[mainAirBarUVParamIndex], TEXTURE_W_H, TEXTURE_W_H);
    }
}

ASMUsed void
PlayerRenderAirBar1P(const Player &player, AirGaugeObject1 &object1, u32 mainAirBarUVParamIndex, const s32 x,
                     const u32 breathingColor) {
    constexpr auto MAX_YLENGTH = 140;

    InitTextureHook(texList_GTSE_0, 0);
    HookTexture(3);

    const s32 currentAirLossScale = -CalculateAirScale(object1.currAirChangeLoss);
    const s32 currentAirGainScale = -CalculateAirScale(object1.currAirChangeGain);

    if (player.isBlastGaugeGear) {
        mainAirBarUVParamIndex = BLUE_PARAM_IDX;
    }

    u32 airGainColor;
    u32 uvParamIndex;
    CheckClutch(player, breathingColor, mainAirBarUVParamIndex, airGainColor, uvParamIndex);

    DrawAirBar(player, x, 392, 10, MAX_YLENGTH, airGainColor, object1.currOpacity, breathingColor, uvParamIndex,
               currentAirLossScale, currentAirGainScale);
}

ASMUsed void
PlayerRenderAirBarMP(const Player &player, AirGaugeObject1 &object1, u32 mainAirBarUVParamIndex, const s32 x,
                     const s32 y, const s32 xLength, const s32 yLength, const u32 breathingColor,
                     const s32 currentAirLoss, const s32 currentAirGain) {
    const s32 currentAirLossScale = -CalculateAirScale(currentAirLoss);
    const s32 currentAirGainScale = -CalculateAirScale(currentAirGain);

    if (player.isBlastGaugeGear) {
        mainAirBarUVParamIndex = BLUE_PARAM_IDX;
    }

    u32 airGainColor;
    u32 uvParamIndex;
    CheckClutch(player, breathingColor, mainAirBarUVParamIndex, airGainColor, uvParamIndex);

    DrawAirBar(player, x, y, xLength, yLength, airGainColor, object1.currOpacity, breathingColor, uvParamIndex,
               currentAirLossScale, currentAirGainScale);
}