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

constexpr std::array<f32, 4> HangOn_ShiftSpeeds = {
        // top speed, boost speed
        pSpeed(230), // Bogey placeholder, unused.
        pSpeed(240), // gear 1->2, shift state 1
        pSpeed(245), // gear 1->2->3, shift state 2
		pSpeed(255), // gear 2->3, shift state 3
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct HangOnInfo {
        u32 lockoutFrameCounter;
		f32 shiftCount;
		u8 shiftState;
};
#pragma GCC diagnostic pop

extern std::array<HangOnInfo, MaxPlayerCount> PlayerHangOnInfo;

void Player_HangOn(Player *player);
