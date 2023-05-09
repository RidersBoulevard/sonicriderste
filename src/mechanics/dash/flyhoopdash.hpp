#pragma once

#include "context.hpp"
#include "mechanics/dash/grindraildash.hpp"

extern const f32 FlyHoopDashAirLoss[3];
extern const s32 FlyHoopDashAirLossRingGear;
extern const s32 FlyHoopDashInitialCosts[3];


void Player_RingLossVisualsSFX(Player *player);
global void RingLoss_OnAttack(Player *player);
global void func_SetPlayerActionType(Player *player, u32 state);

void Player_FlyHoopDashHandler(Player *player);

struct PlayerFlyHoopDashProperties {
	BOOL canUse; // if player can use fly hoop dash
	u8 duration; // used to determine how long you can dash for
	f32 gainDelay;
	BOOL hasUsed;
};

extern PlayerFlyHoopDashProperties playerFlyHoopDashProperties[8];
