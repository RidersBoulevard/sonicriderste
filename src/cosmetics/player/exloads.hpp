#pragma once

#include "context.hpp"
#include "containers/graphicalobject.hpp"

#define CharacterEXLoadSlots 8
#define GearEXLoadSlots 5

#define NotLockedGearSelectionID 0x1A
#define LockedGearSelectionID 0x1B

global void lbl_0013FA84(u32 soundID); /* asm, some different version of PlaySoundFromDAT function */
global u32 EXLoadHUDColors[]; /* asm */
global struct {
    f32 rgba[4];
    f32 rgba2[4];
} EXLoadBoostColors[]; /* asm */

struct CurrentEXLoadInfo {
    u8 exLoadIndex; // index to the current struct EXLoadInfo in use
    u8 exLoadDelayTime; // how many frames of delay before next input is read again, counts down to 0
};

struct EnabledEXLoads {
    u16 characterExLoadID;
    u16 gearExLoadID;
};

struct EXLoadColors {
    void *colors;
    u32 index;
};

struct EXLoadInfo {
    s8 exLoadType; // whether is a L load or a X load, or no ex load
    u8 exLoadID; // taken from enum EXLoads
    s8 character; // character it applies to, if none, it'll be -1 (0xFF)
    s8 gear; // gear it applies to, if none, it'll be -1 (0xFF)
    u16 portraitTextureID; // what texture ID it will use for the portrait of the ex load
    u16 gearTextTextureID; // what texture ID it will use for gear text if it's a gear ex load, otherwise -1
};

struct CSSObject {
    char filler[0x38];
    u8 cssSelectionState[4];
    char filler3[0x4];
    BOOL visualTrigger;
    char filler2[0x3];
    u8 visualTriggerTime;
};

extern CurrentEXLoadInfo Player_EXLoadData[8][2];

constexpr EXLoadInfo CharacterEXLoadDataSlots[CharacterEXLoadSlots] = {
		{
				NoneEXLoadType,
				NoneEXLoad,
				-1,
				-1,
				static_cast<u16>(-1), static_cast<u16>(-1)
		},
		{
				EXLoadType,
				E10REXLoad,
				E10G,
				-1,
				0xEE,                 static_cast<u16>(-1)
		},
        {
                LLoadType,
                NeoMetalEXLoad,
                MetalSonic,
                -1,
                0xEF,                 static_cast<u16>(-1)
        },
        {
                EXLoadType,
                E10BEXLoad,
                E10G,
                -1,
                0xBE,                 static_cast<u16>(-1)
        },
        {
                EXLoadType,
                HatsuneMikuEXLoad,
                Ulala,
                -1,
                0xF4,                 static_cast<u16>(-1)
        },
        {
                LLoadType,
                MetalSonicScarfEXLoad,
                MetalSonic,
                -1,
                0xD6,                 static_cast<u16>(-1)
        },
        {
                EXLoadType,
                RealaEXLoad,
                Nights,
                -1,
                0xF3,                 static_cast<u16>(-1)
        },
        {
                EXLoadType,
                GonGonEXLoad,
                Aiai,
                -1,
                0xF2,                 static_cast<u16>(-1)
        },
};

constexpr EXLoadInfo GearEXLoadDataSlots[GearEXLoadSlots] = {
		{
				NoneEXLoadType,
				NoneEXLoad,
				-1,
				-1,
				static_cast<u16>(-1), static_cast<u16>(-1)
		},
		{
				LLoadType,
				PerfectNazo,
				Sonic,
				ChaosEmerald,
				0xED,                 0x1A
		},
		{
				LLoadType,
				DarkSonicEXLoad,
				Sonic,
				ChaosEmerald,
				0xF0,                 0x1A
		},
		{
				EXLoadType,
				HyperSonicEXLoad,
				Sonic,
				ChaosEmerald,
				0xF1,                 0x1A
		},
		{
				EXLoadType,
				StardustSpeederEXLoad,
				-1,
				HighBooster,
				0xF5,                 0xF6
		}
};

constexpr f32 EXLoadWeights[EXLoadCount] = {
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
        0.95F, // hatsune miku
        -1.0F,
        1.1F, // reala
        1.4F, // gongon
};

constexpr CharacterArchetypes EXLoadArchetypes[EXLoadCount] = {
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        CombatArchetype, // hatsune miku
        NoEXLoadArchetype,
        BoostArchetype, // reala
        AllRounder, // gongon
};

ASMUsed void FetchEnabledEXLoadIDs(const Player *player, EnabledEXLoads &exLoads);
ASMUsed bool IsAnyE10EXLoad(const Player *player);
ASMUsed void ApplyEXLoadFancyVisuals(const Controller &controller, CSSObject &object);
ASMUsed void Character_UpdateGraphicalEXLoad(GraphicalObject *object);
ASMUsed void Gear_UpdateGraphicalEXLoad(GraphicalObject *object);
ASMUsed const char* GetEXLoadFilenameSuperForm(Player *player, const char filename[5]);
ASMUsed void _CheckEXLoadHUDColors();
ASMUsed void _CheckEXLoadBoostColors();

void CheckEXLoadHUDColors(Player *player, EXLoadColors &returnValues);
void CheckEXLoadBoostColors(Player *player, EXLoadColors &returnValues);

BOOL CheckIfEXLoadCanBeApplied(const Player &player, EXLoadInfo &info);
u32 RotateEXLoadAndFetch(Player *player, const EXLoadInfo array[], const u32 &length, const u32 &exLoadMode, const s8 &exLoadType);
void ApplyNextEXLoad(Player *player, const u32 &exLoadMode, const s8 &exLoadType);
void CheckPlayerEXLoadButton(Player *player, const u32 &exLoadMode);
const char* GetEXLoadCharacterModel(Player *player);
const char* GetEXLoadSkateModelStart(Player *player);