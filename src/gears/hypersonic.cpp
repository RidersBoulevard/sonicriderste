#include "mechanics/dash/flyhoopdash.hpp"
#include "hypersonic.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"
#include "supertails.hpp"

HyperSonicInfo PlayerHyperSonicInfo[8];

void Player_CreateHyperSonicParticles(Player *player) {
	auto *particles = reinterpret_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, 0xB3B0, 2)->object);
	particles->unk72 = player->index;
	particles->unk0 = 0.0f;
	particles->unk8 = 0.0f;
	particles->unk10 = 0.0f;
	particles->unk18 = 0.0f;
	particles->unk4 = 0.3f;
	particles->unk14 = 0.8f;
	particles->unk68 = gpsTexList_Particle;
	particles->unk60 = &lbl_001D8F58;
	particles->unk73 = -1;
	particles->unk6C = 0;
	particles->unk74 = 0;
	particles->unk48 = &player->x;
}

void Player_HyperdriveHandler(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];

	if(exLoads.gearExLoadID != HyperSonicEXLoad) return;
	if(player->shortcutAirGainMultiplier >= 200000) {
		if(player->currentAir <= 500) player->currentAir = 500;
		if(hsInfo->hyperdriveEnabled == TRUE) return;
		hsInfo->hyperdriveEnabled = TRUE;
		Player_CreateHyperSonicParticles(player);
		PlayAudioFromDAT(Sound::SFX::HyperDrive);
	} else if(player->shortcutAirGainMultiplier < 0) {
		player->shortcutAirGainMultiplier = 0;
		hsInfo->hyperdriveEnabled = FALSE;
	}
}

void Player_HyperSonicUpdatePlayerStats(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];

	if(player->character == SuperSonic) {
		SNDF_SeqIdStop(player->index + 50);
		SNDF_SeqIdStop(player->index + 46);
	}

	if(exLoads.gearExLoadID != HyperSonicEXLoad) return;

	if(player->state == StartLine){
		// player->currentAir = 69300;
		player->currentAir = 55000;
		player->specialFlags &= ~thirtyPercentAir;
		player->specialFlags |= fiftyPercentAir;
		player->characterArchetype = BoostArchetype;
	}

	if(!player->aiControl && player->input->toggleFaceButtons & (BButton | XButton) && !(player->movementFlags & boosting) &&
	player->state == Cruise && (player->rings >= 30 || hsInfo->hyperdriveEnabled == TRUE)){
		PlayAudioFromDAT(Sound::SFX::HyperBoost);
	}

	if(hsInfo->hyperdriveEnabled == TRUE) {
		player->characterArchetype = SuperSonicArchetype;

		if ((player->input->toggleFaceButtons & (BButton | XButton) && !(player->movementFlags & boosting) &&
		player->state == Cruise)) player->shortcutAirGainMultiplier -= 5550;

		
		if(!(player->state >= Cruise && player->state <= Jump)) {// if not in jump and cruise, drain none
			player->shortcutAirGainMultiplier -= 0;
		} else {
			player->shortcutAirGainMultiplier -= 370;
		}// should take 9 seconds to drain
		player->gearStats[player->level].baseTopSpeed = pSpeed(215.0f);
		player->gearStats[player->level].topSpeed = pSpeed(215.0f);
		player->gearStats[player->level].airDrain = 0;
		player->gearStats[player->level].driftCost = 0;
		player->gearStats[player->level].boostCost = 0;
		player->gearStats[player->level].tornadoCost = 0;
		player->gearStats[player->level].boostSpeed = pSpeed(265.0f);
		hsInfo->previousHyperdrive = TRUE;
	} else if(hsInfo->hyperdriveEnabled == FALSE) {
		player->characterArchetype = BoostArchetype;
		if(player->movementFlags & 0x0400) {// has no air drain while boosting
			player->gearStats[player->level].airDrain = 0;
		} else {
			player->gearStats[player->level].airDrain = 65;
		}
		player->gearStats[player->level].baseTopSpeed = pSpeed(230.0f);
		player->gearStats[player->level].topSpeed = pSpeed(230.0f);
		player->gearStats[player->level].driftCost = 0x5;
		player->gearStats[player->level].boostCost = 33000;
		player->gearStats[player->level].tornadoCost = 5500;
		player->gearStats[player->level].boostSpeed = pSpeed(300.0f);
	}
}

void Player_HyperSonicTotalLinkCalc(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];

	if(exLoads.gearExLoadID != HyperSonicEXLoad) { return; }

	if(player->state == StartLine) {
		hsInfo->totalObjectLinks = 0;
		hsInfo->extraObjectLinks = 0;
		hsInfo->previousHyperdrive = FALSE;
	} 
	else if (player->state == AttackedByPlayer) {
		if (hsInfo->hyperdriveEnabled == TRUE) {
			hsInfo->hyperdriveEnabled = FALSE;
		}
	}

	if(player->hyperSonic_totalLinkTimer > 0) {
		if(hsInfo->totalObjectLinks < player->objectLinkCount) {
			hsInfo->totalObjectLinks = player->objectLinkCount;
		} else if(hsInfo->totalObjectLinks > player->objectLinkCount)//  && (player->objectLinkCount == 0)
		{
			hsInfo->extraObjectLinks += hsInfo->totalObjectLinks;
			hsInfo->totalObjectLinks = 0;
		}
	}

	if(player->hyperSonic_totalLinkTimer == 1) {
		Player_CreateHyperSonicParticles(player);
		PlayAudioFromDAT(Sound::SFX::HyperStream);
		if(hsInfo->previousHyperdrive == TRUE) {
			player->currentAir += static_cast<s32>(player->gearStats[player->level].maxAir
			                                       * static_cast<s32>((hsInfo->extraObjectLinks + hsInfo->totalObjectLinks) * 2.5f))
			                      / 100;// 50% extra
			if(hsInfo->totalObjectLinks == 0) { player->currentAir += (player->gearStats[player->level].maxAir * 4) / 100; }
		} else if(hsInfo->previousHyperdrive == FALSE) {
			player->currentAir += static_cast<s32>(player->gearStats[player->level].maxAir
			                                       * static_cast<s32>((hsInfo->extraObjectLinks + hsInfo->totalObjectLinks) * 2))
			                      / 100;
			if(hsInfo->totalObjectLinks == 0) { player->currentAir += (player->gearStats[player->level].maxAir * 2) / 100; }
		}
		hsInfo->previousHyperdrive = FALSE;
		hsInfo->totalObjectLinks = 0;
		hsInfo->extraObjectLinks = 0;
	}
}

void Player_HyperSonicRingStream(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if(exLoads.gearExLoadID != HyperSonicEXLoad) { return; }

	if(player->state == RailGrind
	   || player->state == Fly
	   || player->unkD90 >= 1) {
		player->hyperSonic_totalLinkTimer = 181;
	}
}