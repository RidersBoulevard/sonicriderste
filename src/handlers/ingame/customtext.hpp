#pragma once

#include "context.hpp"

struct SubFont {
	void *textDataHeader;
	void *textDataStart;
	void *padding;
};

global void* lbl_100178D4[3][4];
global u32 geGame_ModeDetail;

global f32 lbl_001D2E7C;
global void* gpasTexList_SubFont;
global SubFont gasSubFont[];

extern void* CustomTextArchiveFile[];

BOOL Player_IsHUDVisible(const Player &player);
ASMUsed void Player_SuperTailsRenderText();