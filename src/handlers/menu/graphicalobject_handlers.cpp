#include "handlers/menu/debugmenu/debugmenu.hpp"
#include "lib/lib.hpp"
#include "handlers/ingame/customtext.hpp"
#include "handlers/menu/tournamentrace/stageban_handler.hpp"
#include "containers/graphicalobject.hpp"
#include "handlers/music/custom_music.hpp"
#include "cosmetics/player/exloads.hpp"
#include "handlers/ingame/customcss.hpp"
#include "tweaks/player/archetype/character_archetype.hpp"
#include "handlers/files/packman_handlers.hpp"

// ASMDefined void Sys2d_DrawObject(void *graphicalObject); /*asm*/

ASMDefined void GraphicalObjectHandler_TournamentRace(void *graphicalObject) {
	u8 *active = static_cast<u8 *>(graphicalObject) + 0x57;
	u32 *activeTimer = static_cast<u32 *>(graphicalObject) + 0x48 / 4;
	u16 *index = static_cast<u16 *>(graphicalObject) + 0x34 / 2;
	void *info = static_cast<void *>(*(static_cast<u32 **>(graphicalObject) + 0x40 / 4));
	void *objectInfo = static_cast<void *>(*(static_cast<u32 **>(info) + 0x18 / 4 + *index));

	BSS_StageBan *bss = &bss_StageBans;
	void *objectOffsets, *textureData;
	u32 i, playerIndex, switchCase;
	u8 textureID;
	bool init = FALSE;
	if (bss->tournamentRace != 0u) {
		init = *activeTimer == 0;
		if (init) {
			*active = 0xC;
		}

		if ((bss_StageBans_isBanned != 0u) || init) {
			if (*static_cast<u32 *>(objectInfo) >= 9) { // texture count
				objectOffsets = static_cast<u8 *>(objectInfo) + 0xC;

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
								objectOffsets = static_cast<u32 *>(objectOffsets) + 1; // ban count never goes over 10 so skip it

								textureData = (void *) *(u32 *) objectOffsets;
								objectOffsets = (u32 *) objectOffsets + 1;

								textureID = 0x8D + bss->player_banCount[0];
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

								textureID = 0x8D + bss->player_banCount[1];
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
		Sys2d_DrawObject(static_cast<GraphicalObject*>(graphicalObject));
	}

}

ASMDefined void GraphicalObjectHandler_DebugMenuText(void *graphicalObject) {
    DebugMenuData *debugMenu = &DebugMenu_Data;
    void *textTexture = gpasTexList_SubFont.unk0;

    Sys2d_DrawObject(static_cast<GraphicalObject*>(graphicalObject));

    DebugMenu_RenderDescription();

    for (u32 i = 0, y = 100; i < debugMenu->maximumItems; i++, y += 20) {
		const RGBA32 rgba = debugMenu->selectedItemRow == i ? 0xFF0000FF : ~0U;

		auto options = debugMenu->page1Options[i];
        auto textID = DebugMenu_FetchTextIDAllToggles(options);

        RenderText(20, y, rgba, 240, 0, i, DebugMenu_TextData.textData.data(), textTexture, -1.0f);
        RenderText(270, y, rgba, 240, 0, textID, DebugMenu_TextData.extraTextData.data(), textTexture, -1.0f);
    }
}

ASMDefined void GraphicalObjectHandler_DisplayMusicTextMenu(void *graphicalObject) {
    void *textTexture = gpasTexList_SubFont.unk0;

	Sys2d_DrawObject(static_cast<GraphicalObject*>(graphicalObject));

    const u32 musicTextID = bss_CustomMusicID + 1;

    RenderText(20, 20, ~0U, 240, 0, 0, &CustomMusicTextData.textDataHeader, textTexture, -100.0f);
    RenderText(172, 20, ~0U, 480, 0, musicTextID, &CustomMusicTextData.textDataHeader, textTexture, -100.0f);
}

constexpr auto GameLanguage_count = 6;

ASMDefined s32 geMessageLanguage;

constexpr std::array<u8, GameLanguage_count> MessageTexts1 = {
        0xE,
        0x10, // English
        0x12,
        0x14,
        0x16,
        0x18
};

constexpr std::array<u8, GameLanguage_count> MessageTexts2 = {
        0xF,
        0x11, // English
        0x13,
        0x15,
        0x17,
        0x19
};

constexpr std::array<std::array<u8, 20>, GameLanguage_count> ModeTitleTexts = {
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
ASMDefined void GraphicalObjectHandler_ModeTitle(GraphicalObject *gobject) {
    void *object1ptr = static_cast<u8*>(gp2DSys) + (gobject->cullingGroup * 8) +  0x6000;
    void **object1addr = static_cast<void**>(object1ptr);
    auto *object1 = static_cast<TitleSequenceObject1*>(*object1addr);
    gobject->active = true;
    gobject->textureIDs[0].textureSlot1 = ModeTitleTexts[geMessageLanguage][object1->currentMode];
    gobject->textureIDs[1].textureSlot1 = 0x1A;
    gobject->textureIDs[0].textureSlot2 = MessageTexts1[geMessageLanguage];
    gobject->textureIDs[1].textureSlot2 = 0xE;
    gobject->textureIDs[0].textureSlot3 = MessageTexts2[geMessageLanguage];
    gobject->textureIDs[1].textureSlot3 = 0xF;
    Sys2d_DrawObject(gobject);
}

ASMDefined void HandleSys2dAnimationObject1(GraphicalObject *object, u32 index, u8* stateptr, u32 incrementAmount);

ASMDefined void GraphicalObjectHandler_EXLoadButtonCSS(GraphicalObject *gobject) {
    void *object1ptr = static_cast<u8*>(gp2DSys) + (gobject->cullingGroup * 8) +  0x6000;
    void **object1addr = static_cast<void**>(object1ptr);
    auto *object1 = static_cast<CSSObject*>(*object1addr);

    const u8 playerIndex = gobject->idStruct.idIndex;
    switch (object1->state) {
        case 0:
            gobject->extraTransformObjectID = 24;
            gobject->unk60 = 5;
            gobject->extraTransformIndex = playerIndex;
            break;
        default:
            if (object1->cssSelectionState[playerIndex] == 0) {
                gobject->active = false;
            } else {
                const Player &player = players[playerIndex];

                if (object1->cssSelectionState[playerIndex] == 1) {
                    // in character select
                    gobject->active = CheckPlayerHoveringOverPossibleEXLoad(player, EXLoadHoverTypes::OnlyCharacter);
                } else if (object1->cssSelectionState[playerIndex] == 2 || object1->cssSelectionState[playerIndex] == 3) {
                    // in gear select
                    gobject->active = CheckPlayerHoveringOverPossibleEXLoad(player, EXLoadHoverTypes::OnlyGear);
                } else {
                    gobject->active = false;
                }
            }
            break;
    }

    // this function handles all of the object's animation per CSS object1's state
    HandleSys2dAnimationObject1(gobject, 0, &object1->state, 0x10000);

    Sys2d_DrawObject(gobject);
}

ASMDefined u32 __cvt_fp2unsigned(f32);                             /* extern */
extern u16 lbl_001D37F8[];
extern f32 lbl_001D3938;
ASMDefined void lbl_000F9038(GraphicalObject *, u32, u32, u32, u32);

constexpr std::array<u16, 7> ExtraDefaultGearTextID = {
        760,
        761,
        762,
        763,
        764,
        765,
        522, // variable character, add new characters' IDs before this
};

constexpr std::array<u16, 1> NewGearTextID = {
        767, // G.U.N. Gear
};

// decompiled
ASMUsed void GraphicalObjectHandler_CSSInformationText(GraphicalObject* gobject) {
    const u8 playerIndex    = gobject->idStruct.idIndex;
	const CSSObject* object = *(static_cast<CSSObject**>(gp2DSys) + ((gobject->cullingGroup * (8/4)) + (0x6000 / 4)));

	switch (object->state) {                        /* irregular */
        case 0:
            gobject->extraTransformObjectID = 0x23;
            gobject->unk60 = 1;
            gobject->extraTransformIndex = -1;
            Sys2d_DrawObject(gobject);
            gobject->unkB4 = __cvt_fp2unsigned(gobject->textureTransform.x);
            gobject->extraTransformObjectID = 0x22;
            gobject->unk60 = 3;
            gobject->extraTransformIndex = -1;
            gobject->extraTransformIndex = playerIndex;
			return;
		case 1:
			return;
        case 2:
            if (object->cssSelectionState[playerIndex] == 3) {
				const Player* player = &players[playerIndex];
                const s8 gear = static_cast<s8>(player->extremeGear);
                gobject->active = true;

                u16 text = 0;
                if (player->character > Character::E10R && player->extremeGear == ExtremeGear::Default) {
                    // any new characters
                    text = ExtraDefaultGearTextID[std::to_underlying(player->character) - std::to_underlying(Character::Silver)];
                } else if (player->extremeGear > ExtremeGear::Cannonball) {
                    // new gears
                    text = NewGearTextID[player->extremeGear - ExtremeGear::GunGear];
                } else if(player->extremeGear == ExtremeGear::ChaosEmerald){
                    text = 548; // default case
                    switch(player->character){
                        case Character::MetalSonic:
                            text = 772;
                            break;
                        case Character::Sonic:
                            text = 768;
                            if (player->gearExload().exLoadID == EXLoad::PerfectNazo){
                                text = 773;
                                break;
                            } else if (player->gearExload().exLoadID == EXLoad::DarkSonic){
                                text = 774;
                                break;
                            } else if (player->gearExload().exLoadID == EXLoad::HyperSonic){
                                text = 775;
                                break;
                            }
                            break;
                        case Character::Knuckles:
                            text = 770;
                            break;
                        case Character::Shadow:
                            text = 771;
                            break;
                        case Character::Tails:
                            text = 769;
                            break;
                        default:
                            break;
                    }
                } else {
                    text = lbl_001D37F8[((static_cast<s8>(player->character) + 0x29) * static_cast<int>(gear == 0) - 1) + gear];
                }

                if (object->visualTrigger[playerIndex] == 1) {
                    const s32 temp_r3 = static_cast<const s32>(gobject->currentAnimationFrame2);
                    gobject->currentAnimationFrame2 = static_cast<s32>(((u32) (-temp_r3 | temp_r3) >> 0xCU) & 0x80000);
                }

                lbl_000F9038(gobject, 0U, 0x10000U, 2U, 0U);

                if (gobject->currentAnimationFrame2 >= 0x80000U) {
                    Sys2d_DrawObject(gobject);
                    RenderText(static_cast<s32>(gobject->textureTransform.x),
                               static_cast<s32>(gobject->textureTransform.y),
                               gobject->unkB0,
                               gobject->unkB4,
                               0U,
                               text,
                               gasSubFont + 1,
                               gpasTexList_SubFont.unk4,
                               lbl_001D3938 + gobject->textureTransform.z);
                }
            } else if (object->cssSelectionState[playerIndex] == 1 && PlayerCharacterInformationBox[playerIndex]) {
                // in character selection
                gobject->active = true;

                if (object->visualTrigger[playerIndex] == 1) {
                    const s32 temp_r3 = static_cast<const s32>(gobject->currentAnimationFrame2);
                    gobject->currentAnimationFrame2 = static_cast<s32>(((u32) (-temp_r3 | temp_r3) >> 0xCU) & 0x80000);
                }

                lbl_000F9038(gobject, 0U, 0x10000U, 2U, 0U);

                Player* player = &players[playerIndex];
                if (gobject->currentAnimationFrame2 >= 0x80000U && player->character < Character::Total) {
                    auto exLoads = player->characterExload();

                    CharacterArchetype text;
                    if (player->hasCharacterExload() && exLoads.archetype() != CharacterArchetype::NoEXLoad && CheckIfEXLoadCanBeApplied(*player, exLoads)) {
                        text = exLoads.archetype();
                    	if (exLoads.exLoadID == EXLoad::Selkadoom) {text = CharacterArchetype{10};} // Special version of combat text
                    } else if (player->character == Character::Emerl) {
                        text = CharacterArchetype{9};
                    } else if (player->character == Character::Shadow) {
                    	text = CharacterArchetype{10};
                    } else {
                        u32 index;
                        if (player->character == Character::E10G &&
                            (exLoads.exLoadID == EXLoad::E10B || exLoads.exLoadID == EXLoad::E10R || exLoads.exLoadID == EXLoad::E10Y)) {
                            index = std::to_underlying(Character::E10R);
                        } else {
                            index = player->character;
                        }
                        text = AllCharacterArchetypes[index];
                    }

                    Sys2d_DrawObject(gobject);

                    RenderText(static_cast<s32>(gobject->textureTransform.x),
                               static_cast<s32>(gobject->textureTransform.y),
                               gobject->unkB0,
                               gobject->unkB4,
                               0U,
                               std::to_underlying(text),
                               &CharacterInformationFont.textDataHeader,
                               gpasTexList_SubFont.unk4,
                               lbl_001D3938 + gobject->textureTransform.z);
                }
            } else {
                gobject->active = false;
                if (object->cssSelectionState[playerIndex] < 3U) {
                    gobject->currentAnimationFrame2 = 0;
                    gobject->unk5C = 0;
                }
            }
            break;
    	default: break;
    }
}