#pragma once

#include "main.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct GizoidReplicationInfo {
	f32 timer;
	f32 enableDelayTimer; // used mainly for slipstream and turbulence to delay enabling GR
	BOOL isEnabled;
	Player *slipstreamPlayer; // if using slipstream, will store the player pointer of 1st place in slipstream range, otherwise 0
	u32 currentType; // 0 if none
};
#pragma GCC diagnostic pop

extern GizoidReplicationInfo PlayerGizoidReplication[8];
extern const f32 GR_TypeShortcutSpeedMultiplier;
extern const f32 GR_TypeShortcutSpeedMultiplierDual;

void Player_GRHandler(Player *player);

ASMUsed void Player_GRPlayerBump(Player *player, Player *grPlayer);
ASMUsed void Player_GRAttacking(Player *player, Player *grPlayer);
ASMUsed void ClearData_GizoidReplication();
