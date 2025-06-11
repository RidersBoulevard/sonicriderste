#include "riders/player.hpp"

#include <cstring>

#include "gears/berserker.hpp"
#include "handlers/files/separatemodelloading.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "mechanics/dash/flyhoopdash.hpp"
#include "riders/gamemode.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "gears/advantagep.hpp"
#include "handlers/player/zbutton.hpp"
#include "gears/supertails.hpp"
#include "gears/blastGaugeGears.hpp"

void Player::reset() {
	// Preserve fields we want to keep
	const auto voiceID = this->characterVoiceID;

	// Copy the index since it gets cleared in the memset
	const auto index = this->index;

	// Clears the player struct for everything not below unkC4
	memset(&this->unkC4, 0, sizeof(Player) - offsetof(Player, unkC4));
	// Re-Init our extra player data using the default constructors
	MI::impulseData[index] = {};
	PlayerGizoidReplication[index] = {};
	playerFlyHoopDashProperties[index] = {};
	Player_BerserkerStatIncreaseMultipliers[index] = {};
	PlayerAdvantagePInfo[index] = {};
	PlayerZButtonHeldTimers[index] = {};
	PlayerSuperTailsInfo[index] = {};
    PlayerBlastGaugeInfo[index] = {};
	if(RaceExitMethod != ExitMethod::Retry) {// on retry, don't clear data
		IsSeparateBoardModelActive[index] = false;
	}

	// Copy our preserved fields back
	this->characterVoiceID = voiceID;
}
