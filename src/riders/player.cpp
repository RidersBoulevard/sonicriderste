#include "riders/player.hpp"

#include <cstring>

#include "gears/advantagep.hpp"
#include "gears/berserker.hpp"
#include "gears/blastGaugeGears.hpp"
#include "gears/faster.hpp"
#include "gears/hypersonic.hpp"
#include "gears/supermetal.hpp"
#include "gears/supertails.hpp"
#include "gears/hangon.hpp"
#include "gears/airtank.hpp"
#include "handlers/files/separatemodelloading.hpp"
#include "handlers/player/zbutton.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "mechanics/dash/flyhoopdash.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "mechanics/speed_shoes.hpp"
#include "riders/gamemode.hpp"
#include "tweaks/player/archetype/boostarchetypejcbc.hpp"

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
    PlayerHyperSonicInfo[index] = {};
    PlayerBlastGaugeInfo[index] = {};
    PlayerNeoMetalInfo[index] = {};
    player_speed_shoes[index] = {};
    PlayerHangOnInfo[index] = {};
    PlayerAcceleratorInfo[index] = {};
    PlayerAirTankInfo[index] = {};

    // don't clear data if player retried or game files were loaded outside of InitGame()
    if (gu32EndOfGameFlag != std::to_underlying(ExitMethod::Retry)) {
        IsSeparateBoardModelActive[index] = false;
    }

    // Copy our preserved fields back
    this->characterVoiceID = voiceID;
}

void Player::on_lap() {
}

/**
* A place to toss member functions that also need to be called from asm
*/
namespace asm_stuff {
    ASMUsed void Player_OnLap(Player &player) {
        player.on_lap();
    }
}
