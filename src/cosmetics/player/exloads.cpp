/** \file */

#include "exloads.hpp"
#include "lib/lib.hpp"
#include "skinsystem.hpp"
#include "containers/graphicalobject.hpp"

ASMUsed constexpr std::array<RGBA, EXLoadCount> EXLoadHUDColors = {
        RGBA(0x0), // dummy data
        RGBA(0x471394ff), // perfect nazo
        RGBA(0xcf4731ff), // e10r
        RGBA(0x101010FF), // dark sonic
        RGBA(0xC0C0C0FF), // hyper sonic
        RGBA(0x0), // Stardust Speeder II
        RGBA(0x0), // Hyper Hang-On
        RGBA(0x0), // The Professional
        RGBA(0x0), // Ollie King Gear
        RGBA(0xD2C864FF), // Windmaster Jet
        RGBA(0x1239B8FF), // neo metal sonic
        RGBA(0x0), // E99
        RGBA(0x0), // The Beast
        RGBA(0x0), // e10b, UNUSED - it uses original colors
        RGBA(0x00ffe5ff), // hatsune miku
        RGBA(0x0), // metal sonic scarf
        RGBA(0x4a0015ff), // reala
        RGBA(0x210d00ff), // gongon
        RGBA(0x0), // dummy data
        RGBA(0x0), // dummy data
        RGBA(0x0), // dummy data
        RGBA(0x0), // dummy data
        RGBA(0x0), // dummy data
        RGBA(0x0), // dummy data
        RGBA(0x0), // dummy data
        RGBA(0x0), // dummy data
        RGBA(0x0), // dummy data
        RGBA(0x01056bFF), // seelkadoom
        RGBA(0x0), // super sonic ex load
};

ASMUsed constexpr std::array<EXLoadBoostColor, EXLoadCount> EXLoadBoostColors = {
        {
                // dummy data
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // perfect nazo
                {
                        RGBANormalized::fromNormalized(0.2196078431372549f, 0.0549019607843137f, 0.4588235294117647f),
                        RGBANormalized::fromNormalized(0.0f, 0.0666666701436043f, 1.0f)
                },

                // e10r
                {
                        RGBANormalized::fromNormalized(1.0f, 0.345098f, 0.133333f),
                        RGBANormalized::fromNormalized(0.133333f, 0.7843137254901961f, 1.0f)
                },

                // dark sonic
                {
                        RGBANormalized::fromNormalized(0.0f, 0.0392156862745098f, 0.5411764705882353f),
                        RGBANormalized::fromNormalized(0.5686274509803922f, 0.5294117647058824f, 0.0f)
                },

                // hyper sonic
                {
                        RGBANormalized(255, 255, 255),
                        RGBANormalized(0, 255, 0)
                },

                // Stardust Speeder II
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // Hyper Hang-On
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // The Professional
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // Ollie King Gear
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // Windmaster Jet
                {
                        RGBANormalized::fromNormalized(1.0f, 0.933333337306976f, 0.0f),
                        RGBANormalized::fromNormalized(0.0f, 0.0666666701436043f, 1.0f)
                },

                // neo metal sonic
                {
                        RGBANormalized::fromNormalized(0.1215686274509804f, 0.1490196078431373f, 1.0f),
                        RGBANormalized::fromNormalized(1.0f, 0.9725490196078431f, 0.1215686274509804f)
                },

                // E99
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // The Beast
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // e10b, UNUSED - uses original character boost colors
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // hatsune miku
                {
                        RGBANormalized::fromNormalized(0.2705882352941176f, 1.0f, 0.9254901960784314f),
                        RGBANormalized::fromNormalized(1.0f, 0.2705882352941176f, 0.3450980392156863f)
                },

                // metal sonic scarf
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // reala
                {
                        RGBANormalized::fromNormalized(0.8f, 0.0f, 0.0352941176470588f),
                        RGBANormalized::fromNormalized(0.0f, 0.8f, 0.7647058823529412f)
                },

                // gongon
                {
                        RGBANormalized::fromNormalized(0.5294117647058824f, 0.3490196078431373f, 0.2745098039215686f),
                        RGBANormalized::fromNormalized(0.2745098039215686f, 0.4549019607843137f, 0.5294117647058824f)
                },

                // dummy data
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // dummy data
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // dummy data
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // dummy data
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // dummy data
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // dummy data
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // dummy data
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // dummy data
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // dummy data
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

                // seelkadoom
                {
                        RGBANormalized(0, 10, 255),
                        RGBANormalized(84, 84, 84)
                },

                // super sonic ex load
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },
        }
};

ASMDefined void *const gp2DSys;

/**
 * Saves currently selected EX load information for each Player.
 * For each Player, there is an array with size 2.
 * Index 0 corresponds to the specific character EX load (refer to EXLoadInfo).
 * Index 1 corresponds to the specific gear EX load (refer to EXLoadInfo).
 */
m2darray<CurrentEXLoadInfo, MaxPlayerCount, 2> Player_EXLoadData;

/**
 * Gets a specific Player's chosen EX load ID from ::EXLoads, if any is selected.
 *
 * @param player The Player to fetch from.
 * @param exLoads Where the result of the function is saved. If no EX load is selected, ::NoneEXLoad is returned.
 */
[[nodiscard]] EnabledEXLoads FetchEnabledEXLoadIDs(const Player &player) {// Change into member function? or possibly the constructor
	EnabledEXLoads exLoads{};
	if(player.aiControl && player.playerType) { return exLoads; }

	const u8 &controllerPort = player.input->port;

	u16 exLoadIndex = Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex;
	if(exLoadIndex != 0U) {
		exLoads.characterExLoadID = CharacterEXLoadDataSlots[exLoadIndex].exLoadID;
	}
	exLoadIndex = Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadIndex;
	if(exLoadIndex != 0U) {
		exLoads.gearExLoadID = GearEXLoadDataSlots[exLoadIndex].exLoadID;
	}

	return exLoads;
}

ASMUsed void FetchEnabledEXLoadIDs(const Player *player, EnabledEXLoads &exLoads) {
	if(player->aiControl && player->playerType) { return; }

	exLoads = FetchEnabledEXLoadIDs(*player);
}

/**
 * Checks if a Player currently has any of the E10 robot EX loads selected.
 *
 * @param player The Player to check.
 */
ASMUsed bool IsAnyE10EXLoad(const Player *player) {
	auto exLoads = FetchEnabledEXLoadIDs(*player);
	return (exLoads.characterExLoadID == E10BEXLoad || exLoads.characterExLoadID == E10REXLoad);
}

/**
 * Checks if a Player currently has Super Sonic EX load selected.
 *
 * @param player The Player to check.
 */
ASMUsed bool SYBIsSuperSonicEXLoad(const Player *player) {
	auto exLoads = FetchEnabledEXLoadIDs(*player);
	return (exLoads.characterExLoadID == SSEXLoad);
}

/**
 * Checks a Player for if the currently selected EX load has any custom HUD colors available.
 *
 * @param player The Player to check.
 * @return The result of the function gets saved in here. If no applicable HUD colors are found, it returns 0.
 */
[[nodiscard]] EXLoadColors CheckEXLoadHUDColors(const Player &player) {
	if(player.aiControl && player.playerType) { return {}; }
	if(FetchEnabledEXLoadIDs(player).characterExLoadID == E10BEXLoad) { return {}; }

	const u8 &controllerPort = player.input->port;

	u16 exLoadIndex = Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex;
	if(exLoadIndex != 0U) {
		if(EXLoadHUDColors[CharacterEXLoadDataSlots[exLoadIndex].exLoadID]) {
			return {(void *) EXLoadHUDColors.data(), CharacterEXLoadDataSlots[exLoadIndex].exLoadID};
		}
	}
	exLoadIndex = Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadIndex;
	if(exLoadIndex != 0U) {
		if(EXLoadHUDColors[GearEXLoadDataSlots[exLoadIndex].exLoadID]) {
			return {(void *) EXLoadHUDColors.data(), GearEXLoadDataSlots[exLoadIndex].exLoadID};
		}
	}
	return {};
}

/**
 * Checks a Player for if the currently selected EX load has any custom HUD colors available.
 *
 * @param player The Player to check.
 * @param returnValues The result of the function gets saved in here. If no applicable HUD colors are found, it returns 0.
 */
void CheckEXLoadHUDColors(const Player *player, EXLoadColors &returnValues) {
	returnValues = CheckEXLoadHUDColors(*player);
}

/**
 * Checks a Player for if the currently selected EX load has any custom boost colors available.
 *
 * @param player The Player to check.
 * @return The result of the function gets saved in here. If no applicable boost colors are found, it returns 0.
 */
[[nodiscard]] EXLoadColors CheckEXLoadBoostColors(const Player &player) {
	if(player.aiControl && player.playerType) { return {}; }

	if(FetchEnabledEXLoadIDs(player).characterExLoadID == E10BEXLoad) { return {}; }

	const u8 &controllerPort = player.input->port;

	u16 exLoadIndex = Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex;
	if(exLoadIndex != 0U) {
		if(EXLoadBoostColors[CharacterEXLoadDataSlots[exLoadIndex].exLoadID].rgbaFirstLayer) {
			return {(void *) EXLoadBoostColors.data(), CharacterEXLoadDataSlots[exLoadIndex].exLoadID};
		}
	}
	exLoadIndex = Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadIndex;
	if(exLoadIndex != 0U) {
		if(EXLoadBoostColors[GearEXLoadDataSlots[exLoadIndex].exLoadID].rgbaFirstLayer) {
			return {(void *) EXLoadBoostColors.data(), GearEXLoadDataSlots[exLoadIndex].exLoadID};
		}
	}
	return {};
}

/**
 * Checks a Player for if the currently selected EX load has any custom boost colors available.
 *
 * @param player The Player to check.
 * @param returnValues The result of the function gets saved in here. If no applicable boost colors are found, it returns 0.
 */
void CheckEXLoadBoostColors(const Player *player, EXLoadColors &returnValues) {
	returnValues = CheckEXLoadBoostColors(*player);
}

#include "lib/stdlib.hpp"
#include "macros.h"

ASMUsed void _CheckEXLoadHUDColors() {
	asm(R"(
	        stwu r1, -0x90 (r1)
	        stw r0, 0x8 (r1)
	        mflr r0
	        stw r0, 0xC (r1)
	        stmw r5, 0x10 (r1)
	        bl %[HUDColors]
	        lwz r0, 0xC (r1)
	        mtlr r0
	        lwz r0, 0x8 (r1)
	        lmw r5, 0x10 (r1)
	        addi r1, r1, 0x90
			)"
	    :
	    : [HUDColors] "i"(static_cast<void (*)(const Player *, EXLoadColors &)>(CheckEXLoadHUDColors)));
}

ASMUsed void _CheckEXLoadBoostColors() {
	asm(R"(
	        stwu r1, -0x90 (r1)
	        stw r0, 0x8 (r1)
	        mflr r0
	        stw r0, 0xC (r1)
	        stmw r5, 0x10 (r1)
	        bl %[BoostColors]
	        lwz r0, 0xC (r1)
	        mtlr r0
	        lwz r0, 0x8 (r1)
	        lmw r5, 0x10 (r1)
	        addi r1, r1, 0x90
			)"
	    :
	    : [BoostColors] "i"(static_cast<void (*)(const Player *, EXLoadColors &)>(CheckEXLoadBoostColors)));
}

/**
 * Checks a Player's controls to see if they pressed the required buttons to cycle through EX loads on the CSS.
 *
 * @param player The Player to check.
 * @return The EXLoadTypes result for which type of cycling needs to be performed.
 */
[[nodiscard]] inline EXLoadTypes CheckEXLoadControls(const Player &player) {
	if(player.input->holdFaceButtons.hasAny(LButton)) {
		return AllEXLoadTypes;
	}
	if(player.input->rightStickHorizontal > 10) {
		return LLoadTypeForwards;
	}
	if(player.input->rightStickHorizontal < -10) {
		return LLoadTypeBackwards;
	}
	if(player.input->rightStickVertical > 10) {
		return EXLoadTypeForwards;
	}
	if(player.input->rightStickVertical < -10) {
		return EXLoadTypeBackwards;
	}

	return NoneEXLoadType;
}

inline void ApplyVisual(CSSObject &object) {
	object.visualTrigger[0] = TRUE; // object that is passed in is already aligned to the player index, no need to apply index in here
	object.visualTriggerTime = 1;
	lbl_0013FA84(0xA9000000);
	lbl_0013FA84(0xA9000800);
}

/**
 * This function checks whenever the Player is pressing any of the EX load buttons outlined in CheckEXLoadControls().
 * If they are, it'll call the sound effect and visuals for when characters/gears are switched naturally on CSS.
 *
 * @param player The Player to apply the visuals to.
 * @param object Reference to the Object used for handling the CSS.
 */
USED void ApplyEXLoadFancyVisuals(const Player &player, CSSObject &object) {
	const u8 controllerPort = player.input->port;

	if(Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadDelayTime == 0 && Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadDelayTime == 0) {
		if(CheckEXLoadControls(player) != NoneEXLoadType) {
			ApplyVisual(object);
		}
	}

	// For the skin system
	// if (PlayerSkinSystemData[controllerPort].delay == 0) {
	// 	if (CheckSkinSystemControls(player) != NoneSkinSystemType) {
	// 		ApplyVisual(object);
	// 	}
	// }
}

/**
 * Checks if a Player can use the provided EXLoadInfo.
 *
 * @param player The Player to check.
 * @param info The EXLoadInfo struct to check against.
 * @return
 */
[[nodiscard]] bool CheckIfEXLoadCanBeApplied(const Player &player, const EXLoadInfo &info) {
	bool canApplyCharacter;
	if(info.character != -1) {
		// quick exception for e10b character id and super sonic ex load, those exloads are always allowed through
		canApplyCharacter = info.character == player.character ||
                (info.character == E10G && player.character == E10R) ||
                (info.character == Sonic && player.character == SuperSonic);
	} else {
		canApplyCharacter = true;
	}

	bool canApplyGear;
	if(info.gear != -1) {
		canApplyGear = info.gear == player.extremeGear;
	} else {
		canApplyGear = true;
	}

	return canApplyCharacter && canApplyGear;
}

/**
 * This function takes care of updating #Player_EXLoadData whenever a Player scrolls through possible non-applicable EX loads, and sets it back to 0.
 *
 * @param player The Player to update.
 */
ASMUsed void UpdateEXLoadData(Player *player) {
	EXLoadInfo info;// NOLINT(cppcoreguidelines-pro-type-member-init)
	const u8 &controllerPort = player->input->port;

	u16 exLoadIndex = Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex;
	if(exLoadIndex != 0U) {
		info = CharacterEXLoadDataSlots[exLoadIndex];

		if(!CheckIfEXLoadCanBeApplied(*player, info)) {
			Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex = 0;
		}
	}
	exLoadIndex = Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadIndex;
	if(exLoadIndex != 0U) {
		info = GearEXLoadDataSlots[exLoadIndex];

		if(!CheckIfEXLoadCanBeApplied(*player, info)) {
			Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadIndex = 0;
		}
	}
}

/**
 * Gets the next applicable EX load via "scrolling" through the specified EX load slot array.
 *
 * @param player The Player whose EX loads to scroll.
 * @param exLoadDataSlots The applicable EX load slots.
 * @param exLoadMode Which subset type the provided slots are (refer to ::EXLoadModes).
 * @param exLoadType Which scrolling behavior needs to be used. This value can be obtained from CheckEXLoadControls().
 * @return The newly rotated index that corresponds to the array parameter.
 */
[[nodiscard]] u32 RotateEXLoadAndFetch(const Player &player, std::span<const EXLoadInfo> exLoadDataSlots, const EXLoadModes &exLoadMode, const EXLoadTypes &exLoadType) {
	// set the correct ex load type to check if it can be applied, so that it corresponds to the enum in the slot structs
	EXLoadTypes checkType = NoneEXLoadType;
	if(exLoadType == LLoadTypeForwards || exLoadType == LLoadTypeBackwards) {
		checkType = LLoadType;
	} else if(exLoadType == EXLoadTypeForwards || exLoadType == EXLoadTypeBackwards) {
		checkType = EXLoadType;
	}

	const u8 controllerPort = player.input->port;
	u32 index = Player_EXLoadData[controllerPort][exLoadMode].exLoadIndex;
	bool toRotate = true;
	while(toRotate) {
		if(exLoadType == AllEXLoadTypes || exLoadType == LLoadTypeForwards || exLoadType == EXLoadTypeForwards) {
			index += 1;
			if(index >= exLoadDataSlots.size()) { index = 0; }
		} else if(exLoadType == LLoadTypeBackwards || exLoadType == EXLoadTypeBackwards) {
			index -= 1;
			if(static_cast<s32>(index) < 0) { index = exLoadDataSlots.size() - 1; }
		}

		const EXLoadInfo &current = exLoadDataSlots[index];

		if(current.exLoadType == NoneEXLoadType) {
			toRotate = false;
		}

		else if(current.exLoadType == checkType || exLoadType == AllEXLoadTypes) {
			toRotate = !CheckIfEXLoadCanBeApplied(player, current);
		}
	};

	return index;
}

/**
 * Takes care of updating #Player_EXLoadData every time a Player scrolls through the possible applicable EX loads.
 *
 * @param player The Player to update.
 * @param exLoadMode Which type of EX load slots to apply and scroll through (refer to ::EXLoadModes).
 * @param exLoadType Which scrolling behavior needs to be used. This value can be obtained from CheckEXLoadControls().
 */
void ApplyNextEXLoad(const Player &player, const EXLoadModes &exLoadMode, const EXLoadTypes &exLoadType) {
	u32 exLoadIndex = 0;
	if(exLoadMode == CharacterEXLoadMode) {
		exLoadIndex = RotateEXLoadAndFetch(player, CharacterEXLoadDataSlots, exLoadMode, exLoadType);
	} else if(exLoadMode == GearEXLoadMode) {
		exLoadIndex = RotateEXLoadAndFetch(player, GearEXLoadDataSlots, exLoadMode, exLoadType);
	}

	if(exLoadMode < 2) {
		const u8 controllerPort = player.input->port;

		Player_EXLoadData[controllerPort][exLoadMode].exLoadIndex = exLoadIndex;
		Player_EXLoadData[controllerPort][exLoadMode].exLoadDelayTime = 15;
	}
}

/**
 * Checks whether the Player wishes to scroll through all applicable EX loads.
 * Also takes care of making sure there is a delay between spamming the EX load scroll controls.
 *
 * @param player The Player to check.
 * @param exLoadMode Which type of EX load slots to check against (refer to ::EXLoadModes).
 */
void CheckPlayerEXLoadButton(const Player &player, const EXLoadModes &exLoadMode) {
	const u8 controllerPort = player.input->port;
	u8 &delayTime = Player_EXLoadData[controllerPort][exLoadMode].exLoadDelayTime;

	if(delayTime == 0) {
		const EXLoadTypes type = CheckEXLoadControls(player);
		if(type != NoneEXLoadType) {
			ApplyNextEXLoad(player, exLoadMode, type);
		}
	}

	delayTime = static_cast<u8>(clamp<s8>(static_cast<s8>(delayTime - 1), 0));
}

/**
 * Updates the CSS graphics to display the character portrait if EX loads are selected.
 *
 * @param object The GraphicalObject that handles character portrait textures.
 */
USED void Character_UpdateGraphicalEXLoad(GraphicalObject *object) {
	if(object->active == 0) { return; }

	const Player &player = players[object->idStruct.idIndex];
	CheckPlayerEXLoadButton(player, CharacterEXLoadMode);

	const u8 &controllerPort = player.input->port;
	const u8 &exLoadIndex = Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex;

	const EXLoadInfo &info = CharacterEXLoadDataSlots[exLoadIndex];

	if(exLoadIndex != 0) {
		if(CheckIfEXLoadCanBeApplied(player, info)) {
			if(IsAnyE10EXLoad(&player) && player.character == E10R) { return; }

			if (static_cast<s16>(info.portraitTextureID) != -1)
				object->textureIDs[0].textureSlot1 = info.portraitTextureID;

            if (static_cast<s16>(info.characterTextTextureID) != -1)
                object->textureIDs[0].textureSlot2 = info.characterTextTextureID;

            if (info.characterType != DefaultTypeCharacter)
                object->textureIDs[0].textureSlot3 = info.characterType + 114;
		}
	}
}

/**
 * Updates the CSS graphics to display the gear portrait and gear text if EX loads are selected.
 *
 * @param object The GraphicalObject that handles the aforementioned textures.
 */
USED void Gear_UpdateGraphicalEXLoad(GraphicalObject *object) {
	// not locked in gear selection object uses textureSlot1 for portrait, textureSlot5 for gear text
	// locked in gear selection object uses textureSlot4 for gear text

	if(object->active == 0) { return; }

	const Player &player = players[object->idStruct.idIndex];
	const u8 &controllerPort = player.input->port;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-reinterpret-cast"
	//void *objectptr = reinterpret_cast<u8 *>(gp2DSys) + 0x6038;
	//void **objectptr2 = reinterpret_cast<void **>(objectptr);
	//auto *cssObject = reinterpret_cast<CSSObject *>(*objectptr2);
	void **objectptr = reinterpret_cast<void **>(static_cast<u8 *>(gp2DSys) + 0x6038);
	auto *cssObject = static_cast<CSSObject *>(*objectptr);
#pragma clang diagnostic pop

	if(cssObject->cssSelectionState[controllerPort] < 4) {
		CheckPlayerEXLoadButton(player, GearEXLoadMode);
	}

	const auto &exLoadIndex = Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadIndex;
	const auto &info = GearEXLoadDataSlots[exLoadIndex];

    const auto &characterExLoadIndex = Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex;
	const auto &characterInfo = CharacterEXLoadDataSlots[characterExLoadIndex];

	if(object->idStruct.graphicalDataID == NotLockedGearSelectionID) {
        if (characterExLoadIndex != 0) {
            // character ex load is applied, need to check if their default gear is different and if they have different character text texture

            if (CheckIfEXLoadCanBeApplied(player, characterInfo)) {
                if (player.extremeGear == ExtremeGear::Default) {
                    if (static_cast<s16>(characterInfo.defaultGearPortraitTextureID) != -1)
                        object->textureIDs[0].textureSlot1 = characterInfo.defaultGearPortraitTextureID;

                    if (static_cast<s16>(characterInfo.gearTextTextureID) != -1)
                        object->textureIDs[0].textureSlot5 = characterInfo.gearTextTextureID;
                }

                if (static_cast<s16>(characterInfo.characterTextTextureID) != -1)
                    object->textureIDs[0].textureSlot2 = characterInfo.characterTextTextureID;
            }
        }
        if (exLoadIndex != 0) {
            // gear ex load overrides character ex load's default gear

			if(CheckIfEXLoadCanBeApplied(player, info)) {
				if(static_cast<s16>(info.portraitTextureID) != -1) {
                    object->textureIDs[0].textureSlot1 = info.portraitTextureID;
				}

				if(static_cast<s16>(info.portraitTextureID) != -1) {
                    object->textureIDs[0].textureSlot5 = info.gearTextTextureID;
				}
			}
		}
	} else if (object->idStruct.graphicalDataID == LockedGearSelectionID) {
        if (characterExLoadIndex != 0) {
            // character ex load is applied, need to check if their default gear is different and if they have different character text texture

            if (CheckIfEXLoadCanBeApplied(player, characterInfo)) {
                if (player.extremeGear == ExtremeGear::Default) {
                    if (static_cast<s16>(characterInfo.gearTextTextureID) != -1)
                        object->textureIDs[0].textureSlot4 = characterInfo.gearTextTextureID;
                }

                if (static_cast<s16>(characterInfo.characterTextTextureID) != -1)
                    object->textureIDs[0].textureSlot1 = characterInfo.characterTextTextureID;
            }
        }
		if (exLoadIndex != 0) {
            // gear ex load overrides character ex load's default gear

			if(CheckIfEXLoadCanBeApplied(player, info)) {
				if(static_cast<s16>(info.portraitTextureID) != -1) {
                    object->textureIDs[0].textureSlot4 = info.gearTextTextureID;
				}
			}
		}
	}
}

/**
 * Takes care of binding Hatsune Miku's leek she holds at level 3 to her right hand whenever she performs her attack.
 * Only used on the ::HatsuneMikuEXLoad EX load.
 *
 * @param player The Player to apply binding to.
 * @param savePtr Where to save the leek's matrix multiplication result.
 */
ASMUsed void HatsuneMiku_BindLeekToBoneLvl3Attack(Player &player, Matrix3x3F &savePtr) {
	auto exLoads = FetchEnabledEXLoadIDs(player);
	if(exLoads.characterExLoadID == HatsuneMikuEXLoad) {
		Matrix3x3F *mtxList = gpsaMtxList_Player[player.index];
		const u32 rightHandBone = 43;
		nnMultiplyMatrix(&player.unkC4, mtxList + rightHandBone, savePtr);
	}
}

/**
 * Gets the player model's filename for super form EX loads.
 * Used for all EX loads that are applied to Chaos Emerald.
 *
 * @param player The Player to get the result from.
 * @param filename The original filename.
 * @return The new filename, whether that be modified from the original or not.
 */
USED const char *GetEXLoadFilenameSuperForm(Player *player, const char filename[5]) {
	auto exLoads = FetchEnabledEXLoadIDs(*player);

	if(player->extremeGear != ExtremeGear::ChaosEmerald) {
		return filename;
	}

	switch(exLoads.gearExLoadID) {
		case PerfectNazo:
			filename = "PN18";
			break;
		case HyperSonicEXLoad:
			filename = "PH18";
			break;
		case DarkSonicEXLoad:
			filename = "PG18";
			break;
		default:
			break;
	}

	return filename;
}

/**
 * Gets the player model's filename for all EX loads except super form EX loads.
 * Used for all EX loads that aren't Chaos Emerald EX loads.
 *
 * @param player The Player to get the result from.
 * @param gearType The type of gear the Player is using.
 * @return nullptr if the EX load doesn't have a model assigned to it, otherwise the filename of the model.
 */
[[nodiscard]] const char *GetEXLoadCharacterModel(Player *player, const GearType gearType) {
	switch(FetchEnabledEXLoadIDs(*player).characterExLoadID) {
		case HatsuneMikuEXLoad:
			return "EC0";
		case MetalSonicNoScarfEXLoad:
            if (gearType == Bike) {
                return "EC1B";
            }
			return "EC1";
		case RealaEXLoad:
			return "EC2";
		case GonGonEXLoad:
			return "EC3";
        case NeoMetalEXLoad:
            return "EC4";
        case SelkadoomEXLoad:
            if (gearType == Bike) {
                return "EC5B";
            }
            return "EC5";
		default:
			return nullptr;
	}
}

/**
 * Gets the first two starting letters of an EX load's skate models' filename, if applicable.
 * The model ID of the skates then get appended to the end of this filename.
 * For example, if ::RealaEXLoad is picked alongside ::Darkness, the final filename would become "S270".
 *
 * @param player The Player to get the result from.
 * @return nullptr if the EX load doesn't have a model assigned to it, otherwise the start of the model's filename.
 */
[[nodiscard]] const char *GetEXLoadSkateModelStart(Player *player) {
	switch(FetchEnabledEXLoadIDs(*player).characterExLoadID) {
		case HatsuneMikuEXLoad:
			return "S0";
        case MetalSonicNoScarfEXLoad:
            return "S1";
		case RealaEXLoad:
			return "S2";
		case GonGonEXLoad:
			return "S3";
        case SelkadoomEXLoad:
            return "S5";
		default:
			return nullptr;
	}
}

/**
 * Checks whether the Player is hovering over a character or gear that has available EX loads assigned to them.
 *
 * @param player The Player to check.
 * @param type Specifies whether to check character or gear EX loads only.
 * @return true if the Player is hovering over an EX load, otherwise false.
 */
[[nodiscard]] bool CheckPlayerHoveringOverPossibleEXLoad(const Player &player, EXLoadHoverTypes type) {
	// Manually make these spans have dynamic_extent, so they have the same type
	const std::span characterEXLoadDataSlots = {CharacterEXLoadDataSlots.data(), CharacterEXLoadDataSlots.size()};
	const std::span gearEXLoadDataSlots = {GearEXLoadDataSlots.data(), GearEXLoadDataSlots.size()};

	const std::span exLoadDataSlots = type == OnlyCharacter ? characterEXLoadDataSlots : gearEXLoadDataSlots;

	for(const auto &slot: exLoadDataSlots) {
		if(slot.exLoadID == NoneEXLoad) { continue; }

		if(CheckIfEXLoadCanBeApplied(player, slot)) { return true; }
	}
	return false;
}

/// Used to clear #Player_EXLoadData when necessary.
ASMUsed void ClearPlayerEXLoadBSS() {
	TRK_memset(Player_EXLoadData.data(), 0, sizeof(Player_EXLoadData));
	//std::fill(, Player_EXLoadData.end(), 0);
}

/*
 * Called when the START button is pressed and the game is started from the CSS.
 * Takes care of very special EX loads, that don't need an EX load ID set to them (like Super Sonic EX load).
 */
ASMUsed void ClearSpecificEXLoads() {
    for (u32 i = 0; i < MaxPlayerCount; i++) {
        auto &charExLoadInfo = Player_EXLoadData[i][CharacterEXLoadMode];
        if (CharacterEXLoadDataSlots[charExLoadInfo.exLoadIndex].exLoadID == SSEXLoad) {
            charExLoadInfo.exLoadIndex = 0;
        }
    }
}