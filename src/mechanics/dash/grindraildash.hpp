#pragma once

#include "riders/player.hpp"

constexpr std::array<s32, 3> GrindRailDashInitialCosts = {
        20000, 25000, 30000
};

constexpr std::array<f32, 3> GrindRailDashAirCosts = {
        -333.3f, -416.6f, -500.0f
};

constexpr std::array<s32, 3> GrindRailDashInitialCostsMechanic = {
		15000, 18750, 22500
};

constexpr std::array<f32, 3> GrindRailDashAirCostsMechanic = {
		-249.97500f, -312.45f, -375.0f
};

constexpr s32 GrindRailDashAirCostsRingGear = 30;

struct FillerAttackStruct {
	fillerData<0x34>  filler;
};

struct BoostParticleObject1 {
	fillerData<0x38> filler;
	Player *player;
	FillerAttackStruct *attackStruct;
	u32 unknown;
	f32 unkfloat;
};

ASMDefined void RingLoss_OnAttack(Player &player);
void Player_RingLossVisualsSFX(Player &player);
void Player_GrindRailDashHandler(Player &player);
void Player_InitBoostParticles(Player &player);