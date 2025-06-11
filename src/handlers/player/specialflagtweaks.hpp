#pragma once

#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"
#include <cmath>
#include "riders/object.hpp"
#include "lib/stdlib.hpp"

struct SpecialFlagInfo {
	bool lightBoardEffect; // if the effect is active
    std::array<f32, 3> lvlTopSpeed;
    bool isBoosting;
    m2darray<f32, 3, 3> tierAccel;
    u32 gearChange;
    u32 totalBoostTime;
};

extern std::array<SpecialFlagInfo, MaxPlayerCount> PlayerSpecialFlagInfo;

ASMDefined void lbl_Player_BoostEndFunction(Player*);

void Player_lightBoardEffect(Player* player);

void Player_moneyCrisis(Player* player);

void Player_storeFlagInfo(Player* player);

ASMDefined const f32 data_C24748F0;

ASMUsed f32 Player_DecelModifier(Player* player);

ASMUsed u32 Player_MagnetTimer(Player* player, u32 currTime);

ASMUsed void Player_CustomRingPickup(Player* player);

ASMUsed void Player_ArchetypeTierAccelBuffs(Player* player);

ASMUsed void Player_ApplyBoostSpeed(Player* player);

ASMUsed void Player_TornadoBoostApplications(Player* player);

ASMUsed u32 CustomBoostDuration(Player *player, u32 currentBoostDuration);

ASMDefined void lbl_0008CBD4(); // bomb

ASMDefined void lbl_0008CB64(Player *player, u32 timer, f32 setSpeedCap); // ball and chain

ASMDefined void lbl_0008CB1C(u32); // ball and chain model

ASMUsed bool Player_ZIgnoreTornado(Player *player);

ASMUsed void Player_AirGainOnAttack(Player *player);

ASMDefined ObjectNode * gpsCurrentTask;
