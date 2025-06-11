#pragma once

#include "globalDefs.hpp"
#include "riders/character.hpp"
#include "riders/player.hpp"
#include "types.hpp"

/**
 * Stores all of the types of ways you can scroll through the skin system.
*/
enum SkinSystemTypes {
	NoneSkinSystemType,
	SkinSystemTypeForwards,
	SkinSystemTypeBackwards
};

struct SkinSystemInfo {
    u16 character; ///< Which character it applies to
    u16 cssTextureID; ///< What texture ID to use in the CSS portrait
    u32 textureArchiveIndex; ///< Which index texture archive needs to be used for this skin (in the CSP? file)
};

struct SkinSystemData {
    u8 skinID = 0;
    u8 delay = 0;
};

constexpr u32 SkinCount = 2;

constexpr std::array<SkinSystemInfo, SkinCount> PlayerSkinSystemInfo = {{
        {static_cast<u16>(-1), static_cast<u16>(-1), static_cast<u32>(-1)}, // this means no skin is applied
        {Character::Sonic, 0xA7, 0},
}};

extern std::array<SkinSystemData, MaxPlayerCount> PlayerSkinSystemData;

SkinSystemTypes CheckSkinSystemControls(const Player &player);
[[nodiscard]] bool IsSkinSystemControlPressed(const Player &player);