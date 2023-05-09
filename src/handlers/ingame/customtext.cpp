#include "customtext.hpp"
#include "lib/lib.hpp"

global {
	USED u32 ObjectGroup_Player_SuperTailsRenderText = Object_SuperTailsRenderText;
}

BOOL Player_IsHUDVisible(const Player &player) {
	if ((geGame_ModeDetail - CurrentGameMode) < 2) return FALSE;
	if (gu32Game_PauseFlag) return FALSE;
	if (RuleSettings & 0x80) return FALSE;
	if (player.flags & 0x80000) return FALSE;

	for (auto & i : lbl_100178D4) {
		if (i[player.index]) { return FALSE; }
	}

	return TRUE;
}

constexpr u16 CustomText2P_Y[2] = {
		0x56, 0x144 // player 1 and 2 Y
};

constexpr u16 CustomText4P_XY[4][2] = {
		{0x2E,  0x54}, // player 1 X and Y
		{0x1F8, 0x54}, // player 2 X and Y
		{0x2E,  0x136}, // player 3 X and Y
		{0x1F8, 0x136}, // player 4 X and Y
};

ASMUsed void Player_SuperTailsRenderText() {
	for (u32 i = 0; i < InGamePlayerCount; i++) {
		const Player &player = players[i];
		if (!Player_IsHUDVisible(player)) continue;
		if (!(CustomTextArchiveFile[0])) continue;
		if (!isSuperCharacter(player, Tails)) continue; // Make sure this is super tails
		if (player.superFormState) continue; // make sure he isn't super
		if (player.rings < 50) continue; // make sure tails can actually transform

		switch (InGamePlayerCount) {
			case 1:
				RenderText(52, 116, ~0U, 100, 0, 0, &CustomTextArchiveFile[0], *(&gpasTexList_SubFont + 1), lbl_001D2E7C);
				continue;
			case 2: {
				u32 y = CustomText2P_Y[player.index];
				RenderText(46, y, ~0U, 100, 0, 0, &CustomTextArchiveFile[0], *(&gpasTexList_SubFont + 1), lbl_001D2E7C);
				continue;
			}
			case 3:
			case 4: {
				u32 x = CustomText4P_XY[player.index][0];
				u32 y = CustomText4P_XY[player.index][1];

				RenderText(x, y, ~0U, 100, 0, 0, &CustomTextArchiveFile[0], *(&gpasTexList_SubFont + 1), lbl_001D2E7C);
				continue;
			}
			default:
				continue;
		}
	}
}