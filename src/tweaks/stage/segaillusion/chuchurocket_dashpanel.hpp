#pragma once

#include "context.hpp"

constexpr f32 ChuChuDashPanelSpeeds[] = {
		pSpeed(20),
		pSpeed(0),
		pSpeed(100),
};

struct ChuChuRocketDashPanel {
	char filler[0x38];
	f32 speed;
	char filler2[0x8];
	struct ChuChuRocketDashPanelProperties *properties;
};

struct ChuChuRocketDashPanelProperties {
	char filler[0x40];
	u32 id;
};

ASMUsed f32 Player_SegaIllusion_ChuChuRocketDashPanelSpeed(Player *player, ChuChuRocketDashPanel *object);