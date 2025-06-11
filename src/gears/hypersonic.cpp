#include "hypersonic.hpp"
#include "cosmetics/player/exloads.hpp"
#include "gears/blastGaugeGears.hpp"
#include "handlers/player/voicelines.hpp"
#include "lib/sound.hpp"
#include "lib/stdlib.hpp"
#include "riders/gamemode.hpp"
#include "riders/object.hpp"
#include "supertails.hpp"
#include "handlers/player/specialflagtweaks.hpp"
#include "gears/supers.hpp"

std::array<HyperSonicInfo, MaxPlayerCount> PlayerHyperSonicInfo;
std::array<SuperShadowInfo, MaxPlayerCount> PlayerSuperShadowInfo;

void Player_CreateHyperSonicParticles(Player *player) {
	auto *particles = reinterpret_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, 2)->object);
	particles->unk72 = player->index;
	particles->x = 0.0f;
	particles->y = 0.3f;
	particles->z = 0.0f;
	particles->unk10 = 0.0f;
	particles->unk18 = 0.0f;
	particles->unk14 = 0.8f;
	particles->texList = gpsTexList_Particle;
	particles->particleParams = &lbl_001D8F58;
	particles->unk73 = -1;
	particles->baseModelMatrix = nullptr;
	particles->unk74 = 0;
	particles->unk48 = &player->x;
}

void Player_HyperdriveHandler(Player *player) {
	if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != StoryMode
	   && CurrentGameMode != TimeTrial) { return; }
	if(player->character != Character::SuperSonic) { return; }
	if(player->gearExload().exLoadID != EXLoad::HyperSonic) { return; }
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];
	if(bgInfo->currentGauge >= 200000) {
		player->currentAir = clamp(player->currentAir, 500);
		if(player->state == PlayerState::Cruise) { player->speedCap = pSpeed(215.0f); }
		if(hsInfo->hyperdriveEnabled || player->state == PlayerState::AttackedByPlayer) { return; }
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
	if(player->character != Character::SuperSonic) { return; }

	if (player->extremeGear == ExtremeGear::ChaosEmerald) {
		SNDF_SeqIdStop(player->index + 50);
		SNDF_SeqIdStop(player->index + 46);
	}

	if(player->gearExload().exLoadID != EXLoad::HyperSonic) { return; }

	if(player->state == PlayerState::StartLine) {
		if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != StoryMode
		   && CurrentGameMode != TimeTrial) { return; }
		player->currentAir = 55000;
		player->specialFlags &= ~SpecialFlags::thirtyPercentAir;
		player->specialFlags |= SpecialFlags::fiftyPercentAir;
		player->characterArchetype = CharacterArchetype::Boost;
	}
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];

	if(!player->aiControl &&
	   player->input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X) &&
	   !player->movementFlags.hasAny(MovementFlags::boosting) &&
	   player->state == PlayerState::Cruise &&
	   (player->rings >= 30 || hsInfo->hyperdriveEnabled) &&
	   player->unkB90 <= 0) {
		PlayAudioFromDAT(Sound::SFX::HyperBoost);
	}

	if(hsInfo->hyperdriveEnabled) {
		player->characterArchetype = CharacterArchetype::SuperSonic;
		if(player->currentAir <= 500) {
			player->currentAir = 500;
		}
		if(player->input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X) && !player->movementFlags.hasAny(MovementFlags::boosting) && player->state == PlayerState::Cruise) {
			bgInfo->currentGauge -= 5500;
		}

		if(player->state == PlayerState::Cruise || player->state == PlayerState::Jump) {// Only drain when in jump and cruise
			bgInfo->currentGauge -= 523;                      // should take 9 seconds to drain, 370
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
		player->characterArchetype = CharacterArchetype::Boost;
		if(player->movementFlags.hasAny(MovementFlags::boosting)) {// has no air drain while boosting
			player->gearStats[player->level].airDrain = 0;
		} else {
			player->gearStats[player->level].airDrain = 65;
		}
		// player->gearStats[player->level].baseTopSpeed = pSpeed(230.0f);
		player->gearStats[player->level].topSpeed = pSpeed(230.0f);
		player->gearStats[player->level].driftCost = 0x5;
		player->gearStats[player->level].boostCost = 33000;
		player->gearStats[player->level].tornadoCost = 5500;
		if(!player->movementFlags.hasAny(MovementFlags::boosting)) {
			player->gearStats[player->level].boostSpeed = pSpeed(295.0f);
		}
	}
}

void Player_HyperSonicTotalLinkCalc(Player *player) {
	if(player->character != Character::SuperSonic) { return; }

	if(player->gearExload().exLoadID != EXLoad::HyperSonic) { return; }

	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];
	if(player->state == PlayerState::StartLine) {
		hsInfo->totalObjectLinks = 0;
		hsInfo->extraObjectLinks = 0;
		hsInfo->hyperdriveEnabled = false;
		hsInfo->previousHyperdrive = false;
	} else if(player->state == PlayerState::AttackedByPlayer) {
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
			player->currentAir += player->gearStats[player->level].maxAir *
				static_cast<s32>(
					static_cast<f32>(hsInfo->extraObjectLinks + hsInfo->totalObjectLinks) * 2.5f
					) / 100;// 50% extra
			if(hsInfo->totalObjectLinks == 0) {
				player->currentAir += (player->gearStats[player->level].maxAir * 4) / 100;
			}
		} else {
			player->currentAir += player->gearStats[player->level].maxAir *
				static_cast<s32>((hsInfo->extraObjectLinks + hsInfo->totalObjectLinks) * 2
					) / 100;
			if(hsInfo->totalObjectLinks == 0) {
				player->currentAir += (player->gearStats[player->level].maxAir * 2) / 100;
			}
		}
		hsInfo->previousHyperdrive = false;
		hsInfo->totalObjectLinks = 0;
		hsInfo->extraObjectLinks = 0;
	}
}

void Player_HyperSonicRingStream(Player *player) {
	if(player->character != Character::SuperSonic) { return; }

	if(player->gearExload().exLoadID != EXLoad::HyperSonic) { return; }

	if(player->state == PlayerState::RailGrind
	   || player->state == PlayerState::Fly
	   || player->unkD90 >= 90) {
		player->hyperSonic_totalLinkTimer = 181;
	}
}

void Player_ZGSuperSonic(Player *player) {
	if(player->character != Character::SuperSonic) { return; }

	if(player->gearExload().exLoadID != EXLoad::PerfectNazo) { return; }

	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];
	player->characterArchetype = CharacterArchetype::Windcatcher;

	if(player->state == PlayerState::StartLine) {
		player->currentAir = 0;
	} else if(player->state == PlayerState::Run) {
		player->specialFlags |= SpecialFlags::legendEffect;
	} else if(player->state != PlayerState::Run) {
		player->specialFlags &= ~SpecialFlags::legendEffect;
	}

	player->level = 1;

	if(player->input->toggleFaceButtons.hasAny(Buttons::DPadUp) &&
	   player->statusEffectFlags != PlayerStatus::MagnetStatus &&
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
		player->specialFlags |= SpecialFlags::stickyFingers;
		player->gearStats[player->level].maxAir = 110000;
		if(player->state == PlayerState::Cruise) {
			player->speedCap = pSpeed(265.0f);
		}
		player->gearStats[player->level].baseTopSpeed = pSpeed(265.0f);
		player->gearStats[player->level].topSpeed = pSpeed(265.0f);

		if(player->statusEffectFlags == PlayerStatus::MagnetStatus) {
			player->gearStats[player->level].airDrain = 200;
		} else {
			player->gearStats[player->level].airDrain = 168;
		}

		player->gearStats[player->level].driftCost = player->gearptr->levelStats[0].driftingAirCost;
		player->gearStats[player->level].boostCost = 11000;
		player->gearStats[player->level].tornadoCost = 5500;
		player->gearStats[player->level].boostSpeed = pSpeed(265.0f);
	} else {
		player->specialFlags &= ~SpecialFlags::stickyFingers;
		player->gearStats[player->level].maxAir = 110000;
		if(player->state == PlayerState::Cruise) { player->speedCap = pSpeed(235.0f); }
		player->gearStats[player->level].baseTopSpeed = pSpeed(235.0f);
		player->gearStats[player->level].topSpeed = pSpeed(235.0f);

		if(player->statusEffectFlags == PlayerStatus::MagnetStatus) {
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

void Player_SuperShadow(Player *player) {
	if (!isSuperCharacter(*player, Character::Shadow)) return;
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
	// HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];
	SuperShadowInfo *sshInfo = &PlayerSuperShadowInfo[player->index];

	if (player->state == PlayerState::StartLine) {
		// hsInfo->hyperdriveEnabled = false;
		player->supershadowboostflag = false;
		player->gearStats[player->level].boostCost = 22000; // 20 ring boost cost
		sshInfo->inhibitorRingsOff = false;
		sshInfo->chaosBoostControl = 0;
	}

	// Player actions gain meter, not tricks
	
	// If max gauge, give ring bonus ONCE and enable the custom boost
	if (bgInfo->currentGauge >= 200000) {
		bgInfo->currentGauge = 200000;
		if (!sshInfo->inhibitorRingsOff) 
		{
			PlayAudioFromDAT(Sound::SFX::HyperStream);
			Player_CreateHyperSonicParticles(player);
			player->currentAir += 11000; // 10 ring refund
		}
		sshInfo->inhibitorRingsOff = true; // activates if not already active
	}

	// While not boost controlling, let them gain meter
	if (!sshInfo->inhibitorRingsOff) {
		
		// Boost
		if(player->input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X) 
			&& !player->movementFlags.hasAny(MovementFlags::boosting) && player->state == PlayerState::Cruise
			&& player->currentAir >= player->gearStats[player->level].boostCost
			&& player->unkB90 <= 0) 
		{
			// s32 newGauge;
			// if (sshInfo->inhibitorRingsOff) {
			// 	newGauge = bgInfo->currentGauge - player->gearStats[player->level].boostCost * 2.1;
			// 	if (newGauge <= 0) sshInfo->inhibitorRingsOff = false; 
			// 	bgInfo->currentGauge = clamp(newGauge, 0);
			// }
			// else if (!sshInfo->inhibitorRingsOff) 
			bgInfo->currentGauge += (player->gearStats[player->level].boostCost * 3.33);
		}

		// Drifting
		if(player->movementFlags.hasAny(MovementFlags::drifting) && player->state == PlayerState::Cruise) 
		{bgInfo->currentGauge += (player->gearStats[player->level].driftCost * 3.33);}

		// Tornado
		// if(player->input->toggleFaceButtons.hasAll(LButton | RButton) && player->state == Cruise
		// 	&& player->currentAir >= player->gearStats[player->level].tornadoCost) 
		// {bgInfo->currentGauge += (player->gearStats[player->level].tornadoCost * 3.33);}

		// Cruise
		if(player->state == PlayerState::Cruise && !sshInfo->inhibitorRingsOff) 
		{bgInfo->currentGauge += (player->gearStats[player->level].airDrain * 2.4975);}

	}

	// If boost is enabled, increase boost speed to 300 and allow boost control until button is released
	if (sshInfo->inhibitorRingsOff) {
		// player->gearStats[player->level].boostCost = 0; // 15 ring boost cost
		// player->supershadowboostflag = true;

		// If player presses button, remove gauge and remove boost flag if they're not holding the button
		if(!player->movementFlags.hasAny(MovementFlags::boosting) && player->state == PlayerState::Cruise
		&& player->currentAir >= player->gearStats[player->level].boostCost
		&& player->unkB90 <= 0
		&& !player->input->toggleFaceButtons.hasAny(Buttons::A)) 
		{
			// player->gearStats[player->level].boostCost = 16500; // 15 ring boost cost
			// player->gearStats[player->level].boostSpeed = pSpeed(265.0f);
			if (player->input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X)) { 
				player->gearStats[player->level].boostSpeed = pSpeed(300.0f);
				bgInfo->currentGauge = 0;
				sshInfo->chaosBoostControl = 1;
				player->supershadowboostflag = true;
			}
			// if (player->input->toggleFaceButtons.hasAny(Buttons::B)) {
			// 	player->specialFlags &= ~SpecialFlags::stickyFingers;
			// } else if (player->input->toggleFaceButtons.hasAny(Buttons::X)) {
			// 	player->specialFlags |= SpecialFlags::stickyFingers;
			// 	player->gearStats[player->level].boostCost = 22000; // 15 ring boost cost
			// 	player->gearStats[player->level].boostSpeed = pSpeed(290.0f);
			// 	sshInfo->chaosBoostControl = 1;
			// 	// bgInfo->currentGauge = 0;
			// 	// player->supershadowboostflag = true; // If they toggled the boost, set to true
			// }
		}

		if ((player->boostDuration == 0 || player->state != PlayerState::Cruise) && sshInfo->chaosBoostControl) {
			sshInfo->inhibitorRingsOff = false;
			sshInfo->chaosBoostControl = 2;
			// player->supershadowboostflag = false;
		} else sshInfo->inhibitorRingsOff = true;

		// // Boost duration should be 1 if they're boost controlling, 0 otherwise
		// if (player->boostDuration > 0 && player->supershadowboostflag)
		// {
		// 	player->supershadowboostflag = false;
		// 	hsInfo->hyperdriveEnabled = false;
		// }
		// else 
		// if ((player->boostDuration == 0 || player->state != PlayerState::Cruise) && player->supershadowboostflag) 
		// {
		// 	hsInfo->hyperdriveEnabled = false;
		// 	player->supershadowboostflag = false;
		// }
		// else hsInfo->hyperdriveEnabled = true;
	} else {
		// player->gearStats[player->level].boostCost = 16500; // 15 ring boost cost
		// player->gearStats[player->level].boostCost = 22000; // 15 ring boost cost
		player->gearStats[player->level].boostSpeed = pSpeed(275.0f);
		player->supershadowboostflag = false;
		sshInfo->chaosBoostControl = 0;
	}

}