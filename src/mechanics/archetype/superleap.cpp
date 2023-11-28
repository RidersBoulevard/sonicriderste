#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"
#include "mechanics/magneticimpulse.hpp"

ASMUsed u32 Player_SuperLeap(Player *player) {
	//u32 jumpHeight = (player->characterArchetype == TricksterArchetype) ? 0x13 : 0x12;
	u32 jumpHeight = 0x12;
	s32 afterburnActive;
	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
	// OKGInfo *okgInfo = &PlayerOKGInfo[player->index];

	if(player->extremeGear != ExtremeGear::Beginner || player->currentAir > 500) {
		if(player->characterArchetype == TricksterArchetype &&
		   player->input->holdFaceButtons.hasAny(LButton, RButton) &&
		   (player->state == Cruise || (player->extremeGear == ExtremeGear::AdvantageS && player->state == Run && !(player->currentAir <= 0)))) {
			jumpHeight = 0xC + 4;
			//jumpHeight += 2;
			PlayAudioFromDAT(Sound::SFX::SuperLeap);
			player->speed += pSpeed(115);
			// if(player->extremeGear == TheCrazy) player->speed += pSpeed(50); // experimental
			if(player->specialFlags.hasAny(ringGear)) {
				afterburnActive = (MI::impulseData[player->index].afterburnerTimer != 0) ? 5 : 5;
				s32 newAir = player->currentAir - (player->gearStats[player->level].maxAir * afterburnActive) / 100;
				if(newAir < 0) { newAir = 0; }
				player->currentAir = newAir;
			} else {
				afterburnActive = (MI::impulseData[player->index].afterburnerTimer != 0) ? 1 : 1;
				player->changeInAir_loss += (30000 / afterburnActive);
			}
		}
		// if (exLoads.gearExLoadID == OllieKingGearEXLoad) okgInfo->currentSpeed = player->speed;
	}
	return jumpHeight;
}