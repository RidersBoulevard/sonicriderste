#include "separatemodelloading.hpp"
#include "cosmetics/player/exloads.hpp"
#include "cosmetics/player/skinsystem.hpp"
#include "lib/stdlib.hpp"
#include "packman_handlers.hpp"
#include "riders/gamemode.hpp"
#include "filehandler_dat.hpp"
#include "nn/ninjanext.hpp"
#include "lib/files.hpp"

#include <format>

ASMDefined void func_8004EBCC(void *, void *, void *);
ASMDefined void lbl_8004F4BC(u32, void *);// the u32 is actually a pointer but i can't use it as void*
ASMDefined NNS_Object *bss_BoardOnlyModelData[];
ASMDefined NNS_TexList *bss_BoardOnlyTextures[];
ASMDefined void *gpsaUnitMtxPal_Player[];
ASMDefined u32 lbl_0014CD08(const char*, u32, u32, u32, void *, void *, u32, u32, u32);
ASMDefined void nnScaleModelBoneCustom(Matrix3x4F *, u32, f32 *);

std::array<CSSModel, MaxPlayerCount> cssModel;
std::array<CSSModel, MaxPlayerCount> cssSkinArchive;
std::array<CSSModel, MaxPlayerCount> cssEggmeisterTextures;

std::bitset<MaxPlayerCount> IsSeparateBoardModelActive;

/**
 * Gets the board model's file name for a specific player.
 *
 * @param player The player whose board model file name to get.
 * @param isCSS true if CSS function calls this, false if otherwise (i.e. in-game or on load)
 * @return The file name of the board model.
 */
[[nodiscard]] std::string GetBoardFilename(const Player &player, const bool isCSS) {
    std::string filename;// NOLINT(readability-magic-numbers,cppcoreguidelines-pro-type-member-init)

    if(player.extremeGear == ExtremeGear::Default) { // for default boards
        const Character &character = Characters[player.character];

        // default case
		filename = std::format("P{}00", static_cast<char>(character.model));
        //sprintf(filename.data(), "P%c00", character.model);

        if (isCSS || player.isRealPlayer()) {
            switch (player.characterExload().exLoadID) {
                case EXLoad::E10R:
					filename = std::format("P{}000", static_cast<char>(character.model));
                    //sprintf(filename.data(), "P%c000", character.model);
                    break;
                case EXLoad::Reala:
                case EXLoad::Jackle: // SYB: Jackle uses Reala's Default
					filename = "EB0";
                    //sprintf(filename.data(), "EB0");
                    break;
                case EXLoad::HatsuneMiku:
					filename = "EB1";
                    //sprintf(filename.data(), "EB1");
                    break;
                case EXLoad::E10Y:
					filename = std::format("P{}001", static_cast<char>(character.model));
                    break;
                default:
                    break;
            }
        }
    } else {
        // for anything else

        const Gear &gear = Gears[player.extremeGear];

        // default case
		filename = std::format("PB{}", gear.model);
        //sprintf(filename.data(), "PB%d", gear.model);

        switch (player.character) {
            case Character::Storm:
                if (player.getGearTypeIndependent() != GearType::Bike) { break; }
				filename = std::format("PB{}B", gear.model);
                //sprintf(filename.data(), "PB%dB", gear.model);
                break;

                // SYB: This is custom. GEAR REPLICA
            case Character::Ulala ... Character::E10R:
            case Character::SuperSonic:
                switch (player.extremeGear) {
                    using namespace ExtremeGear;
                    case Darkness:
						filename = "PD00";
                        //sprintf(filename.data(), "PD00");
                        break;
                    case ERider ... SuperHangOn:
                    case Grinder ... Cannonball:
                        // every bike and skate besides darkness
						filename = std::format("RB{}", gear.model);
                        //sprintf(filename.data(), "RB%d", gear.model);
                        break;
                    default:
                        break;
                }
                break;
            case Character::Eggman:
                switch (player.extremeGear) {
                    using namespace ExtremeGear;
                    case ERider ... SuperHangOn:
                        break;
                    default:
                        // anything but the bikes use eggman's replica gear, egg meister
						filename = "PE00";
                        //sprintf(filename.data(), "PE00");
                        break;
                }
                break;
            default:
                break;
        }
    }

    // gear ex loads
    // switch (player.gearExload().exLoadID) {
    //     case EXLoad::StardustSpeeder:
    //         if (player.character != Character::Eggman) {
	// 			filename = "EB2";
    //             //sprintf(filename.data(), "EB2");
    //         }
    //         break;
    //     default:
    //         break;
    // }

    return filename;
}

/**
 * Checks if the player needs a separate board model.
 *
 * @param player The player to check.
 * @param gearType The type of the currently selected Extreme Gear.
 * @return true if has a separate board model, otherwise false.
 */
[[nodiscard]] inline bool PlayerHasSeparateBoardModel(const Player &player, GearType gearType) {
    return gearType != GearType::Skates && player.extremeGear != ExtremeGear::ChaosEmerald;
}

/**
 * Checks if the player needs Eggmeister's textures applied to their board model.
 *
 * @param player The player to check.
 * @return true if separate Eggmeister textures are needed, otherwise false
 */
[[nodiscard]] bool PlayerIsEggmeister(const Player &player) {
    return player.character == Character::Eggman &&
    (player.extremeGear < ExtremeGear::ERider || player.extremeGear > ExtremeGear::SuperHangOn);
}

/**
 * Gets the ID it uses to load in the specific texture from all the Eggmeister texture archives.
 *
 * @param player The player to get the ID from.
 * @return Eggmeister specific ID
 */
[[nodiscard]] u32 GetEggmeisterGearID(const Player &player) {
    if (player.extremeGear >= ExtremeGear::ERider && player.extremeGear <= ExtremeGear::SuperHangOn) {
        throw std::logic_error("Bike found in GetEggmeisterGearID!");
    }

    u32 gearID = player.extremeGear;
    if (gearID > ExtremeGear::ERider) {
        gearID -= ExtremeGear::BIKE_COUNT; // subtract all bikes
    }
    if (gearID > 30) gearID = 30; // REPLACE WHEN NEW GEARS HAVE PROPER EGGMEISTER SUPPORT
    return gearID;
}

/**
 * Gets the index of the texture archive file where the Player's current gear's Eggmeister texture is located in.
 *
 * @param player The Player to get the index from.
 * @return Index of the correct Eggmeister texture archive file
 */
[[nodiscard]] u32 GetEggmeisterTextureArchiveIndex(const Player &player) {
    auto gearID = GetEggmeisterGearID(player);
    return gearID / Eggmeister::MAX_TEXTURE_COUNT_PER_ARCHIVE;
}

/**
 * Gets the index of the gear's texture file in the current Eggmeister texture archive.
 *
 * @param player The Player to get the index from.
 * @return Index of the gear's texture file in the texture archive
 */
[[nodiscard]] u32 GetEggmeisterTextureIndexInArchive(const Player &player) {
    auto gearID = GetEggmeisterGearID(player);
    return gearID % Eggmeister::MAX_TEXTURE_COUNT_PER_ARCHIVE;
}

/**
 * Gets the filename for the required Eggmeister texture archive file.
 *
 * @param player The Player who's using Eggmeister.
 * @return The correct Eggmeister texture archive filename.
 */
[[nodiscard]] std::string GetEggmeisterTextureArchiveFilename(const Player &player) {
    if (player.extremeGear >= ExtremeGear::GunGear) {
        return "PETX";
    }

    //std::string filename;
    //sprintf(filename.data(), "PET%d", GetEggmeisterTextureArchiveIndex(player) + 1);
    return std::format("PET{}", GetEggmeisterTextureArchiveIndex(player) + 1);
}

/**
 * Applies specific exceptions and overrides to the current gear type.
 * Mainly used for gear replicas.
 *
 * @param player The player to apply exceptions to.
 * @param gearType The current gear type.
 * @return A new gear type.
 */
[[nodiscard]] inline GearType GetGearTypeCSSExceptions(const Player &player, GearType gearType) {
    switch(player.extremeGear) {
        case ExtremeGear::GunGear:
        case ExtremeGear::OllieKingGear:
            gearType = GearType::Board;
            break;
        default:
            break;
    }

    switch(player.character) {
        case Character::Ulala:
            if (gearType == GearType::Bike) {
                gearType = GearType::Board;
            }
            return gearType;
        case Character::E10G:
        case Character::E10R:
        case Character::SuperSonic:
            return GearType::Board;
        case Character::Eggman:
            return GearType::EggmanType;
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
ASMUsed void DumpBoardModel(Player &player, const u32 index) {
    const bool hasSeparateBoardModel = PlayerHasSeparateBoardModel(player, player.gearType);
	IsSeparateBoardModelActive[player.index] = hasSeparateBoardModel;

    if (hasSeparateBoardModel) {
        const auto filename = GetBoardFilename(player, false);
        const auto *file = DumpPackManFile(filename.c_str());
        SetArchiveBinary(file, index, 0);
    }

	if(player.character == Character::Total) {
		// for variable character, force it to shadow
		player.character = Character::Shadow;
	}
}

/**
 * Dumps a board model on the CSS for a given player if necessary, and sets it up to be rendered in the CSS.
 *
 * @param player The player to dump a board model for.
 * @param index The player's index.
 */
ASMUsed void DumpBoardModelCSS(const Player &player, const u32 index) {
	const auto gearType = GetGearTypeCSSExceptions(player, player.getGearTypeIndependent());
    const bool hasSeparateBoardModel = PlayerHasSeparateBoardModel(player, gearType);
    IsSeparateBoardModelActive[index] = hasSeparateBoardModel;

	CSSModel &modelData = cssModel[index];
	modelData.file = nullptr;

    if (!hasSeparateBoardModel) { return; }
	static std::array<std::string, MaxControllerCount> BoardModelFilenames;// Used to be ASMDefined, Should this be static?

	BoardModelFilenames[index] = GetBoardFilename(player, true);
    //auto filename = GetBoardFilename(player);
    //std::copy(filename.begin(), filename.end(), BoardModelFilenames[index].data());

    modelData.count = lbl_0014CD08(BoardModelFilenames[index].c_str(), 1, 1, 0, &modelData.file, &modelData.status, 1, 1, index);
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
ASMUsed void DumpCharacterModel(const Player &player, const u32 index, char originalFilename[]) {
    const bool hasSeparateBoardModel = PlayerHasSeparateBoardModel(player, player.gearType);
	std::string filename;

	if(player.isRealPlayer() && PlayerSkinSystemData[player.input->port].skinID != 0) {
		//std::array<char, 8> filename;
		filename = std::format("CSP{}", static_cast<char>(Characters[player.character].model));
		//sprintf(filename, "CSP%c", Characters[player.character].model);
		char *skinArchive = DumpPackManFile(filename.c_str());
		SetArchiveBinary(skinArchive, index, 0);
	}

	char *file = nullptr;
	if(hasSeparateBoardModel) { // not skates
		//std::array<char, 8> filename;
		auto exLoadFileName = GetEXLoadCharacterModel(player, player.gearType);

		if(exLoadFileName) {
			filename = *exLoadFileName;
			//sprintf(filename.data(), "%s", exLoadFileName);
		} else {
			filename = std::format("P{}C{}", static_cast<char>(Characters[player.character].model), static_cast<char>(GearTypeChars[std::to_underlying(player.gearType)]));
			//sprintf(filename.data(), "P%cC%c", Characters[player.character].model, GearTypeChars[player.gearType]);
		}

		file = DumpPackManFile(filename.c_str());
	} else if(player.gearType == GearType::Skates) {
		//std::array<char, 8> filename;
		auto skateModel = GetEXLoadSkateModelStart(player);

		if(skateModel) {
			const Gear &gear = Gears[player.extremeGear];
			filename = std::format("{}{}", *skateModel, gear.model);
			//sprintf(filename.data(), "%s%d", skateModel, gear.model);

			file = DumpPackManFile(filename.data());
		} else {
			file = DumpPackManFile(originalFilename);
		}
	} else {
		file = DumpPackManFile(originalFilename);
	}

	SetArchiveBinary(file, index, 0);

    if (PlayerIsEggmeister(player)) {
        SetArchiveBinary(DumpPackManFile(GetEggmeisterTextureArchiveFilename(player).c_str(), 0), index, 0);
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

constexpr auto bufferSize = 5;
/**
 * Dumps a character model for a given player in the CSS, and sets it up to be rendered in the CSS.
 *
 * @param player The player to dump a character model for.
 * @param filename The file that needed to be dumped per vanilla code. This is used because certain character models still follow old naming conventions.
 */
ASMUsed void SetupCharacterModelCSS(const Player &player, char out[bufferSize]) {
    const auto &index = player.input->port;
    cssSkinArchive[index].file = nullptr;
    cssEggmeisterTextures[index].file = nullptr;

	const auto gearType = GetGearTypeCSSExceptions(player, player.getGearTypeIndependent());
	const bool hasSeparateBoardModel = PlayerHasSeparateBoardModel(player, gearType);
	std::string filename;
	if(hasSeparateBoardModel) { // not skates
		auto exLoadFileName = GetEXLoadCharacterModel(player, gearType);
		if(exLoadFileName) {
			filename = *exLoadFileName;
			//sprintf(filename, "%s", exLoadFileName);
		} else {
			filename = std::format("P{}C{}", static_cast<char>(Characters[player.character].model), static_cast<char>(GearTypeChars[std::to_underlying(gearType)]));
			//sprintf(filename, "P%cC%c", Characters[player.character].model, GearTypeChars[gearType]);
		}
	} else if(gearType == GearType::Skates) {
		auto skateModel = GetEXLoadSkateModelStart(player);

		if(skateModel) {
			const Gear &gear = Gears[player.extremeGear];
			filename = std::format("{}{}", *skateModel, gear.model);
			//sprintf(filename, "%s%d", skateModel, gear.model);
		}
	}

	if(!filename.empty()){
		auto bytesWritten = filename.copy(out, bufferSize);
		out[bytesWritten] = '\0'; // Ensure the string is null-terminated
	}


	if(PlayerSkinSystemData[player.input->port].skinID != 0) {
		static std::array<std::string, MaxControllerCount> CSS_SkinArchiveFilename;

		auto &SkinArchiveFilename = CSS_SkinArchiveFilename[index];
		CSSModel &skinData = cssSkinArchive[index];
		SkinArchiveFilename = std::format("CSP{}", static_cast<char>(Characters[player.character].model));
		//sprintf(CSS_SkinArchiveFilename[index], "CSP%c", Characters[player.character].model);
		skinData.count = lbl_0014CD08(SkinArchiveFilename.c_str(), 1, 1, 0, &skinData.file, &skinData.status, 1, 1, index);
	}

    if (PlayerIsEggmeister(player)) {
		static std::array<std::string, MaxControllerCount> CSS_EggmeisterTextures;

		auto &EggmeisterTexture = CSS_EggmeisterTextures[index];
        CSSModel &eggData = cssEggmeisterTextures[index];
        EggmeisterTexture = GetEggmeisterTextureArchiveFilename(player);

        //std::memcpy(EggmeisterTexture.c_str(), eggmeisterTextureFilename.data(), eggmeisterTextureFilename.size());
        eggData.count = lbl_0014CD08(EggmeisterTexture.c_str(), 1, 1, 0, &eggData.file, &eggData.status, 1, 1, index);
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
    if (gu32EndOfGameFlag != std::to_underlying(ExitMethod::Retry) || forceSkip) { return; }
    for (auto &player : getCurrentPlayerList()) {
        // just dump the texture file again if necessary, packman handler will take care of the rest
        if (PlayerIsEggmeister(player)) {
            //std::array<char, 8> filename;
            //sprintf(filename.data(), "PET%d", GetEggmeisterTextureArchiveIndex(player) + 1);
        	auto filename = GetEggmeisterTextureArchiveFilename(player);
            SetArchiveBinary(DumpPackManFile(filename.c_str()), player.index, 0);
        }
    }
}

/**
 * Frees all Eggmeister textures and other regions.
 *
 * @param index The player index, whose assets need to be freed.
 */
void FreeEggmeisterHeap(const u32 index) {
    for (auto &ptr : EggmeisterHeap[index]) {
        if (ptr != nullptr) {
            free_Hi(ptr);
            ptr = nullptr;
        }
    }
}

/**
 * Clears all Eggmeister heap pointers.
 */
ASMUsed void ClearEggmeisterHeapAll() {
    //memset(EggmeisterHeap.data(), 0, sizeof(EggmeisterHeap));
	using InnerArrayT = decltype(EggmeisterHeap[0]);
	std::ranges::for_each(EggmeisterHeap, [](InnerArrayT &arg){
		std::ranges::fill(arg, nullptr);
	});
}

/**
 * Frees all used assets/files on the CSS when backing out of a character selection.
 *
 * @param index The player index, whose assets need to be freed.
 */
ASMUsed void FreeCSSAssets(const u32 index) {
    const CSSModel &eggData = cssEggmeisterTextures[index];
    if (eggData.file != nullptr) {
        // the eggmeister texture archive itself is already freed, just need to free the other files
        FreeEggmeisterHeap(index);
    }

	const CSSModel &skinData = cssSkinArchive[index];
    if (skinData.file != nullptr) {
        CSS_FreeHigh(skinData.count);
    }

	const CSSModel &modelData = cssModel[index];
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
void RenderBoardMesh(const Player &player, NNS_Matrix *objectDataInfo, NNS_NodeStatus *boneVisibilityStatus) {
	const u32 mirrorFlag = static_cast<const u32>(((player.unkBAC & 0x100) ? 1 : 0) * 0x40);// NOLINT(readability-implicit-bool-conversion)
	nnSetTextureList(bss_BoardOnlyTextures[player.index]);

	if(static_cast<bool>(mirrorFlag)) {
		nnDrawObjectExt(bss_BoardOnlyModelData[player.index], objectDataInfo, boneVisibilityStatus, 0x80000000,
						mirrorFlag);
	} else {
		nnDrawObjectLtd(bss_BoardOnlyModelData[player.index], objectDataInfo, boneVisibilityStatus,
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
void RenderBoardMeshTimeTrial(const Player &player, NNS_Matrix *objectDataInfo, NNS_NodeStatus *boneVisibilityStatus) {
	nnSetTextureList(bss_BoardOnlyTextures[player.index]);

	nnDrawObjectExt(bss_BoardOnlyModelData[player.index], objectDataInfo, boneVisibilityStatus, 0x80000000,
					0x02800400);
}

ASMUsed void
RenderBoardModelTimeTrial(const Player &player, NNS_Matrix *objectDataInfo, NNS_NodeStatus *boneVisibilityStatus, void *weightVertPtr,
						  void *otherBoneData) {
	if(IsSeparateBoardModelActive[player.index]) {
		func_8004EBCC(weightVertPtr, bss_BoardOnlyModelData[player.index], gpsaUnitMtxPal_Player[player.index]);
		lbl_8004F4BC(*static_cast<u32 *>(weightVertPtr), otherBoneData);

		RenderBoardMeshTimeTrial(player, objectDataInfo, boneVisibilityStatus);
	}
}

ASMUsed void RenderBoardModel(const Player &player, NNS_Matrix *objectDataInfo, NNS_NodeStatus *boneVisibilityStatus, u32 *weightVertPtr,
							  void *otherBoneData) {
	if(IsSeparateBoardModelActive[player.index]) {
		func_8004EBCC(weightVertPtr, bss_BoardOnlyModelData[player.index], gpsaUnitMtxPal_Player[player.index]);
		lbl_8004F4BC(*weightVertPtr, otherBoneData);

		RenderBoardMesh(player, objectDataInfo, boneVisibilityStatus);
	}
}

ASMUsed void
RenderBoardModelMultiplayer(const Player &player, NNS_Matrix *objectDataInfo, NNS_NodeStatus *boneVisibilityStatus, u32 *weightVertPtr,
							void *otherBoneData, u32 *someData) {
	if(IsSeparateBoardModelActive[player.index]) {
		func_8004EBCC(weightVertPtr, bss_BoardOnlyModelData[player.index], gpsaUnitMtxPal_Player[player.index]);
		*someData ^= 2;// fixes vertex weights on other players' screen
		lbl_8004F4BC(*weightVertPtr, otherBoneData);

		RenderBoardMesh(player, objectDataInfo, boneVisibilityStatus);
	}
}

ASMUsed void ScaleBoardModel(const Player &player, Matrix3x4F *boneMatrices) {
	// scales certain characters boards appropriately to their model size
	// index 2 is the extreme gear bone in the matrices
	//u32 gearType;
	//if(player.extremeGear < ERider){
	//	gearType = 0;
	//}else if(player.extremeGear < Darkness){
	//	gearType = 2;
	//}else{
	//	gearType = 1;
	//}

	if(player.character == Character::Storm && player.gearType == GearType::Bike) {
		f32 scale = 1.25F;// NOLINT(readability-magic-numbers)
		// scale in all axis
		nnScaleModelBoneCustom(boneMatrices + 2, 0, &scale);
		nnScaleModelBoneCustom(boneMatrices + 2, 1, &scale);
		nnScaleModelBoneCustom(boneMatrices + 2, 2, &scale);
	}
}
