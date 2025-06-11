/** \file */

#include "exloads.hpp"
#include "ninjanext.hpp"
#include "skinsystem.hpp"
#include "containers/graphicalobject.hpp"
#include "lib/lib.hpp"
#include "riders/gamemode.hpp"

ASMDefined void *const gp2DSys;

/**
 * Saves currently selected EX load information for each Player.
 */
std::array<CurrentEXLoadInfo, MaxPlayerCount> Player_EXLoadData;

/**
 * Gets a specific Player's chosen EX load ID from ::EXLoad, if any is selected.
 *
 * @param player The Player to fetch from.
 * @param exLoads Where the result of the function is saved. If no EX load is selected, EXLoad::None is returned.
 * @deprecated Please use @ref Player::characterExload()
 */
[[nodiscard,deprecated]] EnabledEXLoads FetchEnabledEXLoadIDs(const Player &player) {
	if((player.aiControl && player.playerType) || !player.hasExload()) { return {}; }

	EnabledEXLoads exLoads{
		.characterExLoadID = player.characterExload().exLoadID,
		.gearExLoadID = player.gearExload().exLoadID,
	};

	return exLoads;
}

/**
 *
 * @copydoc FetchEnabledEXLoadIDs(const Player &)
 */
ASMUsed [[deprecated]] void FetchEnabledEXLoadIDs(const Player *player, EnabledEXLoads &exLoads) {
	if(player->aiControl && player->playerType) { return; }

	exLoads = FetchEnabledEXLoadIDs(*player);
}

/**
 * Checks if a Player currently has any of the E10 robot EX loads selected.
 *
 * @param player The Player to check.
 */
ASMUsed bool IsAnyE10EXLoad(const Player &player) {
	auto exLoads = player.characterExload();
	return exLoads.exLoadID == EXLoad::E10B || exLoads.exLoadID == EXLoad::E10R || exLoads.exLoadID == EXLoad::E10Y;
}

/**
 * Checks if a Player currently has Super Sonic EX load selected.
 *
 * @param player The Player to check.
 */
ASMUsed bool SYBIsSuperSonicEXLoad(const Player &player) {
	return player.characterExload().exLoadID == EXLoad::SuperSonic;
}

/**
 * Checks a Player for if the currently selected EX load has any custom HUD colors available.
 *
 * @param player The Player to check.
 * @return The result of the function gets saved in here. If no applicable HUD colors are found, it returns 0.
 */
[[nodiscard]] EXLoadColors CheckEXLoadHUDColors(const Player &player) {
	if(player.aiControl && player.playerType) { return {}; }
	if(!player.hasExload()) { return {}; }

	const auto &characterExload = player.characterExload();

	if(characterExload.exLoadID == EXLoad::E10B) { return {}; }
	if(characterExload.hudColor()) {
		return characterExload.exloadColor();
	}

	const auto &gearExload = player.gearExload();
	if(gearExload.hudColor()) {
		return gearExload.exloadColor();
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
	if(!player.hasExload()) { return {}; }

	const auto &characterExload = player.characterExload();
	if(characterExload.exLoadID == EXLoad::E10B) { return {}; }

	if(characterExload.boostColor().rgbaFirstLayer) {
		return characterExload.exloadBoostColor();
	}

	const auto &gearExload = player.gearExload();
	if(gearExload.boostColor().rgbaFirstLayer) {
		return gearExload.exloadBoostColor();
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

#include "macros.h"
#include "lib/stdlib.hpp"

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
	if(player.input->holdFaceButtons.hasAny(Buttons::L)) {
		return EXLoadTypes::AllTypes;
	}
	if(player.input->rightStickHorizontal > 10) {
		return EXLoadTypes::CosmeticForwards;
	}
	if(player.input->rightStickHorizontal < -10) {
		return EXLoadTypes::CosmeticBackwards;
	}
	if(player.input->rightStickVertical > 10) {
		return EXLoadTypes::GameplayAlteringForwards;
	}
	if(player.input->rightStickVertical < -10) {
		return EXLoadTypes::GameplayAlteringBackwards;
	}

	return EXLoadTypes::None;
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
ASMUsed void ApplyEXLoadFancyVisuals(const Player &player, CSSObject &object) {
	const u8 controllerPort = player.input->port;

	if(Player_EXLoadData[controllerPort].delayTime == 0) {
		if(CheckEXLoadControls(player) != EXLoadTypes::None) {
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
	if(info.character != Character::Invalid) {
		// quick exception for e10b character id and super sonic ex load, those exloads are always allowed through
		canApplyCharacter = info.character == player.character ||
                (info.character == Character::E10G && player.character == Character::E10R) ||
                (info.character == Character::Sonic && player.character == Character::SuperSonic);
	} else {
		canApplyCharacter = true;
	}

	bool canApplyGear;
	if(info.gear != Character::Invalid) {
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
ASMUsed void UpdateEXLoadData(Player &player) {
	auto &exload = player.exload();
	if(player.hasCharacterExload()) {
		if(!CheckIfEXLoadCanBeApplied(player, *exload.characterExload)) {
			exload.characterExload = {};
		}
	}
	if(player.hasGearExload()) {
		if(!CheckIfEXLoadCanBeApplied(player, *exload.gearExload)) {
			exload.gearExload = {};
		}
	}
}

/**
 * Gets the next applicable EX load via "scrolling" through the specified EX load slot array.
 *
 * @param player The Player whose EX loads to scroll.
 * @param exLoadDataSlots The applicable EX load slots.
 * @param exLoadMode Which subset type the provided slots are (refer to ::EXLoadMode).
 * @param exLoadType Which scrolling behavior needs to be used. This value can be obtained from CheckEXLoadControls().
 * @return The newly rotated index that corresponds to the array parameter.
 */
[[nodiscard]] void RotateEXLoadAndFetch(Player &player, std::span<const EXLoadInfo> exLoadDataSlots, const EXLoadMode &exLoadMode, const EXLoadTypes &exLoadType) {
	// set the correct ex load type to check if it can be applied, so that it corresponds to the enum in the slot structs
	EXLoadTypes checkType = EXLoadTypes::None;
	if(exLoadType == EXLoadTypes::CosmeticForwards || exLoadType == EXLoadTypes::CosmeticBackwards) {
		checkType = EXLoadTypes::Cosmetic;
	} else if(exLoadType == EXLoadTypes::GameplayAlteringForwards || exLoadType == EXLoadTypes::GameplayAlteringBackwards) {
		checkType = EXLoadTypes::GameplayAltering;
	}

	auto &exload = player.exload();
	auto &ptr = exLoadMode == CharacterEXLoadMode ? exload.characterExload : exload.gearExload;
	if(ptr == nullptr) {
		ptr = exLoadDataSlots.data();
	}
	bool toRotate = true;
	while(toRotate) {
		if(exLoadType == EXLoadTypes::AllTypes || exLoadType == EXLoadTypes::CosmeticForwards || exLoadType == EXLoadTypes::GameplayAlteringForwards) {
			if(++ptr > &exLoadDataSlots.back()) {
				ptr = exLoadDataSlots.data();
			}
		} else if(exLoadType == EXLoadTypes::CosmeticBackwards || exLoadType == EXLoadTypes::GameplayAlteringBackwards) {
			if(--ptr < exLoadDataSlots.data()) {
				ptr = &exLoadDataSlots.back();
			}
		}

		const EXLoadInfo &current = *ptr;

		if(current.exLoadType == EXLoadTypes::None) {
			ptr = nullptr;
			break;
		}
		if(current.exLoadType == checkType || exLoadType == EXLoadTypes::AllTypes) {
			toRotate = !CheckIfEXLoadCanBeApplied(player, current);
		}
	};
}

/**
 * Takes care of updating #Player_EXLoadData every time a Player scrolls through the possible applicable EX loads.
 *
 * @param player The Player to update.
 * @param exLoadMode Which type of EX load slots to apply and scroll through (refer to ::EXLoadMode).
 * @param exLoadType Which scrolling behavior needs to be used. This value can be obtained from CheckEXLoadControls().
 */
void ApplyNextEXLoad(Player &player, const EXLoadMode &exLoadMode, const EXLoadTypes &exLoadType) {
	switch (exLoadMode) {
		default: return;
		case EXLoadMode::CharacterEXLoadMode:
			RotateEXLoadAndFetch(player, CharacterEXLoadDataSlots, exLoadMode, exLoadType);
			break;
		case EXLoadMode::GearEXLoadMode:
			RotateEXLoadAndFetch(player, GearEXLoadDataSlots, exLoadMode, exLoadType);
			break;
	}
	player.exload().delayTime = CurrentEXLoadInfo::MaxDelayTime;
}

/**
 * Checks whether the Player wishes to scroll through all applicable EX loads.
 * Also takes care of making sure there is a delay between spamming the EX load scroll controls.
 *
 * @param player The Player to check.
 * @param exLoadMode Which type of EX load slots to check against (refer to ::EXLoadMode).
 */
void CheckPlayerEXLoadButton(Player &player, const EXLoadMode &exLoadMode) {
	u8 &delayTime = player.exload().delayTime;

	if(delayTime == 0) {
		const EXLoadTypes type = CheckEXLoadControls(player);
		if(type != EXLoadTypes::None) {
			ApplyNextEXLoad(player, exLoadMode, type);
		}
	} else if(--delayTime > CurrentEXLoadInfo::MaxDelayTime) [[unlikely]]{
		delayTime = 0;
	}
}

/**
 * Updates the CSS graphics to display the character portrait if EX loads are selected.
 *
 * @param object The GraphicalObject that handles character portrait textures.
 */
ASMUsed void Character_UpdateGraphicalEXLoad(GraphicalObject *object) {
	if(object->active == 0) { return; }

	Player &player = players[object->idStruct.idIndex];
	CheckPlayerEXLoadButton(player, EXLoadMode::CharacterEXLoadMode);

	if(player.hasCharacterExload()) {
		const EXLoadInfo &info = player.characterExload();
		if(CheckIfEXLoadCanBeApplied(player, info)) {
			if(IsAnyE10EXLoad(player) && player.character == Character::E10R) { return; }

			if (static_cast<s16>(info.portraitTextureID) != -1){
				object->textureIDs[0].textureSlot1 = info.portraitTextureID;
			}

            if (static_cast<s16>(info.characterTextTextureID) != -1){
				object->textureIDs[0].textureSlot2 = info.characterTextTextureID;
			}

            if (info.characterType != CharacterType::DefaultTypeCharacter){
				const auto characterType = std::to_underlying(info.characterType);
				object->textureIDs[0].textureSlot3 = characterType + 114;
			}
		}
	}
}

/**
 * Updates the CSS graphics to display the gear portrait and gear text if EX loads are selected.
 *
 * @param object The GraphicalObject that handles the aforementioned textures.
 */
ASMUsed void Gear_UpdateGraphicalEXLoad(GraphicalObject *object) {
	// not locked in gear selection object uses textureSlot1 for portrait, textureSlot5 for gear text
	// locked in gear selection object uses textureSlot4 for gear text

	if(object->active == 0) { return; }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-reinterpret-cast"
	//void *objectptr = reinterpret_cast<u8 *>(gp2DSys) + 0x6038;
	//void **objectptr2 = reinterpret_cast<void **>(objectptr);
	//auto *cssObject = reinterpret_cast<CSSObject *>(*objectptr2);
	void **objectptr = reinterpret_cast<void **>(static_cast<u8 *>(gp2DSys) + 0x6038);
	auto *cssObject = static_cast<CSSObject *>(*objectptr);
#pragma clang diagnostic pop

	Player &player = players[object->idStruct.idIndex];
	const u8 &controllerPort = player.input->port;

	if(cssObject->cssSelectionState[controllerPort] < 4) {
		CheckPlayerEXLoadButton(player, EXLoadMode::GearEXLoadMode);
	}

	const auto &info = player.gearExload();
	const auto &characterInfo = player.characterExload();

	if(object->idStruct.graphicalDataID == NotLockedGearSelectionID) {
        if (player.hasCharacterExload()) {
            // character ex load is applied, need to check if their default gear is different and if they have different character text texture

            if (CheckIfEXLoadCanBeApplied(player, characterInfo)) {
                if (player.extremeGear == ExtremeGear::Default) {
                    if (static_cast<s16>(characterInfo.defaultGearPortraitTextureID) != -1){
						object->textureIDs[0].textureSlot1 = characterInfo.defaultGearPortraitTextureID;
					}

                    if (static_cast<s16>(characterInfo.gearTextTextureID) != -1){
						object->textureIDs[0].textureSlot5 = characterInfo.gearTextTextureID;
					}
                }

                if (static_cast<s16>(characterInfo.characterTextTextureID) != -1){
					object->textureIDs[0].textureSlot2 = characterInfo.characterTextTextureID;
				}
            }
        }
        if (player.hasGearExload()) {
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
        if (player.hasCharacterExload()) {
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
		if (player.hasGearExload()) {
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
 * Takes care of binding Hatsune Miku's leek and microphone she holds to her right hand whenever she performs her attack.
 * Only used on the ::HatsuneMiku EX load.
 *
 * @param player The Player to apply binding to.
 * @param savePtr Where to save the attack part's matrix multiplication result.
 * @param level Which level attack is currently being performed.
 */
ASMUsed void HatsuneMiku_BindAttackPartsToBone(Player &player, Matrix3x3F &savePtr, const u8 level) {
	if(player.characterExload().exLoadID != EXLoad::HatsuneMiku) { return; }

	Matrix3x3F *mtxList = gpsaMtxList_Player[player.index];
	constexpr u32 rightHandBone = 43;

	if (level == 2) {
		// level 3 leek
		nnMultiplyMatrix(&player.unkC4, mtxList + rightHandBone, savePtr);
	} else {
		// level 1 microphone
		Matrix3x3F translateMat;
		nnMakeTranslateMatrix(&translateMat, 0.1f, 0.0f, -0.05f);

		Matrix3x3F modelMatrix;
		nnMultiplyMatrix(&player.unkC4, mtxList + rightHandBone, modelMatrix);
		nnMultiplyMatrix(&modelMatrix, &translateMat, savePtr);
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
ASMUsed const char *GetEXLoadFilenameSuperForm(const Player &player, const char *filename) {
	if(player.extremeGear != ExtremeGear::ChaosEmerald) {
		return filename;
	}

	switch(player.gearExload().exLoadID) {
		case EXLoad::PerfectNazo:
			filename = "PN18";
			break;
		case EXLoad::HyperSonic:
			filename = "PH18";
			break;
		case EXLoad::DarkSonic:
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
[[nodiscard]] std::optional<std::string> GetEXLoadCharacterModel(const Player &player, const GearType gearType) {
	if (!player.isRealPlayer() && isInGame()) {return std::nullopt;}
	switch(player.characterExload().exLoadID) {
		case EXLoad::HatsuneMiku:
			return "EC0";
		case EXLoad::MetalSonicNoScarf:
            if (gearType == GearType::Bike) {
                return "EC1B";
            }
			return "EC1";
		case EXLoad::Reala:
			return "EC2";
		case EXLoad::GonGon:
			return "EC3";
        case EXLoad::NeoMetal:
            return "EC4";
        case EXLoad::Selkadoom:
            if (gearType == GearType::Bike) {
                return "EC5B";
            }
            return "EC5";
		case EXLoad::Jackle:
            return "EC6";
		default:
			return std::nullopt;
	}
}

/**
 * Gets the first two starting letters of an EX load's skate models' filename, if applicable.
 * The model ID of the skates then get appended to the end of this filename.
 * For example, if ::Reala is picked alongside ::Darkness, the final filename would become "S270".
 *
 * @param player The Player to get the result from.
 * @return nullptr if the EX load doesn't have a model assigned to it, otherwise the start of the model's filename.
 */
[[nodiscard]] std::optional<std::string> GetEXLoadSkateModelStart(const Player &player) {
	switch(player.characterExload().exLoadID) {
		case EXLoad::HatsuneMiku:
			return "S0";
        case EXLoad::MetalSonicNoScarf:
            return "S1";
		case EXLoad::Reala:
			return "S2";
		case EXLoad::GonGon:
			return "S3";
        case EXLoad::Selkadoom:
            return "S5";
		case EXLoad::Jackle:
			return "S6";
		default:
			return std::nullopt;
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

	const std::span exLoadDataSlots = type == EXLoadHoverTypes::OnlyCharacter ? characterEXLoadDataSlots : gearEXLoadDataSlots;

	for(const auto &slot: exLoadDataSlots) {
		if(slot.exLoadID == EXLoad::None) { continue; }

		if(CheckIfEXLoadCanBeApplied(player, slot)) { return true; }
	}
	return false;
}

/**
 * Checks if player is currently using the specified character EX load.
 *
 * @param player The player to check.
 * @param exLoad The EX load ID to check for.
 * @return true if player is using the EX load ID, otherwise false
 */
ASMUsed [[nodiscard]] bool CheckPlayerHasCharacterEXLoadID(const Player &player, const EXLoad exLoad) {
	return player.characterExload().id() == std::to_underlying(exLoad);
}

/**
 * Checks if player is currently using the specified gear EX load.
 *
 * @param player The player to check.
 * @param exLoad The EX load ID to check for.
 * @return true if player is using the EX load ID, otherwise false
 */
ASMUsed [[nodiscard]] bool CheckPlayerHasGearEXLoadID(const Player &player, const EXLoad exLoad) {
	return player.gearExload().id() == std::to_underlying(exLoad);
}

/// Used to clear #Player_EXLoadData when necessary.
ASMUsed void ClearPlayerEXLoadBSS() {
	//memset(Player_EXLoadData.data(), 0, sizeof(Player_EXLoadData));
	//std::fill(Player_EXLoadData.begin(), Player_EXLoadData.end(), CurrentEXLoadInfo());
	std::ranges::fill(Player_EXLoadData, CurrentEXLoadInfo{});
}

/*
 * Called when the START button is pressed and the game is started from the CSS.
 * Takes care of very special EX loads, that don't need an EX load ID set to them (like Super Sonic EX load).
 */
ASMUsed void ClearSpecificEXLoads() {
	for(auto &player : players) {
		const auto &charExLoadInfo = player.characterExload();
		if(charExLoadInfo.exLoadID == EXLoad::SuperSonic) {
			player.exload().characterExload = {};
		}
	}
}

ASMUsed constexpr std::array<RGBA32, EXLoadCount> EXLoadHUDColors = {
		RGBA32(0x0), // dummy data
		RGBA32(0x471394ff), // perfect nazo
		RGBA32(0xcf4731ff), // e10r
		RGBA32(0x101010FF), // dark sonic
		RGBA32(0xC0C0C0FF), // hyper sonic
		RGBA32(0x0), // Stardust Speeder II
		RGBA32(0x0), // Hyper Hang-On
		RGBA32(0x0), // The Professional
		RGBA32(0x0), // Ollie King Gear
		RGBA32(0xD2C864FF), // Windmaster Jet
		RGBA32(0x1239B8FF), // neo metal sonic
		RGBA32(0x0), // E99
		RGBA32(0x0), // The Beast
		RGBA32(0x0), // e10b, UNUSED - it uses original colors
		RGBA32(0x00ffe5ff), // hatsune miku
		RGBA32(0x0), // metal sonic scarf
		RGBA32(0x4a0015ff), // reala
		RGBA32(0x210d00ff), // gongon
		RGBA32(0x0), // dummy data
		RGBA32(0x0), // dummy data
		RGBA32(0x0), // dummy data
		RGBA32(0x0), // dummy data
		RGBA32(0x0), // dummy data
		RGBA32(0x0), // dummy data
		RGBA32(0x0), // dummy data
		RGBA32(0x0), // dummy data
		RGBA32(0x0), // dummy data
		RGBA32(0x01056bFF), // seelkadoom
		RGBA32(0x0), // super sonic ex load
		RGBA32(0x0), // throttle
		RGBA32(0xFF5D00FF), // jackle
		RGBA32(0xFFDC0FFF), // E10Y
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

				// Throttle
                {
                        RGBANormalized(true),
                        RGBANormalized(true)
                },

				// Jackle
				{
                        RGBANormalized(255, 93, 0),
                        RGBANormalized(0, 10, 255)
                },

				// E10Y
				{
                        RGBANormalized(255, 220, 15),
                        RGBANormalized(26, 18, 255)
                },
		}
};