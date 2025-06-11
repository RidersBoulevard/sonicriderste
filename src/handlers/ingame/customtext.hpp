#pragma once

#include "riders/player.hpp"
#include "riders/gamemode.hpp"

struct CharGlyph {
    u8 flagsAndWidth;
    u8 character;
};

struct FontHeader {
    u16 texWidth;
    u16 texHeight;
    CharGlyph unkGlyph;
    u8 charWidth;
    u8 padding;
    u32 textCount;
};

struct SubFont {
    FontHeader *textDataHeader;
	CharGlyph **textDataStart;
	void *padding;
};

struct SubFontTexList {
    void* unk0;
    void* unk4;
};

ASMDefined m2darray<void*, 3, 4> gpsRotValueTask;

ASMDefined f32 lbl_001D2E7C;
ASMDefined SubFontTexList gpasTexList_SubFont;
ASMDefined SubFont gasSubFont[];

extern std::array<void*, 2> CustomTextArchiveFile;

bool Player_IsHUDVisible(const Player &player);
ASMUsed void Player_SuperTailsRenderText();

/**
 * Calculates the amount of lines a text string rendered via RenderText() takes up.
 *
 * @param text The string array.
 * @param charWidth The width in pixels between the characters in the font's texture.
 * @param maxLineWidth The max width of one line.
 * @return The amount of lines the text string uses.
 */
[[nodiscard]] u32 CalculateTextStringLineCount(CharGlyph *text, const u8 charWidth, const u16 maxLineWidth);

/**
 * Gets a text string array from the specified subfont with the specified ID.
 *
 * @param subFont The subfont to get the string from.
 * @param textID The ID of the string.
 * @return A pointer to the string.
 */
[[nodiscard]] CharGlyph* GetTextStringArray(const SubFont &subFont, const u32 textID);