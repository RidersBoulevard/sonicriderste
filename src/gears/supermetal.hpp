#pragma once

#include "riders/player.hpp"

struct NeoMetalInfo {
  u8 chaosControlState; // 0 = inactive, 1 = active, 2 = burner state
  s8 chaosDelayTimer;
  s16 chaosControlTimer; // 5 seconds maximum, goes to -1 to inactive
  s32 chaosBurnerTimer; // depends on link amount
  u32 objectLink1;
  u32 objectLink2;
  u32 trickCount;
  u32 attackCount;
  u32 totalChaosActionCount;
  u32 chaosBonus;
  Type stolenType;
  u8 formState;

};

constexpr GearLevelStats SuperMetal_StandardArch = {
  200000,    // max air
  20,      // air drain 16
  250,       // drift cost 200
  33000,    // boost cost 27500 41250
  0x7530,    // tornado cost
  pSpeed(100), // drift dash speed
  pSpeed(230)// boost speed
  // 205 cruise
};

constexpr GearLevelStats SuperMetal_BurnState = {
  200000,    // max air
  20,      // air drain 16
  250,       // drift cost 200
  33000,    // boost cost 27500 41250
  0x7530,    // tornado cost
  pSpeed(100), // drift dash speed
  pSpeed(240)// boost speed
  // 205 cruise
};

constexpr GearLevelStats SuperMetal_SuperStatsII = {
  200000,    // max air
  155,      // air drain
  250,       // drift cost
  57750,    // boost cost
  0x7530,    // tornado cost
  pSpeed(100), // drift dash speed
  pSpeed(260)// boost speed
  // 165 cruise
};

extern std::array<NeoMetalInfo, MaxPlayerCount> PlayerNeoMetalInfo;

void Player_SuperMetalTransformation(Player &player);

void Player_SuperMetalChaosControl(Player &player);

void Player_SuperMetalIII(Player &player);