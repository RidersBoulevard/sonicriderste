#pragma once

#include "context.hpp"

#define DashPadParticle 0xB112

struct PlayerWindParticles {
	// 0x4C in size
	u32 data[0x13];
};

global PlayerWindParticles playerWindParticles[];

void lbl_Slipstream(Player *player);
