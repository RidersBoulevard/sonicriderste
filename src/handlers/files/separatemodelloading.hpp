#pragma once

#include "../../../include/globalDefs.hpp"
#include "riders/player.hpp"
#include <bitset>

struct CSSModel {
    u32 count;
    void *file;
    u32 status;
    u32 padding;
};

extern std::array<CSSModel, MaxPlayerCount> cssModel;
extern std::array<CSSModel, MaxPlayerCount> cssSkinArchive;
extern std::array<CSSModel, MaxPlayerCount> cssEggmeisterTextures;

extern std::bitset<MaxPlayerCount> IsSeparateBoardModelActive;

ASMDefined void CSS_FreeAllHigh();
ASMDefined void CSS_FreeHigh(u32 count);

void FreeEggmeisterHeap(u32 index);
ASMUsed void ClearEggmeisterHeapAll();
[[nodiscard]] bool PlayerIsEggmeister(const Player &player);
[[nodiscard]] u32 GetEggmeisterGearID(const Player &player);
[[nodiscard]] u32 GetEggmeisterTextureArchiveIndex(const Player &player);
[[nodiscard]] u32 GetEggmeisterTextureIndexInArchive(const Player &player);
[[nodiscard]] std::string GetEggmeisterTextureArchiveFilename(const Player &player);

namespace Eggmeister {
    constexpr auto MAX_TEXTURE_COUNT_PER_ARCHIVE = 5; // NOT including the reflection texture!
}