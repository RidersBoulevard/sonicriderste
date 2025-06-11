#pragma once

#include "handlers/menu/debugmenu/debugmenu.hpp"
#include "riders/player.hpp"

struct AcceleratorInfo {
  bool isOverheated;
  // bool TOPSelected;
};

extern std::array<AcceleratorInfo, MaxPlayerCount> PlayerAcceleratorInfo;

void Player_Faster(Player &player);