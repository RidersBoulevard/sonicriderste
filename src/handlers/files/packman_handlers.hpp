#pragma once

#include "riders/character.hpp"
#include "handlers/ingame/customtext.hpp"

constexpr u32 MAX_SKIN_COUNT = 4;

extern std::array<std::array<void*, MAX_SKIN_COUNT>, TotalCharacterAmount> CharacterSkinTextures;
extern m2darray<void*, MaxPlayerCount, 3> EggmeisterHeap;
extern SubFont CharacterInformationFont;
