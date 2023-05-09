#pragma once

#include "context.hpp"

global void OffroadJumpParticleTask();

struct PlayerLCancelInfo {
	u8 frameWindow;
	u8 lockoutTimer;
};

struct OffroadParticleObject1 {
	char filler[0x30];
	u8 playerIndex;
};

void Player_LCancelHandler(Player *player);