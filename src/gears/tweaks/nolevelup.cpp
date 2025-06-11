#include "cosmetics/player/exloads.hpp"
#include "riders/player.hpp"

ASMUsed SpecialFlags Player_NonLevelupGears(Player *player, SpecialFlags specialFlags) {
    //u8 controllerPort = player->input->port;
    const auto &exLoads = player->gearExload().exLoadID;
    if (isSuperCharacter(*player, Character::Tails) || // super tails
		isSuperCharacter(*player, Character::MetalSonic) || // super metal
        player->extremeGear == ExtremeGear::CoverF ||
        player->extremeGear == ExtremeGear::CoverS ||
        player->extremeGear == ExtremeGear::CoverP ||
        player->extremeGear == ExtremeGear::GunGear
        ) {
		specialFlags |= SpecialFlags::ringGear;
	}
    return specialFlags;
}