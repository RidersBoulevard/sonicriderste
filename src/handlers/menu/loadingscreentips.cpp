#include "globalDefs.hpp"
#include "lib/lib.hpp"
#include "lib/stdlib.hpp"
#include "handlers/files/packman_handlers.hpp"

bool CanRenderLoadingScreenTips = false;
u8 LoadingScreenFontOpacity;
u8 LoadingScreenTextID;

/**
 * Renders randomly selected tips on the loading screen before a race.
 */
ASMUsed void RenderLoadingScreenLines() {
    if (LoadingScreenFontTexList == nullptr || !CanRenderLoadingScreenTips) return;

    const u32 centeredCoords = CenterText(540, 0, LoadingScreenTextID, &LoadingScreenTipsFont.textDataHeader);
    const u32 xDiff = centeredCoords >> 17;
    const u32 yDiff = (centeredCoords & 0xFFFF) / 2;
    const s32 x = static_cast<const s32>(320 - xDiff);
    const s32 y = static_cast<const s32>(230 - yDiff);
    s32 opacity = LoadingScreenFontOpacity;
    u32 rgba = 0xFFFFFF00 | opacity;

    RenderText(x, y, rgba, 540, 0, LoadingScreenTextID, &LoadingScreenTipsFont.textDataHeader, LoadingScreenFontTexList, -100.0f);

    opacity += 8;
    LoadingScreenFontOpacity = clamp(opacity, 0, 0xFF);
}