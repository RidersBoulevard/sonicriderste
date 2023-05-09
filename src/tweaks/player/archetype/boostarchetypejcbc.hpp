#pragma once

#include "context.hpp"

struct DriftInfo {
	f32 lastDirectionDrifted;
};

extern DriftInfo PlayerDriftInfo[8];

global void lbl_0006E664();
global void lbl_Player_BoostEndFunction(Player*);
ASMUsed void Player_BoostArchetypeJCBC(Player *player);