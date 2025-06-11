#pragma once
#include "riders/player.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"
#include "lib/stdlib.hpp"

struct AdvantagePInfo {
	std::array<s32, 2> TOPGauge;
    u8 TOP_Pos;
    bool isInTopMode;
    // bool TOPSelected;
};

extern std::array<AdvantagePInfo, MaxPlayerCount> PlayerAdvantagePInfo;

void Player_AdvantageP(Player *player);