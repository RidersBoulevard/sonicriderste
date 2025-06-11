#pragma once

#include "riders/player.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct SuperShadowInfo {
	bool inhibitorRingsOff;
	u8 chaosBoostControl;
};
#pragma GCC diagnostic pop

extern std::array<SuperShadowInfo, MaxPlayerCount> PlayerSuperShadowInfo;

ASMDefined std::array<u32, Character::Total> SuperFormHUDColors;
ASMDefined std::array<u32, MaxPlayerCount> SuperNeoMetalHUDColors;