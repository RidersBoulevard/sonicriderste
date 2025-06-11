#pragma once

#include "containers/vector3.hpp"
#include "globalDefs.hpp"
#include "riders/character.hpp"
#include "riders/hud.hpp"
#include "riders/player.hpp"
#include "gvr_texture.hpp"
#include "nn/struct/object.hpp"

ASMDefined Vector3F gcosNnSystemVecZeroFast;
ASMDefined void* gpsTexList_Particle;

ASMDefined const std::array<Matrix3x4F *, MaxPlayerCount> gpsaMtxList_Player;
ASMDefined void InitTextureHook(void *, u32);
ASMDefined void HookTexture(u32);
ASMDefined void CreateHUDElement(u32 x, u32 y, u32 xLength, u32 yLength, u32 rgba, u32, void *textureBoundaries);
ASMDefined void Draw2dEnd();

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
ASMDefined void Draw2d(u16 xPos, u16 yPos, u16 xLength, u16 yLength, u32 rgba, u32 always0_1 = 0, u32 always0_2 = 0, u32 always2 = 2);

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

void Custom_CreateHUDElement(void *textureStructs, u32 textureID, u32 textureRGBA, const HUDStruct *hud, f32 aspectRatio = 0.0f);
void GetPlayerPositionRotationMatrix(Player &player, Matrix3x4F &out);

/**
 * Loads an animation file for a player and saves it to the specified ID in the player's animation map.
 *
 * @param player The player to load it on.
 * @param animationFile The animation file to load.
 * @param id The desired ID in the animation map of the player to save the animation's ID to.
 */
ASMUsed void LoadPlayerCustomAnimationID(Player &player, void *animationFile, const u8 id);

/**
 * Loads an animation file for a player, but does not save it to the player's animation map itself.
 *
 * @param player The player to load it on.
 * @param animationFile The animation file to load.
 * @return The ID of the newly loaded animation file to be saved in the animation map
 */
ASMUsed u16 LoadPlayerCustomAnimationDeferred(Player &player, void *animationFile);
ASMUsed u32 SetupCustomAnimationArchive(u8 *packManStart, u32 *packManOffsetStart, u32 playerIndex);
Vector3F MatrixExtractTranslation(Matrix3x4F *m);

ASMDefined {

/**
 * Loads a singular texture file into the given texture struct
 * (usually this texture struct would be contained within a texture list).
 *
 * @param tex The texture struct to load into.
 * @param info Texture specific info. Leave this as the default value and it'll work most likely.
 * @param gvrTexture The GVR texture to load.
 * @return 1 if successful, -1 if not.
 */
s32 utilTexLoadFileOne(TexStruct *tex, const TexInfo *info = &defaultTexInfo, const void *gvrTexture = nullptr);

/**
 * Sets up a .gno model file in a PackMan archive, and saves a pointer to it.
 *
 * @param packManStart The start of the PackMan archive file.
 * @param packManOffsetStart A pointer to the offset from the PackMan archive's start that points to the .gno model file.
 * @param savePtr Where to save a pointer to the .gno model file to.
 */
void GetSet_Gno(void *packManStart, void *packManOffsetStart, void *savePtr);

}
