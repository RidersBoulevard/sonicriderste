#pragma once
#include "context.hpp"

struct MagnetObject1 {
	char filler[0x3C];
	u8 playerIndex;
};

ASMUsed void* SFX_Magnet(MagnetObject1 *object1, void* object);
ASMUsed void SFX_Death(Player *player);
ASMUsed void SFX_TenOrMoreTricks(Player *player, u32 isTrickNotFailed);
ASMUsed void SFX_HitByTornado();
