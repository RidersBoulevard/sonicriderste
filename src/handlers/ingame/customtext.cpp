#include "customtext.hpp"
#include "lib/lib.hpp"
#include "main.hpp"
#include "riders/gamemode.hpp"
#include "riders/object.hpp"
#include "gears/supertails.hpp"

ASMDefined {
	USED auto ObjectGroup_Player_SuperTailsRenderText = ObjectGroups::Object_SuperTailsRenderText;
}

bool Player_IsHUDVisible(const Player &player) {
	if ((geGame_ModeDetail - CurrentGameMode) < 2) { return false; }
	if (gu32Game_PauseFlag != 0u) { return false; }
	if (ruleSettings.unk7) { return false; }
	if (player.flags.hasAny(static_cast<PlayerFlags>(0x80000))) { return false; }

	for (auto &i : gpsRotValueTask) {
		if (i[player.index] != nullptr) {
			return false;
		}
	}

	return true;
}

constexpr std::array<u16, 2> CustomText2P_Y = {
		0x56, 0x144 // player 1 and 2 Y
};

constexpr m2darray<u16, 4, 2> CustomText4P_XY = {{
		{0x2E,  0x54}, // player 1 X and Y
		{0x1F8, 0x54}, // player 2 X and Y
		{0x2E,  0x136}, // player 3 X and Y
		{0x1F8, 0x136}, // player 4 X and Y
}};

ASMUsed void Player_SuperTailsRenderText() {
	if (CustomTextArchiveFile[0] == nullptr) { return; }
	const auto humanPlayers = getConnectedHumanPlayers();
	for (const auto ref : humanPlayers) {
		const auto &player = ref.get(); // Unwrap referance_wrapper
        if (!player.isSuperCharacter(Character::Tails)) { continue; } // Make sure this is super tails
        if (!Player_IsHUDVisible(player)) { continue; }
		if (player.superFormState != 0) { continue; } // make sure he isn't super
		if (player.rings < supertailsdef::MAGNETCOST) { continue; } // make sure tails can actually spend rings

        // if magnet can be activated, convert true into 1, which is the text ID for magnet text, otherwise it's 0 for the other text
        const u32 textID = player.rings < supertailsdef::REQUIREDTRANSFORMRINGS;

		switch (humanPlayers.size()) {
			case 1:
				RenderText(52, 116, ~0U, 100, 0, textID, CustomTextArchiveFile.data(), gpasTexList_SubFont.unk4, lbl_001D2E7C);
				continue;
			case 2: {
				const u32 &y = CustomText2P_Y[player.index];
				RenderText(46, y, ~0U, 100, 0, textID, CustomTextArchiveFile.data(), gpasTexList_SubFont.unk4, lbl_001D2E7C);
				continue;
			}
			case 3:
			case 4: {
				const auto &[x, y] = CustomText4P_XY[player.index];
				//const u32 &x = CustomText4P_XY[player.index][0];
				//const u32 &y = CustomText4P_XY[player.index][1];

				RenderText(x, y, ~0U, 100, 0, textID, CustomTextArchiveFile.data(), gpasTexList_SubFont.unk4, lbl_001D2E7C);
				continue;
			}
			default: std::unreachable();
		}
	}
}

[[nodiscard]] u32 CalculateTextStringLineCount(CharGlyph *text, const u8 charWidth, const u16 maxLineWidth) {
    u32 lineCount = 1;
    u32 curLineWidth = 0;
    const u8 spaceSize = charWidth / 2;
    text--;

    while (true) {
        text++;
        auto glyph = *text;

        if (glyph.flagsAndWidth & 0x80) {
            // null terminator
            if (glyph.character == 0) break;

            switch (glyph.character) {
                case 2:
                    // space
                    curLineWidth += spaceSize;
                    if (curLineWidth <= maxLineWidth) continue;
                    [[fallthrough]];
                case 1:
                    // new line
                    curLineWidth = 0;
                    lineCount++;
                    continue;
                default:
                    continue;
            }
        }

        const u8 width = (glyph.flagsAndWidth & 0x40) ? glyph.flagsAndWidth & 0x3F : charWidth;
        curLineWidth += width;

        if (curLineWidth > maxLineWidth) {
            curLineWidth = width;
            lineCount++;
        }
    }

    return lineCount;
}

[[nodiscard]] CharGlyph* GetTextStringArray(const SubFont &subFont, const u32 textID) {
    return *(subFont.textDataStart + textID);
}
