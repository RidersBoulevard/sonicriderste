#pragma once

#include "riders/player.hpp"

constexpr std::array ChuChuDashPanelSpeeds = {
        pSpeed(20),
        pSpeed(0),
        pSpeed(100),
};

struct ChuChuRocketDashPanel {
	fillerData<0x38> filler;
	f32 speed;
	fillerData<0x8> filler2;
	struct ChuChuRocketDashPanelProperties *properties;
};

struct ChuChuRocketDashPanelProperties {
	fillerData<0x40> filler;
	u32 id;
};

ASMUsed f32 Player_SegaIllusion_ChuChuRocketDashPanelSpeed(Player *player, ChuChuRocketDashPanel *object);