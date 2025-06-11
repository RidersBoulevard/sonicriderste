#pragma once

#include "riders/player.hpp"

constexpr auto ItemBoxCount = 0xB; //Todo: Is this an accurate name?

constexpr std::array<u8, ItemBoxCount> DefaultItemRNGChange_level3AirGearRNG = {
		40, 0, 0, 0, // 40% 10 ring box
		30, 0, 30, 0,// 30% 50 air, 30% speed shoes
		0, 0, 0
};

constexpr std::array<u8, ItemBoxCount> DefaultItemRNGChange_RingGearRNG = {
		60, 0, 0, 0, // 60% 10 ring box
		20, 0, 20, 0,// 20% 50 air, 20% speed shoes
		0, 0, 0
};

ASMUsed const void *ItemRNGChange(const Player *player, const u8 *rngChances, u32 currentItem);
void RNGChangesReset(const Player *player);