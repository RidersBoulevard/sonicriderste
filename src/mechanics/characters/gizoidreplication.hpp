#pragma once

#include "riders/player.hpp"
#include "types.hpp"

enum GRStates {
	GR_None,
	GR_BumpingIntoPlayerState,
	GR_AttackingPlayerState,
	GR_TurbSlipstreamState
};

struct GizoidReplicationInfo {
	f32 timer = 0;
	f32 enableDelayTimer = 0;          // used mainly for slipstream and turbulence to delay enabling GR
	Player *slipstreamPlayer = nullptr;// if using slipstream, will store the player pointer of 1st place in slipstream range, otherwise nullptr
	bool isEnabled = false;
	Flag<Type> currentType = {};
};

extern std::array<GizoidReplicationInfo, MaxPlayerCount> PlayerGizoidReplication;
extern const f32 GR_TypeShortcutSpeedMultiplier;
extern const f32 GR_TypeShortcutSpeedMultiplierDual;

void Player_GRHandler(Player *player);

ASMUsed void Player_GRPlayerBump(Player *player, Player *grPlayer);
ASMUsed void Player_GRAttacking(Player *player, Player *grPlayer);
ASMUsed void ClearData_GizoidReplication();
