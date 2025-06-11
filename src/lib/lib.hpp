#pragma once

#include "containers/vector3.hpp"
#include "globalDefs.hpp"
#include "riders/character.hpp"
#include "riders/hud.hpp"
#include "riders/player.hpp"

ASMDefined Vector3F gcosNnSystemVecZeroFast;
ASMDefined void* gpsTexList_Particle;

ASMDefined std::array<Matrix3x3F *, MaxPlayerCount> const gpsaMtxList_Player;
ASMDefined void nnMultiplyMatrix(Matrix3x3F *A, Matrix3x3F *B, Matrix3x3F &out);
ASMDefined void nnMakeTranslateMatrix(Matrix3x3F *M, f32 x, f32 y, f32 z);
ASMDefined void InitTextureHook(void *, u32);
ASMDefined void HookTexture(u32);
ASMDefined void CreateHUDElement(u32 x, u32 y, u32 xLength, u32 yLength, u32 rgba, u32, void *textureBoundaries);

ASMDefined void RenderText(
		s32 x,
		s32 y,
		u32 rgba,
		u32 textLength,
		u32 lineSpacing,
		u32 textID,
		void *textTable,
		void *textureStruct,
		f32);

ASMDefined u32 CenterText(u32 textLength, u32 lineSpacing, u32 textID, void *textTable);

/// Draws a simple box over the screen.
ASMDefined void Draw2d(u16 xPos, u16 yPos, u16 xLength, u16 yLength, u32 rgba, u32 always0_1, u32 always0_2, u32 always2);

ASMDefined void utilBinSetUpMotion(void* savePtr, void* gnmFile);

struct AttackObjReadManager{
	u8 motptrsID;
	u8 character;
};

ASMDefined std::array<AttackObjReadManager, 8> gsAttackObjReadManager;

struct AnimationIDInfo {
    std::array<u8*, 5> gearTypePtrs;
};

ASMDefined AnimationIDInfo lbl_001F1140[];
ASMDefined u16 tu16ExclusiveMotionNo;

ASMDefined std::array<void*, 0x1000> gpsaMotion_PlayerNeo;

constexpr std::array<u8, Character::Total+1> CharacterAnimationFiles = {
		0x0,
		0x1,
		0x2,
		0x3,
		0x4,
		0x5,
		0x6,
		0x7,
		0x8,
		0x9,
		0xA,
		0xB,
		0xC,
		0xD,
		0xE,
		0xF,
		0x10,
		0xA, // silver
		0x0, // metal sonic
		0x1, // emerl
		0x6, // blaze
		0x2, // chaos
		0x8, // tikal
		0xA, // variable character
};

void Custom_CreateHUDElement(void *textureStructs, u32 textureID, u32 textureRGBA, const HUDStruct *hud, const f32 aspectRatio = 0.0f);
void GetPlayerPositionRotationMatrix(Player &player, Matrix3x3F &out);
ASMUsed u32 SetupCustomAnimationArchive(u8 *packManStart, u32 *packManOffsetStart, u32 playerIndex);
Vector3F MatrixExtractTranslation(Matrix3x3F *m);

// getset_gno
ASMDefined void GetSet_Gno(void *packManStart, void *packManOffsetStart, void *savePtr);

struct GVRTextureArchive {
    u16 texNum;
    u16 flag; // either 0 or 1
    u32 offsetTable[];
};

struct GVRTexture {
    std::array<char, 4> gcixMagic;
    fillerData<0xC> data1;
    std::array<char, 4> gvrtMagic;
    u32 texSize; ///< (little endian) starting from the currently read offset, so this variable is 0x18 bytes less than the entire file
    fillerData<0x4> data2;
    u16 width;
    u16 height;
    u8 *texData;
};