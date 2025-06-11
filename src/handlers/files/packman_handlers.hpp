#pragma once

#include "handlers/ingame/customtext.hpp"
#include "riders/character.hpp"
#include "ninjanext.hpp"

constexpr u32 MAX_SKIN_COUNT = 4;

extern std::array<std::array<void*, MAX_SKIN_COUNT>, Character::Total> CharacterSkinTextures;
extern m2darray<void*, MaxPlayerCount, 3> EggmeisterHeap;
extern SubFont CharacterInformationFont;

extern std::array<NNS_MatMotion *, MaxPlayerCount> gpsaMatMotion_Player;
extern std::array<NNS_MatMotObj *, MaxPlayerCount> gpsaMatMotObj_Player;
extern SubFont LoadingScreenTipsFont;
extern void* LoadingScreenFontTexList;

extern NNS_Object *AfterburnerEffect_Object;
extern NNS_TexList *AfterburnerEffect_TexList;
extern NNS_MtxPal *AfterburnerEffect_MtxPal;
extern NNS_NodeStat *AfterburnerEffect_NodeStat;
extern NNS_Motion *AfterburnerEffect_Motion;
extern NNS_MatMotObj *AfterburnerEffect_MatMotObj;
extern NNS_MatMotion *AfterburnerEffect_MatMotion;