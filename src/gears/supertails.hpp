#pragma once

#include "riders/player.hpp"

struct SuperTailsInfo {
	bool playerMagnet; // Check for player induced magnet
    s32 tailwindTimer; // Timer for Tailwind mechanic
    bool transformCheck; // second check to change from state 1 -> 2
};

extern std::array<SuperTailsInfo, MaxPlayerCount> PlayerSuperTailsInfo;

namespace supertailsdef {
    constexpr auto MAGNETCOST = 10;
    constexpr auto REQUIREDTRANSFORMRINGS = 50; // How many rings needed to transform to super state
}



ASMDefined void lbl_0008CC74(u32);
ASMDefined void SNDF_SeqIdStop(u32);
ASMDefined void lbl_Player_BoostEndFunction(Player*);

void Player_SuperTailsTransformation(Player &player);
