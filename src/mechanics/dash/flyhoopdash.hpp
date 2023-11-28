#pragma once

#include "mechanics/dash/grindraildash.hpp"
#include "riders/player.hpp"

constexpr std::array<s32, 3> FlyHoopDashInitialCosts = {
        // 40000, 45000, 50000
        30000, 35000, 40000
};

constexpr std::array<f32, 3> FlyHoopDashAirLoss = {
        -333.3f, -416.6f, -500.0f
};
constexpr std::array<s32, 3> FlyHoopDashInitialCostsMechanic = {
		// 40000, 45000, 50000
		22500, 26250, 30000
};

constexpr std::array<f32, 3> FlyHoopDashAirLossMechanic = {
		-249.97500f, -312.45f, -375.0f
};

constexpr s32 FlyHoopDashAirLossRingGear = 30;


void Player_RingLossVisualsSFX(Player *player);
ASMDefined void RingLoss_OnAttack(Player *player);
ASMDefined void func_SetPlayerActionType(Player *player, u32 state);

void Player_FlyHoopDashHandler(Player *player);

struct PlayerFlyHoopDashProperties {
	bool canUse = false; // if player can use fly hoop dash
	u8 duration = 0; // used to determine how long you can dash for
	f32 gainDelay = 0;
	bool hasUsed = false;
};

extern std::array<PlayerFlyHoopDashProperties, MaxPlayerCount> playerFlyHoopDashProperties;
