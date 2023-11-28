#pragma once

#include "riders/player.hpp"

namespace HangOn{
	enum GearSpecific{
		Level4,

	};
}

namespace SuperHangOn{
	enum GearSpecific{
		Level4,

	};
}

constexpr m2darray<f32, 4, 2> HangOn_Speeds = {{
        // top speed, boost speed
        {pSpeed(185), pSpeed(230)},// gear 1
        {pSpeed(215), pSpeed(250)}, // gear 2
        {pSpeed(235), pSpeed(265)}, // gear 3
}};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct HangOnInfo {
        u32 lockoutFrameCounter;
};
#pragma GCC diagnostic pop

extern std::array<HangOnInfo, MaxPlayerCount> PlayerHangOnInfo;

void Player_HangOn(Player *player);
void Player_HangOnAT(Player *player);
