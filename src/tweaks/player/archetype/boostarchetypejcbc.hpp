#pragma once

#include "mechanics/magneticimpulse.hpp"

struct DriftInfo {
	f32 lastDirectionDrifted;
	bool hasHeldDrift;
};

extern std::array<DriftInfo, MaxPlayerCount> PlayerDriftInfo;

ASMDefined void lbl_0006E664();
ASMDefined void lbl_Player_BoostEndFunction(Player*);
ASMUsed void Player_BoostArchetypeJCBC(Player *player);
void Player_windcatcher(Player *player);