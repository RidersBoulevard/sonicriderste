#pragma once

#include "riders/player.hpp"

constexpr auto DashPadParticle = 0xB112;

struct PlayerWindParticles {
	// 0x4C in size
	std::array<u32, 0x13> data;
};

ASMDefined PlayerWindParticles playerWindParticles[];

void lbl_Slipstream(Player &player);
