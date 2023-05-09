#include "context.hpp"
#include "cosmetics/player/exloads.hpp"

#include <bitset>
#include <cstdio>

global u32 nnHookModelTextures(void *);
global void nnDrawObjectExt(void *, void *, void *, u32, u32);
global void nnDrawObjectLtd(void *, void *, void *, u32, u32);
global void func_8004EBCC(void *, void *, void *);
global void lbl_8004F4BC(u32, void *); // the u32 is actually a pointer but i can't use it as void*
global void *bss_BoardOnlyModelData[];
global void *bss_BoardOnlyTextures[];
global void *lbl_1000DF64[];
global u32 lbl_0014CD08(char[], u32, u32, u32, void *, void *, u32, u32, u32);
global void nnScaleModelBoneCustom(struct Matrix3x3 *, u32, f32 *);
global u32 RaceExitMethod;

struct CSSModel{
	u32 count;
	void *file;
	u32 status;
	u32 padding;
};

std::array<CSSModel, MaxPlayerCount> cssModel;

std::bitset<MaxPlayerCount> IsSeparateBoardModelActive;

ASMUsed void ClearData_SeparateBoardModels(){
	if(RaceExitMethod != 2){ // on retry, don't clear data
		IsSeparateBoardModelActive.reset();
	}
}

ASMUsed void DumpBoardModel(Player *player, const u32 index){
	//u8 controllerPort = player->input->port;
	EnabledEXLoads exLoads{};
	FetchEnabledEXLoadIDs(player, exLoads);

	IsSeparateBoardModelActive[player->index] = false;
	std::array<char, 8> filename; // NOLINT(readability-magic-numbers,cppcoreguidelines-pro-type-member-init)
	if(player->extremeGear == DefaultGear){
		IsSeparateBoardModelActive[player->index] = true;
		const Character &character = Characters[player->character];
		if(exLoads.characterExLoadID == E10REXLoad && !player->playerType) {
            sprintf(filename.data(), "P%c000", character.model);
        } else if (exLoads.characterExLoadID == RealaEXLoad && !player->playerType) {
            sprintf(filename.data(), "EB0");
        } else if (exLoads.characterExLoadID == HatsuneMikuEXLoad && !player->playerType) {
            sprintf(filename.data(), "EB1");
        } else{
			sprintf(filename.data(), "P%c00", character.model);
		}

	}else if(player->gearType != Skates && player->extremeGear != ChaosEmerald){ // not skates
		IsSeparateBoardModelActive[player->index] = true;
		const Gear &gear = Gears[player->extremeGear];
		if(player->character == Storm && player->gearType == Bike){
			sprintf(filename.data(), "PB%dB", gear.model);
		}else if(player->extremeGear == HeavyBike){
			switch(player->character){
				case Ulala:
				case E10G:
				case E10R: sprintf(filename.data(), "PB20B");
					break;
				default: sprintf(filename.data(), "PB%d", gear.model);
			}
		}else if(exLoads.gearExLoadID == StardustSpeederEXLoad){ sprintf(filename.data(), "EB2"); }
		else{
			sprintf(filename.data(), "PB%d", gear.model);
		}
	}

	if(player->gearType != Skates && player->extremeGear != ChaosEmerald){
		char *file = DumpPackManFile(filename.data(), 0);
		SetArchiveBinary(file, index, 0);
	}

	if(player->character == TotalCharacterAmount){
		// for variable character, force it to shadow
		player->character = Shadow;
	}

}

ASMUsed void DumpBoardModelCSS(Player *player, const u32 index){
	//u8 controllerPort = player->input->port;
	EnabledEXLoads exLoads{};
	FetchEnabledEXLoadIDs(player, exLoads);

	IsSeparateBoardModelActive[index] = false;

    GearType gearType = Skates;
    if(player->extremeGear < ERider){
        gearType = Board;
    }else if(player->extremeGear < Darkness){
        gearType = Bike;
    }

	static char CSS_BoardFilename[4][8]; // Used to be global, Should this be static?

	if(player->extremeGear == DefaultGear){
		IsSeparateBoardModelActive[index] = true;
		const Character &character = Characters[player->character];
		if(exLoads.characterExLoadID == E10REXLoad){
			sprintf(CSS_BoardFilename[index], "P%c000", character.model);
		} else if (exLoads.characterExLoadID == RealaEXLoad) {
            sprintf(CSS_BoardFilename[index], "EB0");
		} else if (exLoads.characterExLoadID == HatsuneMikuEXLoad) {
            sprintf(CSS_BoardFilename[index], "EB1");
        } else {
            sprintf(CSS_BoardFilename[index], "P%c00", character.model);
        }

	}else if(gearType != Skates && player->extremeGear != ChaosEmerald){ // not skates
		IsSeparateBoardModelActive[index] = true;
		const Gear &gear = Gears[player->extremeGear];
		if(player->character == Storm && gearType == Bike){
			sprintf(CSS_BoardFilename[index], "PB%dB", gear.model);
		}else if(player->extremeGear == HeavyBike){
			switch(player->character){
				case Ulala:
				case E10G:
				case E10R: sprintf(CSS_BoardFilename[index], "PB20B");
					break;
				default: sprintf(CSS_BoardFilename[index], "PB%d", gear.model);
			}
		}else if(exLoads.gearExLoadID == StardustSpeederEXLoad){ sprintf(CSS_BoardFilename[index], "EB2"); }
		else{
			sprintf(CSS_BoardFilename[index], "PB%d", gear.model);
		}
	}

	if(gearType != Skates && player->extremeGear != ChaosEmerald){
		CSSModel &modelData = cssModel[index];
		modelData.count = lbl_0014CD08(CSS_BoardFilename[index], 1, 1, 0, &modelData.file, &modelData.status, 1, 1, index);
	}
}

constexpr std::array<unsigned char, 4> GearTypeChars = {
		'O', 'S', 'B', 'B'
};

ASMUsed void DumpCharacterModel(Player *player, const u32 index, char originalFilename[]){
	EnabledEXLoads exLoads{};
	FetchEnabledEXLoadIDs(player, exLoads);

	char *file;

    if (player->gearType != Skates && player->extremeGear != ChaosEmerald) {
		// not skates

		char filename[8];
        const char* exLoadFileName = GetEXLoadCharacterModel(player);

		if(exLoadFileName != nullptr){
			sprintf(filename, "%s", exLoadFileName);
		} else {
            sprintf(filename, "P%cC%c", Characters[player->character].model, GearTypeChars[player->gearType]);
        }

		file = DumpPackManFile(filename, 0);
	} else if (player->gearType == Skates) {
        char filename[8];
        const char* skateModel = GetEXLoadSkateModelStart(player);

        if (skateModel != nullptr) {
            const Gear &gear = Gears[player->extremeGear];
            sprintf(filename, "%s%d", skateModel, gear.model);

            file = DumpPackManFile(filename, 0);
        } else {
            file = DumpPackManFile(originalFilename, 0);
        }
	} else {
        file = DumpPackManFile(originalFilename, 0);
    }

    SetArchiveBinary(file, index, 0);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
ASMUsed void SetupCharacterModelCSS(Player *player, char filename[]){
	GearType gearType = Skates;
	if(player->extremeGear < ERider){
		gearType = Board;
	}else if(player->extremeGear < Darkness){
		gearType = Bike;
	}

	if(player->extremeGear == HeavyBike){
		switch(player->character){
			case Ulala:
			case E10G:
			case E10R: gearType = Board;
				break;
			default:
				break;
		}
	}

	if (gearType != Skates && player->extremeGear != ChaosEmerald) {
		// not skates

        const char* exLoadFileName = GetEXLoadCharacterModel(player);
        if (exLoadFileName != nullptr) {
            sprintf(filename, "%s", exLoadFileName);
        } else {
            sprintf(filename, "P%cC%c", Characters[player->character].model, GearTypeChars[gearType]);
        }
	} else if (gearType == Skates) {
        const char* skateModel = GetEXLoadSkateModelStart(player);

        if (skateModel != nullptr) {
            const Gear &gear = Gears[player->extremeGear];
            sprintf(filename, "%s%d", skateModel, gear.model);
        }
    }
}
#pragma GCC diagnostic pop

// this is handled automatically in the CSS by the game 

/*
void FreeBoardModelCSS(u32 index) {
    struct CSSModel *modelData = &CSSModel[index];
    free_Hi(modelData->file);
}
*/

void RenderBoardMesh(Player *player, void *objectDataInfo, void *boneVisibilityStatus){
	const u32 mirrorFlag = static_cast<const u32>(((player->unkBAC & 0x100) ? 1 : 0) * 0x40); // NOLINT(readability-implicit-bool-conversion)
	nnHookModelTextures(bss_BoardOnlyTextures[player->index]);

	if(static_cast<bool>(mirrorFlag)){
		nnDrawObjectExt(bss_BoardOnlyModelData[player->index], objectDataInfo, boneVisibilityStatus, 0x80000000,
		                        mirrorFlag);
	}else{
		nnDrawObjectLtd(bss_BoardOnlyModelData[player->index], objectDataInfo, boneVisibilityStatus,
		                           0x80000000, mirrorFlag);
	}
}

void RenderBoardMeshTimeTrial(Player *player, void *objectDataInfo, void *boneVisibilityStatus){
	nnHookModelTextures(bss_BoardOnlyTextures[player->index]);

	nnDrawObjectExt(bss_BoardOnlyModelData[player->index], objectDataInfo, boneVisibilityStatus, 0x80000000,
	                        0x02800400);
}

ASMUsed void
RenderBoardModelTimeTrial(Player *player, void *objectDataInfo, void *boneVisibilityStatus, void *weightVertPtr,
                          void *otherBoneData){
	u32 gearType = 1;
	if(player->extremeGear < ERider){
		gearType = 0;
	}else if(player->extremeGear < Darkness){
		gearType = 2;
	}

	if(gearType != 1 && IsSeparateBoardModelActive[player->index]){
		// not skates
		func_8004EBCC(weightVertPtr, bss_BoardOnlyModelData[player->index], lbl_1000DF64[player->index]);
		lbl_8004F4BC(*static_cast<u32 *>(weightVertPtr), otherBoneData);

		RenderBoardMeshTimeTrial(player, objectDataInfo, boneVisibilityStatus);
	}
}

ASMUsed void RenderBoardModel(Player *player, void *objectDataInfo, void *boneVisibilityStatus, u32 *weightVertPtr,
                              void *otherBoneData){
	u32 gearType = 1;
	if(player->extremeGear < ERider){
		gearType = 0;
	}else if(player->extremeGear < Darkness){
		gearType = 2;
	}

	if(gearType != 1 && IsSeparateBoardModelActive[player->index]){
		// not skates
		func_8004EBCC(weightVertPtr, bss_BoardOnlyModelData[player->index], lbl_1000DF64[player->index]);
		lbl_8004F4BC(*weightVertPtr, otherBoneData);

		RenderBoardMesh(player, objectDataInfo, boneVisibilityStatus);
	}
}

ASMUsed void
RenderBoardModelMultiplayer(Player *player, void *objectDataInfo, void *boneVisibilityStatus, u32 *weightVertPtr,
                            void *otherBoneData, u32 *someData){
	if(player->gearType != 1 && IsSeparateBoardModelActive[player->index]){
		// not skates
		func_8004EBCC(weightVertPtr, bss_BoardOnlyModelData[player->index], lbl_1000DF64[player->index]);
		*someData ^= 2; // fixes vertex weights on other players' screen
		lbl_8004F4BC(*weightVertPtr, otherBoneData);

		RenderBoardMesh(player, objectDataInfo, boneVisibilityStatus);
	}
}

ASMUsed void ScaleBoardModel(Player *player, Matrix3x3 *boneMatrices){
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

	if(player->character == Storm && player->gearType == 2){
		f32 scale = 1.25F; // NOLINT(readability-magic-numbers)
		// scale in all axis
		nnScaleModelBoneCustom(boneMatrices + 2, 0, &scale);
		nnScaleModelBoneCustom(boneMatrices + 2, 1, &scale);
		nnScaleModelBoneCustom(boneMatrices + 2, 2, &scale);
	}
}
