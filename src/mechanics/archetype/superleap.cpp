#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "handlers/player/specialflagtweaks.hpp"

constexpr std::array<s32, 3> SuperLeap_LevelCosts = {30000, 45000, 60000};

ASMUsed u32 Player_SuperLeap(Player *player) {
	//u32 jumpHeight = (player->characterArchetype == TricksterArchetype) ? 0x13 : 0x12;
	u32 jumpHeight = 0x12;
	s32 afterburnActive;

	// if(player->extremeGear != ExtremeGear::Beginner || player->currentAir > 500) {
	// 	if(player->characterArchetype == CharacterArchetype::Trickster &&
	// 	   player->input->holdFaceButtons.hasAny(Buttons::L, Buttons::R) &&
	// 	   (player->state == PlayerState::Cruise || (player->extremeGear == ExtremeGear::AdvantageS && player->state == PlayerState::Run && !(player->currentAir <= 0)))) {
	// 		jumpHeight = 0xC + 2;
	// 		//jumpHeight += 2;
	// 		SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];
	// 		spfInfo->lightBoardEffect = false;
	// 		PlayAudioFromDAT(Sound::SFX::SuperLeap);
	// 		player->speed += pSpeed(115);
	// 		// if(player->extremeGear == TheCrazy) player->speed += pSpeed(50); // experimental
	// 		if(player->specialFlags.hasAny(SpecialFlags::ringGear)) {
	// 			// afterburnActive = (MI::impulseData[player->index].afterburnerTimer != 0) ? 5 : 5;
	// 			s32 newAir = player->currentAir - (player->gearStats[player->level].maxAir * 5) / 100;
	// 			if(newAir < 0) { newAir = 0; }
	// 			player->currentAir = newAir;
	// 		} else {
	// 			// afterburnActive = (MI::impulseData[player->index].afterburnerTimer != 0) ? 1 : 1;
	// 			player->changeInAir_loss += SuperLeap_LevelCosts[player->level];
	// 		}
	// 	}
	// }
	return jumpHeight;
}