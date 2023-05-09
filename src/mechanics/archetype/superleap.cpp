#include "context.hpp"
#include "cosmetics/player/exloads.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "lib/sound.hpp"

ASMUsed u32 Player_SuperLeap(Player *player){
	//u32 jumpHeight = (player->characterArchetype == TricksterArchetype) ? 0x13 : 0x12;
	u32 jumpHeight = 0x12;
	s32 afterburnActive;
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	// struct OKGInfo *okgInfo = &PlayerOKGInfo[player->index];

	if(!(player->extremeGear == Beginner && player->currentAir <= 500)){
		if(player->characterArchetype == TricksterArchetype &&
		   player->input->holdFaceButtons & (LButton | RButton) &&
		   player->state == Cruise){
			PlayAudioFromDAT(Sound::SFX::SuperLeap);
			player->speed += pSpeed(100);
			if(player->extremeGear == TheCrazy) player->speed += pSpeed(50); // experimental
			jumpHeight += 2;
			if(player->specialFlags & ringGear){
				afterburnActive = (magneticImpulseData[player->index].afterburnerTimer != 0) ? 2 : 5;
				s32 newAir = player->currentAir - (player->gearStats[player->level].maxAir * afterburnActive) / 100;
				if(newAir < 0) newAir = 0;
				player->currentAir = newAir;
			}else{ 
				afterburnActive = (magneticImpulseData[player->index].afterburnerTimer != 0) ? 2 : 1;
				player->changeInAir_loss += (20000 / afterburnActive); 
				}

		}
		// if (exLoads.gearExLoadID == OllieKingGearEXLoad) okgInfo->currentSpeed = player->speed;
	}
	return jumpHeight;
}