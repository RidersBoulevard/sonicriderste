#pragma once

#include "context.hpp"

ASMUsed void lbl_DashPanelSpeed(f32 dashPanelSpeed, Player *player, Object *object);

constexpr f32 data_DashPanelSpeed[17] = {
		pSpeed(20.0f), // test stage
		pSpeed(20.0f), // metal city
		pSpeed(20.0f), // splash canyon
		pSpeed(20.0f), // egg factory
		pSpeed(20.0f), // green cave
		pSpeed(20.0f), // sand ruins
		pSpeed(20.0f), // babylon garden
		pSpeed(20.0f), // digital dimension
		pSpeed(40.0f), // sega carnival
		pSpeed(20.0f), // night chase
		pSpeed(40.0f), // red canyon
		pSpeed(20.0f), // ice factory
		pSpeed(20.0f), // white cave
		pSpeed(20.0f), // dark desert
		pSpeed(60.0f), // sky road
		pSpeed(60.0f), // babylon guardian
		pSpeed(20.0f), // sega illusion
};
