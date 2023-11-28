#include "hypersonic.hpp"
#include "cosmetics/player/exloads.hpp"
#include "gears/blastGaugeGears.hpp"
#include "handlers/player/voicelines.hpp"
#include "lib/sound.hpp"
#include "lib/stdlib.hpp"
#include "riders/gamemode.hpp"
#include "riders/object.hpp"
#include "supertails.hpp"

std::array<HyperSonicInfo, MaxPlayerCount> PlayerHyperSonicInfo;

void Player_CreateHyperSonicParticles(Player *player) {
	auto *particles = reinterpret_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, 2)->object);
	particles->unk72 = player->index;
	particles->x = 0.0f;
	particles->y = 0.3f;
	particles->z = 0.0f;
	particles->unk10 = 0.0f;
	particles->unk18 = 0.0f;
	particles->unk14 = 0.8f;
	particles->unk68 = gpsTexList_Particle;
	particles->unk60 = &lbl_001D8F58;
	particles->unk73 = -1;
	particles->unk6C = 0;
	particles->unk74 = 0;
	particles->unk48 = &player->x;
}

void Player_HyperdriveHandler(Player *player) {
	if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != StoryMode
	   && CurrentGameMode != TimeTrial) { return; }
	if(player->character != SuperSonic) { return; }
	if(FetchEnabledEXLoadIDs(*player).gearExLoadID != HyperSonicEXLoad) { return; }
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];
	if(bgInfo->currentGauge >= 200000) {
		player->currentAir = clamp(player->currentAir, 500);
		if(player->state == Cruise) player->speedCap = pSpeed(215.0f);
		if(hsInfo->hyperdriveEnabled || player->state == AttackedByPlayer) { return; }
		hsInfo->hyperdriveEnabled = true;
		Player_CreateHyperSonicParticles(player);
		PlayAudioFromDAT(Sound::SFX::HyperDrive);

		const auto selectedLine = lbl_RNG_Number(3) + 13;
		Sound::PlaySound(*player, selectedLine);
	} else if(bgInfo->currentGauge < 0) {
		bgInfo->currentGauge = 0;
		hsInfo->hyperdriveEnabled = false;
	}
}

void Player_HyperSonicUpdatePlayerStats(Player *player) {
	if(player->character != SuperSonic) return;

	if (player->extremeGear == ExtremeGear::ChaosEmerald) {
		SNDF_SeqIdStop(player->index + 50);
		SNDF_SeqIdStop(player->index + 46);
	}

	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
	if(exLoads.gearExLoadID != HyperSonicEXLoad) { return; }

	if(player->state == StartLine) {
		if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != StoryMode
		   && CurrentGameMode != TimeTrial) { return; }
		player->currentAir = 55000;
		player->specialFlags &= ~thirtyPercentAir;
		player->specialFlags |= fiftyPercentAir;
		player->characterArchetype = BoostArchetype;
	}
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];

	if(!player->aiControl &&
	   player->input->toggleFaceButtons.hasAny(BButton, XButton) &&
	   !player->movementFlags.hasAny(boosting) &&
	   player->state == Cruise &&
	   (player->rings >= 30 || hsInfo->hyperdriveEnabled) &&
	   player->unkB90 <= 0) {
		PlayAudioFromDAT(Sound::SFX::HyperBoost);
	}

	if(hsInfo->hyperdriveEnabled) {
		player->characterArchetype = SuperSonicArchetype;
		if(player->currentAir <= 500) {
			player->currentAir = 500;
		}
		if(player->input->toggleFaceButtons.hasAny(BButton, XButton) && !player->movementFlags.hasAny(boosting) && player->state == Cruise) {
			bgInfo->currentGauge -= 5500;
		}

		if(player->state == Cruise || player->state == Jump) {// Only drain when in jump and cruise
			bgInfo->currentGauge -= 370;                      // should take 9 seconds to drain
		}

		// player->gearStats[player->level].baseTopSpeed = pSpeed(215.0f);
		player->gearStats[player->level].topSpeed = pSpeed(215.0f);
		player->gearStats[player->level].airDrain = 0;
		player->gearStats[player->level].driftCost = 0;
		player->gearStats[player->level].boostCost = 0;
		player->gearStats[player->level].tornadoCost = 0;
		player->gearStats[player->level].boostSpeed = pSpeed(265.0f);
		hsInfo->previousHyperdrive = true;
	} else {// Hyperdrive disabled
		player->characterArchetype = BoostArchetype;
		if(player->movementFlags.hasAny(boosting)) {// has no air drain while boosting
			player->gearStats[player->level].airDrain = 0;
		} else {
			player->gearStats[player->level].airDrain = 65;
		}
		// player->gearStats[player->level].baseTopSpeed = pSpeed(230.0f);
		player->gearStats[player->level].topSpeed = pSpeed(230.0f);
		player->gearStats[player->level].driftCost = 0x5;
		player->gearStats[player->level].boostCost = 33000;
		player->gearStats[player->level].tornadoCost = 5500;
		player->gearStats[player->level].boostSpeed = pSpeed(300.0f);
	}
}

void Player_HyperSonicTotalLinkCalc(Player *player) {
	if(player->character != SuperSonic) { return; }

	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
	if(exLoads.gearExLoadID != HyperSonicEXLoad) { return; }

	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];
	if(player->state == StartLine) {
		hsInfo->totalObjectLinks = 0;
		hsInfo->extraObjectLinks = 0;
		hsInfo->hyperdriveEnabled = false;
		hsInfo->previousHyperdrive = false;
	} else if(player->state == AttackedByPlayer) {
		if(hsInfo->hyperdriveEnabled) {
			hsInfo->hyperdriveEnabled = false;
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
		if(hsInfo->previousHyperdrive) {
			player->currentAir += static_cast<s32>(player->gearStats[player->level].maxAir *
												   static_cast<s32>(
														   static_cast<f32>(hsInfo->extraObjectLinks + hsInfo->totalObjectLinks) * 2.5f)
												   )
								  / 100;// 50% extra
			if(hsInfo->totalObjectLinks == 0) {
				player->currentAir += (player->gearStats[player->level].maxAir * 4) / 100;
			}
		} else {
			player->currentAir += static_cast<s32>(player->gearStats[player->level].maxAir
												   * static_cast<s32>((hsInfo->extraObjectLinks + hsInfo->totalObjectLinks) * 2))
								  / 100;
			if(hsInfo->totalObjectLinks == 0) { player->currentAir += (player->gearStats[player->level].maxAir * 2) / 100; }
		}
		hsInfo->previousHyperdrive = false;
		hsInfo->totalObjectLinks = 0;
		hsInfo->extraObjectLinks = 0;
	}
}

void Player_HyperSonicRingStream(Player *player) {
	if(player->character != SuperSonic) { return; }
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if(exLoads.gearExLoadID != HyperSonicEXLoad) { return; }

	if(player->state == RailGrind
	   || player->state == Fly
	   || player->unkD90 >= 1) {
		player->hyperSonic_totalLinkTimer = 181;
	}
}

void Player_ZGSuperSonic(Player *player) {
	if(player->character != SuperSonic) { return; }

	if(FetchEnabledEXLoadIDs(*player).gearExLoadID != PerfectNazo) { return; }

	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];
	player->characterArchetype = Windcatcher;

	if(player->state == StartLine) {
		player->currentAir = 0;
	} else if(player->state == Run) {
		player->specialFlags |= legendEffect;
	} else if(player->state != Run) {
		player->specialFlags &= ~legendEffect;
	}

	player->level = 1;

	if(player->input->toggleFaceButtons.hasAny(DPadUp) &&
	   player->statusEffectFlags != 4 &&
	   player->currentAir >= 11000) {
		lbl_0008CC74(player->index);// spawn magnet
		PlayAudioFromDAT(Sound::SFX::HyperStream);
		player->currentAir -= 11000;
	}

	if(player->currentAir >= 66000) {
		if(!hsInfo->hyperdriveEnabled) {
			PlayAudioFromDAT(Sound::SFX::HyperDrive);
			player->statusEffectFlags = Flag<PlayerStatus>();
			Player_CreateHyperSonicParticles(player);
		}
		hsInfo->hyperdriveEnabled = true;
	} else if(player->currentAir <= 0) {
		hsInfo->hyperdriveEnabled = false;
	}

	if(hsInfo->hyperdriveEnabled) {
		player->specialFlags |= stickyFingers;
		player->gearStats[player->level].maxAir = 110000;
		if(player->state == Cruise) {
			player->speedCap = pSpeed(265.0f);
		}
		player->gearStats[player->level].baseTopSpeed = pSpeed(265.0f);
		player->gearStats[player->level].topSpeed = pSpeed(265.0f);

		if(player->statusEffectFlags == 4) {
			player->gearStats[player->level].airDrain = 200;
		} else {
			player->gearStats[player->level].airDrain = 168;
		}

		player->gearStats[player->level].driftCost = player->gearptr->levelStats[0].driftingAirCost;
		player->gearStats[player->level].boostCost = 11000;
		player->gearStats[player->level].tornadoCost = 5500;
		player->gearStats[player->level].boostSpeed = pSpeed(265.0f);
	} else {
		player->specialFlags &= ~stickyFingers;
		player->gearStats[player->level].maxAir = 110000;
		if(player->state == Cruise) { player->speedCap = pSpeed(235.0f); }
		player->gearStats[player->level].baseTopSpeed = pSpeed(235.0f);
		player->gearStats[player->level].topSpeed = pSpeed(235.0f);

		if(player->statusEffectFlags == 4) {
			player->gearStats[player->level].airDrain = 0x0;
		} else {
			player->gearStats[player->level].airDrain = 0x12;
		}

		player->gearStats[player->level].driftCost = player->gearptr->levelStats[0].driftingAirCost;
		player->gearStats[player->level].boostCost = 11000;
		player->gearStats[player->level].tornadoCost = 5500;
		player->gearStats[player->level].boostSpeed = pSpeed(245.0f);
	}
}

void Player_Excalibur(Player *player) {
	if(player->character != SuperSonic) { return; }
	if(FetchEnabledEXLoadIDs(*player).gearExLoadID != DarkSonicEXLoad) { return; }

	player->specialFlags |= stickyFingers;

	// player->characterArchetype = CombatArchetype;
	// player->level = 2;

	if(CurrentGameMode != FreeRace &&
	   CurrentGameMode != WorldGrandPrix &&
	   CurrentGameMode != StoryMode &&
	   CurrentGameMode != TimeTrial) { return; }
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];
	if(bgInfo->currentGauge >= 200000) {
		bgInfo->currentGauge = 200000;
		if(player->currentAir <= 500) { player->currentAir = 500; }
		if(player->input->toggleFaceButtons.hasAny(BButton, XButton) &&
		   !player->movementFlags.hasAny(boosting) &&
		   player->state == Cruise) {
			if(hsInfo->hyperdriveEnabled != TRUE) {
				hsInfo->hyperdriveEnabled = TRUE;
				Player_CreateHyperSonicParticles(player);
				PlayAudioFromDAT(Sound::SFX::HyperDrive);
			}
		}
		// if(player->state == Cruise) player->speedCap = pSpeed(215.0f);
		// TODO: activate custom sound line here
	} else if(bgInfo->currentGauge < 0) {
		bgInfo->currentGauge = 0;
		hsInfo->hyperdriveEnabled = FALSE;
	}

	if(player->state == StartLine) {
		// if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != StoryMode && CurrentGameMode != TimeTrial) { return; } // Condition already checked
		player->currentAir = 55000;
		player->specialFlags &= ~thirtyPercentAir;
		player->specialFlags |= fiftyPercentAir;
	}

	if(!player->aiControl &&
	   player->input->toggleFaceButtons.hasAny(BButton, XButton) &&
	   !player->movementFlags.hasAny(boosting) &&
	   player->state == Cruise &&
	   (player->rings >= 30 || hsInfo->hyperdriveEnabled == TRUE)) {
		PlayAudioFromDAT(Sound::SFX::HyperBoost);
	}

	if(hsInfo->hyperdriveEnabled) {
		if(player->currentAir <= 500) player->currentAir = 500;
		// if ((player->input->toggleFaceButtons & (BButton | XButton) && !(player->movementFlags & boosting) &&
		// player->state == Cruise)) bgInfo->currentGauge -= 5500;
		if(player->state != Jump && player->state != Cruise) {// if not in jump and cruise, drain none
			bgInfo->currentGauge -= 0;
		} else {
			bgInfo->currentGauge -= 370;
		}// should take 9 seconds to drain
		if(player->movementFlags.hasAny(boosting)) {
			player->specialFlags |= berserkerEffect;
		} else
			player->specialFlags &= ~berserkerEffect;
		player->gearStats[player->level].boostCost = 16500;
		player->gearStats[player->level].tornadoCost = 5500;
	} else {
		player->gearStats[player->level].airDrain = 0;
		// player->gearStats[player->level].baseTopSpeed = pSpeed(200.0f);
		// player->gearStats[player->level].topSpeed = player->gearStats[0].topSpeed;
		// if (player->state == Cruise) player->speedCap = player->gearStats[0].topSpeed;
		player->level = 0;
		player->gearStats[player->level].driftCost = player->gearptr->levelStats[0].driftingAirCost * 3;
		player->gearStats[player->level].boostCost = 22000;
		player->gearStats[player->level].tornadoCost = 11000;
		player->gearStats[player->level].boostSpeed = pSpeed(265.0f);
		player->specialFlags &= ~berserkerEffect;
		// player->specialFlags &= ~stickyFingers;
	}
}