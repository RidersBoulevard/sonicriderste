#include "lib.hpp"
#include "cosmetics/player/exloads.hpp"
#include <array>

void Custom_CreateHUDElement(void* textureStructs, u32 textureID, u32 textureRGBA, HUDStruct *hud) {
    InitTextureHook(textureStructs, textureID);
    HookTexture(3);

	// calculate texture boundaries
	std::array<f32, 4> textureBoundaries = {
			static_cast<f32>(hud->textureX) * hud->scale,
			static_cast<f32>(hud->textureY) * hud->scale,
			static_cast<f32>(hud->textureX + hud->textureXLength) * hud->scale,
			static_cast<f32>(hud->textureY + hud->textureYLength) * hud->scale
	};

    const u32 xLength = hud->screenX + hud->textureXLength;
    const u32 yLength = hud->screenY + hud->textureYLength;

    CreateHUDElement(hud->screenX, hud->screenY, xLength, yLength, textureRGBA, 0, textureBoundaries.data());
}

void GetPlayerPositionRotationMatrix(Player &player, Matrix3x3 &out) {
    auto *mtxList = gpsaMtxList_Player[player.index];
    Matrix3x3 *mtx = mtxList + 2;
    nnMultiplyMatrix(&player.unkC4, mtx, out);
}

std::array<u8, 35> MikuAnimationMap = {
        0x01,
        0x02,
        0x0A,
        0x0B,
        0x0D,
        0x0E,
        0x0F,
        0x14,
        0x15,
        0x16,
        0x37,
        0x38,
        0x39,
        0x3A,
        0x3B,
        0x49,
        0x3E,
        0x3F,
        0x40,
        0x42,
        0x43,
        0x44,
        0x45,
        0x46,
        0x47,
        0x48,
        0x56,
        0x22,
        0x23,
        0x24,
        0x25,
        0x26,
        0x27,
        0x20,
        0xFF,
};

std::array<u8, TotalCharacterAmount+1> CharacterAnimationFiles = {
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

ASMUsed u32 SetupCustomAnimationArchive(u8 *packManStart, u32 *packManOffsetStart, u32 playerIndex) {
    u32 setupFileCount = 0;
    Player &player = players[playerIndex];
    AttackObjReadManager &attackObj = gsAttackObjReadManager[playerIndex];

    u8 character = CharacterAnimationFiles[player.character];
    u8 gearType = player.gearType;

    if (player.extremeGear == OpaOpa) {
        gearType = 3;
        attackObj.motptrsID = (character * 5) + gearType;
        attackObj.character = 0x11;
    }
    else {
        attackObj.motptrsID = (character * 5) + gearType;
        attackObj.character = character;
    }

    u8 *idInfo = lbl_001F1140[character].gearTypePtrs[gearType];

    /*
    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(&player, exLoads);

    if (exLoads.characterExLoadID == HatsuneMikuEXLoad) {
        idInfo = MikuAnimationMap.data();
    }
    */

    if (idInfo == nullptr) {
        return setupFileCount;
    }

    u32 exclusiveMotionIndex = player.gearType * 0x200 + tu16ExclusiveMotionNo;
    u32 *exclusiveMotions = reinterpret_cast<u32*>(&gpsaMotion_PlayerNeo[exclusiveMotionIndex]);

    while (true) {
        u32 offset = *packManOffsetStart;
        packManOffsetStart++;
        utilBinSetUpMotion(exclusiveMotions, packManStart + offset);

        setupFileCount++;
        exclusiveMotions++;

        u32 motionIndex = tu16ExclusiveMotionNo;
        tu16ExclusiveMotionNo++;

        u8 animationID = *idInfo;
        player.animationIDMap[animationID] = motionIndex;

        idInfo++;

        if (*idInfo == 0xFF)
            break;
    }

    return setupFileCount;
}