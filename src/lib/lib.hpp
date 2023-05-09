#pragma once

#include "context.hpp"
#include "containers/vector3.hpp"

global Vector3 gcosNnSystemVecZeroFast;
global void* gpsTexList_Particle;

global Matrix3x3* const gpsaMtxList_Player[8];
global void nnMultiplyMatrix(Matrix3x3 *A, Matrix3x3 *B, Matrix3x3 &out);
global void InitTextureHook(void *, u32);
global void HookTexture(u32);
global void CreateHUDElement(u32 x, u32 y, u32 xLength, u32 yLength, u32 rgba, u32, void *textureBoundaries);

global void RenderText(
		u32 x,
		u32 y,
		u32 rgba,
		u32 textLength,
		u32 lineSpacing,
		u32 textID,
		void *textTable,
		void *textureStruct,
		f32);

global u32 CenterText(u32 textLength, u32 lineSpacing, u32 textID, void *textTable);
global void utilBinSetUpMotion(void* savePtr, void* gnmFile);

global struct AttackObjReadManager{
    u8 motptrsID;
    u8 character;
} gsAttackObjReadManager[8];

struct AnimationIDInfo {
    u8* gearTypePtrs[5];
};

global AnimationIDInfo lbl_001F1140[];
global u16 tu16ExclusiveMotionNo;

global void* gpsaMotion_PlayerNeo[0x1000];

extern std::array<u8, TotalCharacterAmount+1> CharacterAnimationFiles;

void Custom_CreateHUDElement(void *textureStructs, u32 textureID, u32 textureRGBA, HUDStruct *hud);
void GetPlayerPositionRotationMatrix(Player &player, Matrix3x3 &out);
ASMUsed u32 SetupCustomAnimationArchive(u8 *packManStart, u32 *packManOffsetStart, u32 playerIndex);