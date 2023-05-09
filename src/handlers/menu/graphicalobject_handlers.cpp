#include "handlers/menu/debugmenu/debugmenu.hpp"
#include "lib/lib.hpp"
#include "handlers/ingame/customtext.hpp"
#include "handlers/menu/tournamentrace/stageban_handler.hpp"
#include "containers/graphicalobject.hpp"
#include "handlers/music/custom_music.hpp"

// global void Sys2d_DrawObject(void *graphicalObject); /*asm*/

global void GraphicalObjectHandler_TournamentRace(void *graphicalObject) {
	BSS_StageBan *bss = &bss_StageBans;
	void *objectOffsets, *textureData;
	u32 i, playerIndex, switchCase;
	u8 textureID;
	BOOL init = FALSE;

	u8 *active = (u8 *) graphicalObject + 0x57;
	u32 *activeTimer = (u32 *) graphicalObject + 0x48 / 4;
	u16 *index = (u16 *) graphicalObject + 0x34 / 2;
	void *info = (void *) *((u32 *) graphicalObject + 0x40 / 4);
	void *objectInfo = (void *) *((u32 *) info + 0x18 / 4 + *index);

	if (bss->tournamentRace) {
		init = *activeTimer == 0;
		if (init)
			*active = 0xC;

		if (bss_StageBans_isBanned || init) {
			if (*(u32 *) objectInfo >= 9) { // texture count
				objectOffsets = (u8 *) objectInfo + 0xC;

				// update player win count and ban count texture
				for (i = 0; i < 4; i++) {
					if (i >= 2) {
						switchCase = i - 2;
						playerIndex = 1;
					} else {
						switchCase = i;
						playerIndex = 0;
					}

					if (playerIndex == 0) {
						switch (switchCase) {
							case 0:
								// p1 ban count
								objectOffsets = (u32 *) objectOffsets + 1; // ban count never goes over 10 so skip it

								textureData = (void *) *(u32 *) objectOffsets;
								objectOffsets = (u32 *) objectOffsets + 1;

								textureID = 0x8D + bss->player1_banCount;
								*((u8 *) textureData + 0xB) = textureID;
								break;
							case 1:
								// p1 win count
								textureData = (void *) *(u32 *) objectOffsets;
								objectOffsets = (u32 *) objectOffsets + 1;

								textureID = 0x8D + (bss->player1Score / 10);
								*((u8 *) textureData + 0xB) = textureID;

								textureData = (void *) *(u32 *) objectOffsets;
								objectOffsets = (u32 *) objectOffsets + 1;

								textureID = 0x8D + (bss->player1Score % 10);
								*((u8 *) textureData + 0xB) = textureID;
								break;
						}
					} else {
						switch (switchCase) {
							case 0:
								// p2 ban count
								objectOffsets = (u32 *) objectOffsets + 1; // ban count never goes over 10 so skip it

								textureData = (void *) *(u32 *) objectOffsets;
								objectOffsets = (u32 *) objectOffsets + 1;

								textureID = 0x8D + bss->player2_banCount;
								*((u8 *) textureData + 0xB) = textureID;
								break;
							case 1:
								// p2 win count
								textureData = (void *) *(u32 *) objectOffsets;
								objectOffsets = (u32 *) objectOffsets + 1;

								textureID = 0x8D + (bss->player2Score / 10);
								*((u8 *) textureData + 0xB) = textureID;

								textureData = (void *) *(u32 *) objectOffsets;
								objectOffsets = (u32 *) objectOffsets + 1;

								textureID = 0x8D + (bss->player2Score % 10);
								*((u8 *) textureData + 0xB) = textureID;
								break;
						}
					}
				}

			}
		}
		Sys2d_DrawObject((GraphicalObject*)graphicalObject);
	}

}

global void GraphicalObjectHandler_DebugMenuText(void *graphicalObject) {
    DebugMenuData *debugMenu = &DebugMenu_Data;
    void *textTexture = gpasTexList_SubFont;

    Sys2d_DrawObject((GraphicalObject*)graphicalObject);

    DebugMenu_RenderDescription();

    for (u32 i = 0, y = 100; i < debugMenu->maximumItems; i++, y += 20) {
        u32 rgba, textID;
        DebugOptions options = debugMenu->page1Options[i];

        if (debugMenu->selectedItemRow == i) {
            rgba = 0xFF0000FF;
        } else {
            rgba = ~0U;
        }

        textID = DebugMenu_FetchTextIDAllToggles(&options);

        RenderText(20, y, rgba, 240, 0, i, &DebugMenu_TextData.textData[0], textTexture, -1.0f);
        RenderText(270, y, rgba, 240, 0, textID, &DebugMenu_TextData.extraTextData[0], textTexture, -1.0f);
    }
}

global void GraphicalObjectHandler_DisplayMusicTextMenu(void *graphicalObject) {
    void *textTexture = gpasTexList_SubFont;

    Sys2d_DrawObject((GraphicalObject*)graphicalObject);

    u32 musicTextID = bss_CustomMusicID + 1;

    RenderText(20, 20, ~0U, 240, 0, 0, &CustomMusicTextData.textDataHeader, textTexture, -100.0f);
    RenderText(172, 20, ~0U, 480, 0, musicTextID, &CustomMusicTextData.textDataHeader, textTexture, -100.0f);
}

#define GAMELANGUAGE_COUNT 6

global s32 geMessageLanguage;

constexpr std::array<u8, GAMELANGUAGE_COUNT> MessageTexts1 = {
        0xE,
        0x10, // English
        0x12,
        0x14,
        0x16,
        0x18
};

constexpr std::array<u8, GAMELANGUAGE_COUNT> MessageTexts2 = {
        0xF,
        0x11, // English
        0x13,
        0x15,
        0x17,
        0x19
};

constexpr std::array<std::array<u8, 20>, GAMELANGUAGE_COUNT> ModeTitleTexts = {
        // extra ID at the end is for debug menu
        {
                {
                        0x2D, 0x2E, 0x2F, 0x31, 0x30, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C,
                        0x3D, 0x3E, 0x3F, 0x8E
                },

                // English
                {
                        0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
                        0x84, 0x2B, 0x2C, 0x8E
                },

                {
                        0x40, 0x1B, 0x1C, 0x41, 0x42, 0x5F, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x28, 0x4B,
                        0x4C, 0x4D, 0x4E, 0x8E
                },

                {
                        0x4F, 0x1B, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x61, 0x25, 0x58, 0x59, 0x5A, 0x5B,
                        0x85, 0x5E, 0x5D, 0x8E
                },

                {
                        0x63, 0x1B, 0x64, 0x65, 0x66, 0x60, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x62, 0x6F,
                        0x70, 0x71, 0x72, 0x8E
                },

                {
                        0x73, 0x1B, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x6C, 0x7D, 0x7E, 0x7F, 0x80,
                        0x86, 0x82, 0x83, 0x8E
                }
        }
};

// function decompiled
global void GraphicalObjectHandler_ModeTitle(GraphicalObject *gobject) {
    void *object1ptr = static_cast<u8*>(gp2DSys) + (gobject->cullingGroup * 8) +  0x6000;
    void **object1addr = static_cast<void**>(object1ptr);
    auto *object1 = static_cast<TitleSequenceObject1*>(*object1addr);
    gobject->active = gobject->active | 8;
    gobject->textureIDs[0].textureSlot1 = ModeTitleTexts[geMessageLanguage][object1->currentMode];
    gobject->textureIDs[1].textureSlot1 = 0x1A;
    gobject->textureIDs[0].textureSlot2 = MessageTexts1[geMessageLanguage];
    gobject->textureIDs[1].textureSlot2 = 0xE;
    gobject->textureIDs[0].textureSlot3 = MessageTexts2[geMessageLanguage];
    gobject->textureIDs[1].textureSlot3 = 0xF;
    Sys2d_DrawObject(gobject);
}