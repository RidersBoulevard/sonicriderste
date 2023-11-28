#pragma once

#include "riders/player.hpp"

struct MagnetObject1 {
	fillerData<0x3C> filler;
	u8 playerIndex;
};

ASMUsed void *SFX_Magnet(MagnetObject1 *object1, void *object);
ASMUsed void SFX_Death(Player *player);
ASMUsed void SFX_TenOrMoreTricks(Player *player, u32 isTrickNotFailed);
ASMUsed void SFX_HitByTornado();
