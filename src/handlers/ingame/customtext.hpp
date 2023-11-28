#pragma once

#include "riders/player.hpp"
#include "riders/gamemode.hpp"

struct SubFont {
	void *textDataHeader;
	void *textDataStart;
	void *padding;
};

struct SubFontTexList {
    void* unk0;
    void* unk4;
};

ASMDefined m2darray<void*, 3, 4> lbl_100178D4;

ASMDefined f32 lbl_001D2E7C;
ASMDefined SubFontTexList gpasTexList_SubFont;
ASMDefined SubFont gasSubFont[];

extern std::array<void*, 2> CustomTextArchiveFile;

bool Player_IsHUDVisible(const Player &player);
ASMUsed void Player_SuperTailsRenderText();