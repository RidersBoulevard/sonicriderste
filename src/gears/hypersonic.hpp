#pragma once

#include "riders/player.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct HyperSonicInfo {
	u32 totalObjectLinks; // stores first type shortcut links taken until rings are granted
	u32 extraObjectLinks; // stores links when a new type shortcut starts until rings are granted
    // u8 lastTypeShortcut;
	bool hyperdriveEnabled;
	bool previousHyperdrive;
	// Player *slipstreamPlayer; // if using slipstream, will store the player pointer of 1st place in slipstream range, otherwise null
	// u32 currentType; // 0 if none
};
#pragma GCC diagnostic pop

extern std::array<HyperSonicInfo, MaxPlayerCount> PlayerHyperSonicInfo;

ASMDefined {
void func_Particle_Task();
extern void *gpsTexList_Particle;
extern void *lbl_001D8F58;
void lbl_0008CC74(u32);

void Player_HyperSonicUpdatePlayerStats(Player *player);

void Player_HyperSonicTotalLinkCalc(Player *player);

void Player_HyperSonicRingStream(Player *player);

void Player_HyperdriveHandler(Player *player);

void Player_ZGSuperSonic(Player *player);

void Player_SuperShadow(Player *player);
}