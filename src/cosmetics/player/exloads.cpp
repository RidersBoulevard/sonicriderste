#include "exloads.hpp"
#include "macros.hpp"
#include "lib/lib.hpp"

global void *gp2DSys;

// first size 8 array for all players in the match

// next size 2 array for both gear and character ex loads
// ^ index 0 being character ex load
// ^ index 1 being gear ex load
CurrentEXLoadInfo Player_EXLoadData[8][2];

void FetchEnabledEXLoadIDs(const Player *player, EnabledEXLoads &exLoads) {
	if(player->aiControl && player->playerType) { return; }

    exLoads.characterExLoadID = 0;
    exLoads.gearExLoadID = 0;

	const u8 controllerPort = player->input->port;
	u16 exLoadIndex;

	if((exLoadIndex = Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex)) {
		exLoads.characterExLoadID = CharacterEXLoadDataSlots[exLoadIndex].exLoadID;
	}
	if((exLoadIndex = Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadIndex)) {
		exLoads.gearExLoadID = GearEXLoadDataSlots[exLoadIndex].exLoadID;
	}
}

ASMUsed bool IsAnyE10EXLoad(const Player *player) {
    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(player, exLoads);
    return (exLoads.characterExLoadID == E10BEXLoad || exLoads.characterExLoadID == E10REXLoad);
}

void CheckEXLoadHUDColors(Player *player, EXLoadColors &returnValues) {
	returnValues.colors = nullptr;
	returnValues.index = 0;

	if(player->aiControl && player->playerType) { return; }

    EnabledEXLoads enabledExLoads{};
    FetchEnabledEXLoadIDs(player, enabledExLoads);
    if (enabledExLoads.characterExLoadID == E10BEXLoad) return;

	const u8 controllerPort = player->input->port;
	u8 exLoadIndex;
	if((exLoadIndex = Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex)) {
		if(EXLoadHUDColors[CharacterEXLoadDataSlots[exLoadIndex].exLoadID]) {
			returnValues.colors = &EXLoadHUDColors[0];
			returnValues.index = CharacterEXLoadDataSlots[exLoadIndex].exLoadID;
			return;
		}
	}
	if((exLoadIndex = Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadIndex)) {
		if(EXLoadHUDColors[GearEXLoadDataSlots[exLoadIndex].exLoadID]) {
			returnValues.colors = &EXLoadHUDColors[0];
			returnValues.index = GearEXLoadDataSlots[exLoadIndex].exLoadID;
			return;
		}
	}
}

void CheckEXLoadBoostColors(Player *player, EXLoadColors &returnValues) {
	returnValues.colors = nullptr;
	returnValues.index = 0;

	if(player->aiControl && player->playerType) { return; }

    EnabledEXLoads enabledExLoads{};
    FetchEnabledEXLoadIDs(player, enabledExLoads);
    if (enabledExLoads.characterExLoadID == E10BEXLoad) return;

	const u8 controllerPort = player->input->port;
	u8 exLoadIndex;

	if((exLoadIndex = Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex)) {
		if(EXLoadBoostColors[CharacterEXLoadDataSlots[exLoadIndex].exLoadID].rgba[0] != -1.0f) {
			returnValues.colors = &EXLoadBoostColors[0];
			returnValues.index = CharacterEXLoadDataSlots[exLoadIndex].exLoadID;
			return;
		}
	}
	if((exLoadIndex = Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadIndex)) {
		if(EXLoadBoostColors[GearEXLoadDataSlots[exLoadIndex].exLoadID].rgba[0] != -1.0f) {
			returnValues.colors = &EXLoadBoostColors[0];
			returnValues.index = GearEXLoadDataSlots[exLoadIndex].exLoadID;
			return;
		}
	}
}

ASMUsed void _CheckEXLoadHUDColors() {
	asm(
	        "    stwu r1, -0x90 (r1)\n"
	        "    stw r0, 0x8 (r1)\n"
	        "    mflr r0\n"
	        "    stw r0, 0xC (r1)\n"
	        "    stmw r5, 0x10 (r1)\n"
	        "    bl %[HUDColors]\n"
	        "    lwz r0, 0xC (r1)\n"
	        "    mtlr r0\n"
	        "    lwz r0, 0x8 (r1)\n"
	        "    lmw r5, 0x10 (r1)\n"
	        "    addi r1, r1, 0x90\n"
	        :
	        : [HUDColors] "i"(CheckEXLoadHUDColors));
}

ASMUsed void _CheckEXLoadBoostColors() {
	asm(
	        "    stwu r1, -0x90 (r1)\n"
	        "    stw r0, 0x8 (r1)\n"
	        "    mflr r0\n"
	        "    stw r0, 0xC (r1)\n"
	        "    stmw r5, 0x10 (r1)\n"
	        "    bl %[BoostColors]\n"
	        "    lwz r0, 0xC (r1)\n"
	        "    mtlr r0\n"
	        "    lwz r0, 0x8 (r1)\n"
	        "    lmw r5, 0x10 (r1)\n"
	        "    addi r1, r1, 0x90\n"
	        :
	        : [BoostColors] "i"(CheckEXLoadBoostColors));
}

USED void ApplyEXLoadFancyVisuals(const Controller &controller, CSSObject &object) {
	/*
	this function checks whenever the player is pressing any of the ex load buttons
	if they are, it'll call the sound effect and visuals for when characters/gears are switched naturally on CSS
	*/
	const u8 controllerPort = controller.port;

	if(Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadDelayTime == 0 && Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadDelayTime == 0) {

		if(controller.holdFaceButtons & (XButton | LButton)
		   || controller.rightStickHorizontal > 10
		   || controller.rightStickHorizontal < -10) {

			object.visualTrigger = TRUE;
			object.visualTriggerTime = 1;
			lbl_0013FA84(0xA9000000);
			lbl_0013FA84(0xA9000800);
		}
	}
}

bool CheckIfEXLoadCanBeApplied(const Player &player, EXLoadInfo &info) {
	bool canApplyCharacter;
	if(info.character != -1) {
        // quick exception for e10b character id, those exloads are always allowed through
		canApplyCharacter = (info.character == player.character || (info.character == E10G && player.character == E10R)) ? TRUE : FALSE;
	} else {
		canApplyCharacter = TRUE;
	}

	bool canApplyGear;
	if(info.gear != -1) {
		canApplyGear = (info.gear == player.extremeGear) ? TRUE : FALSE;
	} else {
		canApplyGear = TRUE;
	}

	return canApplyCharacter && canApplyGear;
}

ASMUsed void UpdateEXLoadData(Player *player) {
	EXLoadInfo info;// NOLINT(cppcoreguidelines-pro-type-member-init)
	u8 controllerPort = player->input->port;
	u16 exLoadIndex;

	if((exLoadIndex = Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex)) {
		info = CharacterEXLoadDataSlots[exLoadIndex];

		if(!CheckIfEXLoadCanBeApplied(*player, info))
			Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex = 0;
	}
	if((exLoadIndex = Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadIndex)) {
		info = GearEXLoadDataSlots[exLoadIndex];

		if(!CheckIfEXLoadCanBeApplied(*player, info))
			Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadIndex = 0;
	}
}

u32 RotateEXLoadAndFetch(const Player &player, const EXLoadInfo array[], const u32 &length, const u32 &exLoadMode,
                         const s8 &exLoadType) {
	bool toRotate = TRUE;
	const u8 controllerPort = player.input->port;
	u32 i = Player_EXLoadData[controllerPort][exLoadMode].exLoadIndex;
	do {
		if(exLoadType == AllEXLoadTypesForwards || exLoadType == EXLoadType || exLoadType == LLoadType) {
			i += 1;
			if(i >= length) i = 0;
		} else if(exLoadType == AllEXLoadTypesBackwards) {
			i -= 1;
			if(static_cast<s32>(i) < 0) i = length - 1;
		}

		EXLoadInfo current = array[i];

		if(current.exLoadType == NoneEXLoadType) toRotate = FALSE;

		else if(current.exLoadType == exLoadType || exLoadType == AllEXLoadTypesBackwards || exLoadType == AllEXLoadTypesForwards) {
			toRotate = !CheckIfEXLoadCanBeApplied(player, current);
		}

	} while(toRotate);

	return i;
}

void ApplyNextEXLoad(const Player &player, const u32 &exLoadMode, const s8 &exLoadType) {
	//bool canApplyCharacter, canApplyGear;
	//EXLoadInfo info;

	u32 exLoadIndex = 0;
	if(exLoadMode == CharacterEXLoadMode) {
		exLoadIndex = RotateEXLoadAndFetch(player, CharacterEXLoadDataSlots, CharacterEXLoadSlots, exLoadMode,
		                                   exLoadType);
		//info = CharacterEXLoadDataSlots[exLoadIndex];
	} else if(exLoadMode == GearEXLoadMode) {
		exLoadIndex = RotateEXLoadAndFetch(player, GearEXLoadDataSlots, GearEXLoadSlots, exLoadMode, exLoadType);
		//info = GearEXLoadDataSlots[exLoadIndex];
	}

	if(exLoadMode < 2) {
		u8 controllerPort = player.input->port;

		Player_EXLoadData[controllerPort][exLoadMode].exLoadIndex = exLoadIndex;
		Player_EXLoadData[controllerPort][exLoadMode].exLoadDelayTime = 15;
	}
}

void CheckPlayerEXLoadButton(const Player &player, const u32 &exLoadMode) {
	u8 controllerPort = player.input->port;
	u8 delayTime = Player_EXLoadData[controllerPort][exLoadMode].exLoadDelayTime;

	if(delayTime == 0) {
		if(player.input->holdFaceButtons & XButton) {
			ApplyNextEXLoad(player, exLoadMode, EXLoadType);
		} else if(player.input->holdFaceButtons & LButton) {
			ApplyNextEXLoad(player, exLoadMode, LLoadType);
		} else if(player.input->rightStickHorizontal > 10) {
			ApplyNextEXLoad(player, exLoadMode, AllEXLoadTypesForwards);
		} else if(player.input->rightStickHorizontal < -10) {
			ApplyNextEXLoad(player, exLoadMode, AllEXLoadTypesBackwards);
		}
	}

	delayTime = Player_EXLoadData[controllerPort][exLoadMode].exLoadDelayTime - 1;
	delayTime = static_cast<u8>(clamp<s8>(static_cast<s8>(delayTime), 0));
	Player_EXLoadData[controllerPort][exLoadMode].exLoadDelayTime = delayTime;
}

USED void Character_UpdateGraphicalEXLoad(GraphicalObject *object) {
	const Player &player = players[object->idStruct.idIndex];
	u8 controllerPort = player.input->port;
	u8 exLoadIndex;

	if(!(object->active & 0x8)) return;

	CheckPlayerEXLoadButton(player, CharacterEXLoadMode);

	exLoadIndex = Player_EXLoadData[controllerPort][CharacterEXLoadMode].exLoadIndex;

	EXLoadInfo info = CharacterEXLoadDataSlots[exLoadIndex];

	if(exLoadIndex != 0) {
		if(CheckIfEXLoadCanBeApplied(player, info)) {
            if (IsAnyE10EXLoad(&player) && player.character == E10R) return;

			if (static_cast<s16>(info.portraitTextureID) != -1)
				object->textureIDs->textureSlot1 = info.portraitTextureID;
		}
	}
}

USED void Gear_UpdateGraphicalEXLoad(GraphicalObject *object) {
	// not locked in gear selection object uses textureSlot1 for portrait, textureSlot5 for gear text
	// locked in gear selection object uses textureSlot4 for gear text

	if(!(object->active & 0x8)) { return; }

	const Player &player = players[object->idStruct.idIndex];
	u8 controllerPort = player.input->port;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-reinterpret-cast"
	void *objectptr = reinterpret_cast<u8 *>(gp2DSys) + 0x6038;
	void **objectptr2 = reinterpret_cast<void **>(objectptr);
	auto *cssObject = reinterpret_cast<CSSObject *>(*objectptr2);
#pragma clang diagnostic pop

	if(cssObject->cssSelectionState[controllerPort] < 4) {
		CheckPlayerEXLoadButton(player, GearEXLoadMode);
	}

	u8 exLoadIndex = Player_EXLoadData[controllerPort][GearEXLoadMode].exLoadIndex;
	EXLoadInfo info = GearEXLoadDataSlots[exLoadIndex];

	if(object->idStruct.graphicalDataID == NotLockedGearSelectionID) {
		if(exLoadIndex != 0) {
			if(CheckIfEXLoadCanBeApplied(player, info)) {
				if(static_cast<s16>(info.portraitTextureID) != -1)
					object->textureIDs->textureSlot1 = info.portraitTextureID;

				if(static_cast<s16>(info.portraitTextureID) != -1)
					object->textureIDs->textureSlot5 = info.gearTextTextureID;
			}
		}
	} else if(object->idStruct.graphicalDataID == LockedGearSelectionID) {
		if(exLoadIndex != 0) {
			if(CheckIfEXLoadCanBeApplied(player, info)) {
				if(static_cast<s16>(info.portraitTextureID) != -1)
					object->textureIDs->textureSlot4 = info.gearTextTextureID;
			}
		}
	}
}

ASMUsed void HatsuneMiku_BindLeekToBoneLvl3Attack(Player &player, Matrix3x3 &savePtr) {
    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(&player, exLoads);
    if (exLoads.characterExLoadID == HatsuneMikuEXLoad) {
        Matrix3x3* mtxList = gpsaMtxList_Player[player.index];
        u32 const rightHandBone = 43;
        nnMultiplyMatrix(&player.unkC4, mtxList + rightHandBone, savePtr);
    }
}

// Used for all EX loads that are applied to Chaos Emerald.
USED const char *GetEXLoadFilenameSuperForm(Player *player, const char filename[5]) {
	EnabledEXLoads exLoads{};
	FetchEnabledEXLoadIDs(player, exLoads);

	if (player->extremeGear != ChaosEmerald) {
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

// Used for all EX loads that aren't Chaos Emerald EX loads.
const char* GetEXLoadCharacterModel(Player *player) {
    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(player, exLoads);

    switch (exLoads.characterExLoadID) {
        case NeoMetalEXLoad:
            return "EC4";
        case HatsuneMikuEXLoad:
            return "EC0";
        case MetalSonicScarfEXLoad:
            return "EC1";
        case RealaEXLoad:
            return "EC2";
        case GonGonEXLoad:
            return "EC3";
        default:
            return nullptr;
    }
}

const char* GetEXLoadSkateModelStart(Player *player) {
    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(player, exLoads);

    switch (exLoads.characterExLoadID) {
        case HatsuneMikuEXLoad:
            return "S0";
        case RealaEXLoad:
            return "S2";
        case GonGonEXLoad:
            return "S3";
        default:
            return nullptr;
    }
}