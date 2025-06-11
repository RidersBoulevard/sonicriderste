#pragma once

#include "main.hpp"
#include "riders/player.hpp"
#include "handlers/player/initgeardata.hpp"

namespace Gambler{
	enum GearSpecific{
		Level4,

	};
}

//ASMUsed void Gambler_GainTypeOnLevelup(Player *player);
//ASMUsed void Gambler_LoseTypeOnLeveldown(Player *player);
void Player_GamblerBoost(Player *player);