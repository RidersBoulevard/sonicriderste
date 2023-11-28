#pragma once

#include "riders/player.hpp"

ASMDefined void OffroadJumpParticleTask();

struct PlayerLCancelInfo {
	u8 frameWindow;
	u8 lockoutTimer;
};

struct OffroadParticleObject1 {
	fillerData<0x30> filler;
	u8 playerIndex;
};

void Player_LCancelHandler(Player *player);