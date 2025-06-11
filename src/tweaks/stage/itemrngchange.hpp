#pragma once

#include "riders/object.hpp"
#include "riders/player.hpp"

constexpr auto ItemBoxCount = std::to_underlying(ItemID::RNG); //Todo: Is this an accurate name?

using ItemRNGChange_t = std::array<u8, ItemBoxCount>;
//using ItemRNGSpan_t = std::span<const u8, ItemBoxCount>;

constexpr ItemRNGChange_t DefaultItemRNGChange_level3AirGearRNG = {
		40, 0, 0, 0, // 40% 10 ring box
		30, 0, 30, 0,// 30% 50 air, 30% speed shoes
		0, 0, 0
};

constexpr ItemRNGChange_t DefaultItemRNGChange_RingGearRNG = {
		60, 0, 0, 0, // 60% 10 ring box
		20, 0, 20, 0,// 20% 50 air, 20% speed shoes
		0, 0, 0
};

ASMUsed const ItemRNGChange_t & ItemRNGChange(const Player &player, const ItemRNGChange_t &rngChances, u32 currentItem);
void RNGChangesReset(const Player &player);