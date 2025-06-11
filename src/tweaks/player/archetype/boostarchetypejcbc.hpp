#pragma once

#include "mechanics/magneticimpulse.hpp"

struct DriftInfo {
	f32 lastDirectionDrifted;
	bool hasHeldDrift;
};

struct NewCombatInfo {
	f32 attackBonusTimer;
};

extern std::array<DriftInfo, MaxPlayerCount> PlayerDriftInfo;
extern std::array<NewCombatInfo, MaxPlayerCount> PlayerCombatInfo;

ASMDefined void lbl_0006E664();
ASMUsed void Player_BoostArchetypeJCBC(Player &player);
