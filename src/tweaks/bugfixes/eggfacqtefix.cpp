#include "riders/object.hpp"
#include "riders/player.hpp"
#include "riders/stage.hpp"

struct EggFacQTEObject1 {
	std::array<char, 0x48> filler;
	std::array<u8, 8> playerQTEstates;
};

ASMUsed u32 EggFactoryQTEGlitchFix(Player *player, u32 respawnPoint) {
	if(CurrentStage != EggFactory) { return respawnPoint; }
	if(respawnPoint == 0x7 && player->state == PlayerState::QTE) { // the broken respawn point
		respawnPoint = 0x53;
		for(auto &object: getObjectList()) {
			if(object.object_group == 0x89E4) {
				static_cast<EggFacQTEObject1 *>(object.object)->playerQTEstates[player->index] = 0;
			}
		}
	}
	return respawnPoint;
}