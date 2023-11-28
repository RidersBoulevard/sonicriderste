#include "coverp.hpp"
#include "cosmetics/player/exloads.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"
#include "riders/object.hpp"

inline u32 Player_Coverp_ChangeWeight(Player *player, u32 weightState) {
	u32 weightCase;
	if(player->coverP_weightState == weightState) { weightCase = 2; }// same mode toggled, does nothing
	else if(player->rings < 10) {
		weightCase = 0;
	}                       // rings less than requirement does nothing
	else { weightCase = 1; }// different mode toggled

	return weightCase;
}

void Player_CreateCoverPParticles(Player *player) {
	auto *particles = static_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, 2)->object);
	particles->unk72 = player->index;
	particles->x = 0.0F;
	particles->z = 0.0F;
	particles->unk10 = 0.0F;
	particles->unk18 = 0.0F;
	particles->y = 0.3F;
	particles->unk14 = 0.8F;
	particles->unk68 = gpsTexList_Particle;
	particles->unk60 = &lbl_001D8F58;
	particles->unk73 = static_cast<u8>(~0);
	particles->unk6C = nullptr;
	particles->unk74 = 0;
	particles->unk48 = &player->x;
}

void Player_CoverP(Player *player) {
	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);

	if(player->extremeGear != ExtremeGear::CoverP) { return; }
	player->level = 1;
	// player->gearStats[player->level].topSpeed = player->gearStats[0].topSpeed;
	f32 originalWeight = CharacterWeights[player->character] + 0.4f;
	// TODO: change below to when we fix exload unloading
	if(exLoads.characterExLoadID == RealaEXLoad || exLoads.characterExLoadID == GonGonEXLoad) {
		originalWeight = EXLoadWeights[exLoads.characterExLoadID] + 0.4f;
	}

	// player->gearStats[0].airDrain = 10;
	// player->gearStats[0].boostCost = 0x61A8 * 0.80f;
	// player->gearStats[0].driftCost = 0xA6;
	// player->gearStats[0].tornadoCost = 0x61A8* 0.80f;

	if(player->state == StartLine) {
		player->coverP_weightState = 3;
		return;
	}

	u8 weightState;
	if(player->input->toggleFaceButtons.hasAny(DPadLeft)) {
		weightState = 1;
		f32 calculatedWeight;
		switch(Player_Coverp_ChangeWeight(player, weightState)) {
			default:
				return;
			case 1:
				player->rings -= 10;
				calculatedWeight = originalWeight - (originalWeight * 0.50f);
				if(calculatedWeight < 0.1f) { calculatedWeight = 0.1f; }
				player->weight = calculatedWeight;
				// weightState = 1;
				break;
			case 2:
				return;
		}
		player->coverP_weightState = weightState;
		Player_CreateCoverPParticles(player);
		PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);
	} else if(player->input->toggleFaceButtons.hasAny(DPadUp)) {
		weightState = 3;
		switch(Player_Coverp_ChangeWeight(player, weightState)) {
			default:
				return;
			case 1:
                player->rings -= 10;
				player->weight = originalWeight;
				// weightState = 3;
				break;
			case 2:
				return;
		}
		player->coverP_weightState = weightState;
		Player_CreateCoverPParticles(player);
		PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);

	} else if(player->input->toggleFaceButtons.hasAny(DPadRight)) {
		weightState = 2;
		f32 calculatedWeight;
		switch(Player_Coverp_ChangeWeight(player, weightState)) {
			case 2:
			default:
				return;
			case 1:
                player->rings -= 10;
                calculatedWeight = originalWeight + (originalWeight * 0.50f);
                if (calculatedWeight > 2.92500f) calculatedWeight = 2.92500f;
				player->weight = calculatedWeight;
				// weightState = 2;
				break;
		}
		player->coverP_weightState = weightState;
		Player_CreateCoverPParticles(player);
		PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);
	}

	//const f32 acceleration = static_cast<f32>(player->rings) / 5.0F;
	//if(player->characterArchetype != BoostArchetype) {
	//	player->gearStats[player->level].baseAccel = acceleration;
	//} else if(player->characterArchetype == BoostArchetype) {
	//	if((player->movementFlags & boosting) != 0U) {
	//		player->gearStats[player->level].baseAccel = player->gearptr->acceleration;
	//	} else {
	//		player->gearStats[player->level].baseAccel = acceleration;
	//	}
	//}
	//f32 boostSpeed = pSpeed(185.0F) + pSpeed(static_cast<f32>(player->rings) * 0.3F);
	//if(player->characterArchetype == BoostArchetype) {
	//	boostSpeed += BoostArchetypeBoostSpeeds[player->level];
	//}
	//player->gearStats[player->level].boostSpeed = boostSpeed;

	//if((player->movementFlags & boosting) != 0u) {
	//	player->gearStats[player->level].topSpeed = boostSpeed;
	//} else if(player->state == Cruise || player->state == Fly) {
	//	const f32 topSpeed = player->gearStats[0].topSpeed + pSpeed(static_cast<f32>(player->rings) * 0.234F);
	//	player->gearStats[player->level].topSpeed = topSpeed;
	//	player->speedCap = topSpeed;
	//}
}