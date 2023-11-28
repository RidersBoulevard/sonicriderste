#include "separatemodelloading.hpp"
#include "cosmetics/player/exloads.hpp"
#include "cosmetics/player/skinsystem.hpp"
#include "lib/stdlib.hpp"
#include "packman_handlers.hpp"
#include "riders/gamemode.hpp"

#include <cstdio>
#include <cstring>

ASMDefined u32 nnHookModelTextures(void *);
ASMDefined void nnDrawObjectExt(void *, void *, void *, u32, u32);
ASMDefined void nnDrawObjectLtd(void *, void *, void *, u32, u32);
ASMDefined void func_8004EBCC(void *, void *, void *);
ASMDefined void lbl_8004F4BC(u32, void *);// the u32 is actually a pointer but i can't use it as void*
ASMDefined void *bss_BoardOnlyModelData[];
ASMDefined void *bss_BoardOnlyTextures[];
ASMDefined void *lbl_1000DF64[];
ASMDefined u32 lbl_0014CD08(char[], u32, u32, u32, void *, void *, u32, u32, u32);
ASMDefined void nnScaleModelBoneCustom(Matrix3x3F *, u32, f32 *);

std::array<CSSModel, MaxPlayerCount> cssModel;
std::array<CSSModel, MaxPlayerCount> cssSkinArchive;
std::array<CSSModel, MaxPlayerCount> cssEggmeisterTextures;

std::bitset<MaxPlayerCount> IsSeparateBoardModelActive;

/**
 * Gets the board model's file name for a specific player.
 *
 * @param player The player whose board model file name to get.
 * @return The file name of the board model.
 */
[[nodiscard]] std::array<char, 8> GetBoardFilename(Player *player) {
    const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
    std::array<char, 8> filename;// NOLINT(readability-magic-numbers,cppcoreguidelines-pro-type-member-init)

    if(player->extremeGear == ExtremeGear::Default) {
        // for default boards

        const Character &character = Characters[player->character];

        // default case
        sprintf(filename.data(), "P%c00", character.model);

        if (player->isRealPlayer()) {
            switch (exLoads.characterExLoadID) {
                case E10REXLoad:
                    sprintf(filename.data(), "P%c000", character.model);
                    break;
                case RealaEXLoad:
                    sprintf(filename.data(), "EB0");
                    break;
                case HatsuneMikuEXLoad:
                    sprintf(filename.data(), "EB1");
                    break;
                default:
                    break;
            }
        }
    } else {
        // for anything else

        const Gear &gear = Gears[player->extremeGear];

        // default case
        sprintf(filename.data(), "PB%d", gear.model);

        switch (player->character) {
            case Storm:
                if (player->getGearTypeIndependent() != Bike) { break; }
                sprintf(filename.data(), "PB%dB", gear.model);
                break;

                // SYB: This is custom. GEAR REPLICA
            case Ulala ... E10R:
            case SuperSonic:
                switch (player->extremeGear) {
                    using namespace ExtremeGear;
                    case HeavyBike:
                        sprintf(filename.data(), "PB20B");
                        break;
                    case Darkness:
                        sprintf(filename.data(), "PD00");
                        break;
                    case ERider ... AirTank:
                    case Destroyer ... SuperHangOn:
                    case Grinder ... Cannonball:
                        // every bike and skate besides heavy bike and darkness
                        sprintf(filename.data(), "RB%d", gear.model);
                        break;
                    default:
                        break;
                }
                break;
            case Eggman:
                switch (player->extremeGear) {
                    using namespace ExtremeGear;
                    case ERider ... SuperHangOn:
                        break;
                    default:
                        // anything but the bikes use eggman's replica gear, egg meister
                        sprintf(filename.data(), "PE00");
                        break;
                }
                break;
            default:
                break;
        }
    }

    // gear ex loads
    switch (exLoads.gearExLoadID) {
        case StardustSpeederEXLoad:
            if (player->character != Eggman) {
                sprintf(filename.data(), "EB2");
            }
            break;
        default:
            break;
    }

    return filename;
}

/**
 * Checks if the player needs a separate board model.
 *
 * @param player The player to check.
 * @param gearType The type of the currently selected Extreme Gear.
 * @return true if has a separate board model, otherwise false.
 */
[[nodiscard]] inline bool PlayerHasSeparateBoardModel(Player *player, GearType gearType) {
    return gearType != Skates && player->extremeGear != ExtremeGear::ChaosEmerald;
}

/**
 * Checks if the player needs Eggmeister's textures applied to their board model.
 *
 * @param player The player to check.
 * @return true if separate Eggmeister textures are needed, otherwise false
 */
[[nodiscard]] bool PlayerIsEggmeister(Player *player) {
    return player->character == Eggman &&
    (player->extremeGear < ExtremeGear::ERider || player->extremeGear > ExtremeGear::SuperHangOn);
}

/**
 * Gets the ID it uses to load in the specific texture from all the Eggmeister texture archives.
 *
 * @param player The player to get the ID from.
 * @return Eggmeister specific ID
 */
[[nodiscard]] u32 GetEggmeisterGearID(const Player *player) {
    if (player->extremeGear >= ExtremeGear::ERider && player->extremeGear <= ExtremeGear::SuperHangOn) {
        throw std::logic_error("Bike found in GetEggmeisterGearID!");
    }

    u32 gearID = player->extremeGear;
    if (gearID > ExtremeGear::ERider) {
        gearID -= ExtremeGear::BIKE_COUNT; // subtract all bikes
    }

    return gearID;
}

/**
 * Gets the index of the texture archive file where the Player's current gear's Eggmeister texture is located in.
 *
 * @param player The Player to get the index from.
 * @return Index of the correct Eggmeister texture archive file
 */
[[nodiscard]] u32 GetEggmeisterTextureArchiveIndex(const Player *player) {
    auto gearID = GetEggmeisterGearID(player);
    return gearID / Eggmeister::MAX_TEXTURE_COUNT_PER_ARCHIVE;
}

/**
 * Gets the index of the gear's texture file in the current Eggmeister texture archive.
 *
 * @param player The Player to get the index from.
 * @return Index of the gear's texture file in the texture archive
 */
[[nodiscard]] u32 GetEggmeisterTextureIndexInArchive(const Player *player) {
    auto gearID = GetEggmeisterGearID(player);
    return gearID % Eggmeister::MAX_TEXTURE_COUNT_PER_ARCHIVE;
}

/**
 * Gets the filename for the required Eggmeister texture archive file.
 *
 * @param player The Player who's using Eggmeister.
 * @return The correct Eggmeister texture archive filename.
 */
[[nodiscard]] std::array<char, 8> GetEggmeisterTextureArchiveFilename(const Player *player) {
    auto exLoads = FetchEnabledEXLoadIDs(*player);
    if (exLoads.isExloadEnabled() && exLoads.gearExLoadID == StardustSpeederEXLoad) {
        return std::array<char, 8>{"PETX"};
    }

    std::array<char, 8> filename;
    sprintf(filename.data(), "PET%d", GetEggmeisterTextureArchiveIndex(player) + 1);
    return filename;
}

/**
 * Applies specific exceptions and overrides to the current gear type.
 * Mainly used for gear replicas.
 *
 * @param player The player to apply exceptions to.
 * @param gearType The current gear type.
 * @return A new gear type.
 */
[[nodiscard]] inline GearType GetGearTypeCSSExceptions(Player *player, GearType gearType) {
    switch(player->character) {
        case Ulala:
            if (gearType == Bike) {
                gearType = Board;
            }
            return gearType;
        case E10G:
        case E10R:
        case SuperSonic:
            return Board;
        case Eggman:
            return EggmanType;
        default:
            return gearType;
    }
}

/**
 * Dumps a board model for a given player if necessary, and sets it up to be rendered in-game.
 *
 * @param player The player to dump a board model for.
 * @param index The player's index.
 */
ASMUsed void DumpBoardModel(Player *player, const u32 index) {
    const bool hasSeparateBoardModel = PlayerHasSeparateBoardModel(player, player->gearType);
	IsSeparateBoardModelActive[player->index] = hasSeparateBoardModel;

    if (hasSeparateBoardModel) {
        const auto filename = GetBoardFilename(player);
        const auto *file = DumpPackManFile(filename.data(), 0);
        SetArchiveBinary(file, index, 0);
    }

	if(player->character == TotalCharacterAmount) {
		// for variable character, force it to shadow
		player->character = Shadow;
	}
}

/**
 * Dumps a board model on the CSS for a given player if necessary, and sets it up to be rendered in the CSS.
 *
 * @param player The player to dump a board model for.
 * @param index The player's index.
 */
ASMUsed void DumpBoardModelCSS(Player *player, const u32 index) {
	const auto gearType = GetGearTypeCSSExceptions(player, player->getGearTypeIndependent());
    const bool hasSeparateBoardModel = PlayerHasSeparateBoardModel(player, gearType);
    IsSeparateBoardModelActive[index] = hasSeparateBoardModel;
	static m2darray<char, MaxControllerCount, 8> CSS_BoardFilename;// Used to be ASMDefined, Should this be static?

    cssModel[index].file = nullptr;

    if (!hasSeparateBoardModel) { return; }

    auto filename = GetBoardFilename(player);
    std::copy(filename.begin(), filename.end(), CSS_BoardFilename[index].data());

    CSSModel &modelData = cssModel[index];
    modelData.count = lbl_0014CD08(CSS_BoardFilename[index].data(), 1, 1, 0, &modelData.file, &modelData.status, 1, 1, index);
}

constexpr std::array<unsigned char, 4> GearTypeChars = {
		'O', 'S', 'B', 'B'
};

/**
 * Dumps a character model for a given player, and sets it up to be rendered in-game.
 *
 * @param player The player to dump a character model for.
 * @param index The player's index.
 * @param originalFilename The file that needed to be dumped per vanilla code. This is used because certain character models still follow old naming conventions.
 */
ASMUsed void DumpCharacterModel(Player *player, const u32 index, char originalFilename[]) {
    const bool hasSeparateBoardModel = PlayerHasSeparateBoardModel(player, player->gearType);
	char *file;

	if(player->isRealPlayer() && PlayerSkinSystemData[player->input->port].skinID != 0) {
		char filename[8];
		char *skinArchive;
		sprintf(filename, "CSP%c", Characters[player->character].model);
		skinArchive = DumpPackManFile(filename, 0);
		SetArchiveBinary(skinArchive, index, 0);
	}

	if(hasSeparateBoardModel) {
		// not skates

		std::array<char, 8> filename;
		const char *exLoadFileName = GetEXLoadCharacterModel(player, player->gearType);

		if(exLoadFileName != nullptr) {
			sprintf(filename.data(), "%s", exLoadFileName);
		} else {
			sprintf(filename.data(), "P%cC%c", Characters[player->character].model, GearTypeChars[player->gearType]);
		}

		file = DumpPackManFile(filename.data(), 0);
	} else if(player->gearType == Skates) {
		std::array<char, 8> filename;
		const char *skateModel = GetEXLoadSkateModelStart(player);

		if(skateModel != nullptr) {
			const Gear &gear = Gears[player->extremeGear];
			sprintf(filename.data(), "%s%d", skateModel, gear.model);

			file = DumpPackManFile(filename.data(), 0);
		} else {
			file = DumpPackManFile(originalFilename, 0);
		}
	} else {
		file = DumpPackManFile(originalFilename, 0);
	}

	SetArchiveBinary(file, index, 0);

    if (PlayerIsEggmeister(player)) {
        SetArchiveBinary(DumpPackManFile(GetEggmeisterTextureArchiveFilename(player).data(), 0), index, 0);
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

/**
 * Dumps a character model for a given player in the CSS, and sets it up to be rendered in the CSS.
 *
 * @param player The player to dump a character model for.
 * @param filename The file that needed to be dumped per vanilla code. This is used because certain character models still follow old naming conventions.
 */
ASMUsed void SetupCharacterModelCSS(Player *player, char filename[]) {
	static char CSS_SkinArchiveFilename[4][8];
    static char CSS_EggmeisterTextures[4][8];

    const auto gearType = GetGearTypeCSSExceptions(player, player->getGearTypeIndependent());
    const bool hasSeparateBoardModel = PlayerHasSeparateBoardModel(player, gearType);

    const auto &index = player->input->port;
    cssSkinArchive[index].file = nullptr;
    cssEggmeisterTextures[index].file = nullptr;

	if(hasSeparateBoardModel) {
		// not skates

		const char *exLoadFileName = GetEXLoadCharacterModel(player, gearType);
		if(exLoadFileName != nullptr) {
			sprintf(filename, "%s", exLoadFileName);
		} else {
			sprintf(filename, "P%cC%c", Characters[player->character].model, GearTypeChars[gearType]);
		}
	} else if(gearType == Skates) {
		const char *skateModel = GetEXLoadSkateModelStart(player);

		if(skateModel != nullptr) {
			const Gear &gear = Gears[player->extremeGear];
			sprintf(filename, "%s%d", skateModel, gear.model);
		}
	}

	if(PlayerSkinSystemData[player->input->port].skinID != 0) {
		CSSModel &skinData = cssSkinArchive[index];
		sprintf(CSS_SkinArchiveFilename[index], "CSP%c", Characters[player->character].model);
		skinData.count = lbl_0014CD08(CSS_SkinArchiveFilename[index], 1, 1, 0, &skinData.file, &skinData.status, 1, 1, index);
	}

    if (PlayerIsEggmeister(player)) {
        CSSModel &eggData = cssEggmeisterTextures[index];
        auto eggmeisterTextureFilename = GetEggmeisterTextureArchiveFilename(player);
        std::memcpy(CSS_EggmeisterTextures[index], eggmeisterTextureFilename.data(), eggmeisterTextureFilename.size());
        eggData.count = lbl_0014CD08(CSS_EggmeisterTextures[index], 1, 1, 0, &eggData.file, &eggData.status, 1, 1, index);
    }
}
#pragma GCC diagnostic pop

/**
 * This function is ran after restarting the race whilst in-game.
 * Since all of the high heap gets freed upon restart (and on other occasions too),
 * this function needs to be ran to ensure we still have the required textures in memory for all Eggmeister users.
 *
 * @param forceSkip Forces skipping the loading process. During race restarts, this function gets run twice (thank you SEGA), this is just to optimize.
 */
ASMUsed void ReloadEggmeisterTextures(const bool forceSkip) {
    // check for restart
    if (RaceExitMethod != 2 || forceSkip) { return; }
    
    for (auto &player : getCurrentPlayerList()) {
        // just dump the texture file again if necessary, packman handler will take care of the rest
        if (PlayerIsEggmeister(&player)) {
            std::array<char, 8> filename;
            sprintf(filename.data(), "PET%d", GetEggmeisterTextureArchiveIndex(&player) + 1);
            SetArchiveBinary(DumpPackManFile(filename.data(), 0), player.index, 0);
        }
    }
}

/**
 * Frees all Eggmeister textures and other regions.
 *
 * @param index The player index, whose assets need to be freed.
 */
void FreeEggmeisterHeap(const u32 index) {
    for (u32 i = 0; i < 3; i++) {
        if (EggmeisterHeap[index][i] != nullptr) {
            free_Hi(EggmeisterHeap[index][i]);
            EggmeisterHeap[index][i] = nullptr;
        }
    }
}

/**
 * Clears all Eggmeister heap pointers.
 */
ASMUsed void ClearEggmeisterHeapAll() {
    TRK_memset(EggmeisterHeap.data(), 0, sizeof(EggmeisterHeap));
}

/**
 * Frees all used assets/files on the CSS when backing out of a character selection.
 *
 * @param index The player index, whose assets need to be freed.
 */
ASMUsed void FreeCSSAssets(const u32 index) {
    const CSSModel &eggData = cssEggmeisterTextures[index];
    const CSSModel &skinData = cssSkinArchive[index];
    const CSSModel &modelData = cssModel[index];

    if (eggData.file != nullptr) {
        // the eggmeister texture archive itself is already freed, just need to free the other files
        FreeEggmeisterHeap(index);
    }

    if (skinData.file != nullptr) {
        CSS_FreeHigh(skinData.count);
    }

    if (modelData.file != nullptr) {
        CSS_FreeHigh(modelData.count);
    }
}

/**
 * Renders a player's dumped board model.
 *
 * @param player The player whose dumped board model to render.
 * @param objectDataInfo Matrix list for every bone. This should be derived from the player's matrix list, to ensure the board is animated properly.
 * @param boneVisibilityStatus The animated NodeHide keyframe data.
 */
void RenderBoardMesh(Player *player, void *objectDataInfo, void *boneVisibilityStatus) {
	const u32 mirrorFlag = static_cast<const u32>(((player->unkBAC & 0x100) ? 1 : 0) * 0x40);// NOLINT(readability-implicit-bool-conversion)
	nnHookModelTextures(bss_BoardOnlyTextures[player->index]);

	if(static_cast<bool>(mirrorFlag)) {
		nnDrawObjectExt(bss_BoardOnlyModelData[player->index], objectDataInfo, boneVisibilityStatus, 0x80000000,
						mirrorFlag);
	} else {
		nnDrawObjectLtd(bss_BoardOnlyModelData[player->index], objectDataInfo, boneVisibilityStatus,
						0x80000000, mirrorFlag);
	}
}

/**
 * Renders a ghost player's board model in time trial.
 *
 * @param player The ghost player.
 * @param objectDataInfo Matrix list for every bone. This should be derived from the player's matrix list, to ensure the board is animated properly.
 * @param boneVisibilityStatus The animated NodeHide keyframe data.
 */
void RenderBoardMeshTimeTrial(Player *player, void *objectDataInfo, void *boneVisibilityStatus) {
	nnHookModelTextures(bss_BoardOnlyTextures[player->index]);

	nnDrawObjectExt(bss_BoardOnlyModelData[player->index], objectDataInfo, boneVisibilityStatus, 0x80000000,
					0x02800400);
}

ASMUsed void
RenderBoardModelTimeTrial(Player *player, void *objectDataInfo, void *boneVisibilityStatus, void *weightVertPtr,
						  void *otherBoneData) {
	if(IsSeparateBoardModelActive[player->index]) {
		func_8004EBCC(weightVertPtr, bss_BoardOnlyModelData[player->index], lbl_1000DF64[player->index]);
		lbl_8004F4BC(*static_cast<u32 *>(weightVertPtr), otherBoneData);

		RenderBoardMeshTimeTrial(player, objectDataInfo, boneVisibilityStatus);
	}
}

ASMUsed void RenderBoardModel(Player *player, void *objectDataInfo, void *boneVisibilityStatus, u32 *weightVertPtr,
							  void *otherBoneData) {
	if(IsSeparateBoardModelActive[player->index]) {
		func_8004EBCC(weightVertPtr, bss_BoardOnlyModelData[player->index], lbl_1000DF64[player->index]);
		lbl_8004F4BC(*weightVertPtr, otherBoneData);

		RenderBoardMesh(player, objectDataInfo, boneVisibilityStatus);
	}
}

ASMUsed void
RenderBoardModelMultiplayer(Player *player, void *objectDataInfo, void *boneVisibilityStatus, u32 *weightVertPtr,
							void *otherBoneData, u32 *someData) {
	if(IsSeparateBoardModelActive[player->index]) {
		func_8004EBCC(weightVertPtr, bss_BoardOnlyModelData[player->index], lbl_1000DF64[player->index]);
		*someData ^= 2;// fixes vertex weights on other players' screen
		lbl_8004F4BC(*weightVertPtr, otherBoneData);

		RenderBoardMesh(player, objectDataInfo, boneVisibilityStatus);
	}
}

ASMUsed void ScaleBoardModel(Player *player, Matrix3x3F *boneMatrices) {
	// scales certain characters boards appropriately to their model size
	// index 2 is the extreme gear bone in the matrices
	//u32 gearType;
	//if(player->extremeGear < ERider){
	//	gearType = 0;
	//}else if(player->extremeGear < Darkness){
	//	gearType = 2;
	//}else{
	//	gearType = 1;
	//}

	if(player->character == Storm && player->gearType == Bike) {
		f32 scale = 1.25F;// NOLINT(readability-magic-numbers)
		// scale in all axis
		nnScaleModelBoneCustom(boneMatrices + 2, 0, &scale);
		nnScaleModelBoneCustom(boneMatrices + 2, 1, &scale);
		nnScaleModelBoneCustom(boneMatrices + 2, 2, &scale);
	}
}
