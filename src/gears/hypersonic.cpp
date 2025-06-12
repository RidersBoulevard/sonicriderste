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

void Player_CreateHyperSonicParticles(Player &player) {
	auto &particles = *static_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, Object1Sizes::x80)->object);
	particles.unk72 = player.index;
	particles.x = 0.0f;
	particles.y = 0.3f;
	particles.z = 0.0f;
	particles.unk10 = 0.0f;
	particles.unk18 = 0.0f;
	particles.unk14 = 0.8f;
	particles.texList = gpsTexList_Particle;
	particles.particleParams = &lbl_001D8F58;
	particles.unk73 = -1;
	particles.baseModelMatrix = nullptr;
	particles.unk74 = 0;
	particles.unk48 = &player.x;
}

void Player_HyperdriveHandler(Player &player) {
	if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != StoryMode
	   && CurrentGameMode != TimeTrial) { return; }
	if(player.character != Character::SuperSonic) { return; }
	if(player.gearExload().exLoadID != EXLoad::HyperSonic) { return; }
	BlastGaugeInfo &bgInfo = PlayerBlastGaugeInfo[player.index];
	HyperSonicInfo &hsInfo = PlayerHyperSonicInfo[player.index];
	if(bgInfo.currentGauge >= 200000) {
		player.currentAir = clamp(player.currentAir, 500);
		if(player.state == PlayerState::Cruise) { player.speedCap = pSpeed(215.0f); }
		if(hsInfo.hyperdriveEnabled || player.state == PlayerState::AttackedByPlayer) { return; }
		hsInfo.hyperdriveEnabled = true;
		Player_CreateHyperSonicParticles(player);
		PlayAudioFromDAT(Sound::SFX::HyperDrive);

		const auto selectedLine = lbl_RNG_Number(3) + 13;
		Sound::PlaySound(player, selectedLine);
	} else if(bgInfo.currentGauge < 0) {
		bgInfo.currentGauge = 0;
		hsInfo.hyperdriveEnabled = false;
	}
}

void Player_HyperdriveHandler2(Player *player) {
	if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != StoryMode
	   && CurrentGameMode != TimeTrial) { return; }
	if(player->character != Character::SuperSonic) { return; }
	if(player->gearExload().exLoadID != EXLoad::HyperSonic) { return; }
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];

	if(bgInfo->currentGauge >= 200000) {
		// Change to air gear
		if (!player->isBlastGaugeGear) { return; }
		player->specialFlags &= ~SpecialFlags::ringGear;
		player->specialFlags &= ~SpecialFlags::fiftyPercentAir;
		// set air to maximum equal to meter amount
		player->gearStats[player->level].maxAir = 200000;
		player->currentAir = 200000;
		bgInfo->currentGauge = 0;
		player->speed += pSpeed(100.0f - static_cast<f32>(player->rings)/2.0f);
		player->isBlastGaugeGear = false;
		// player->currentAir = clamp(player->currentAir, 500);
		if(player->state == PlayerState::Cruise) { player->speedCap = pSpeed(220.0f); }
		if(hsInfo->hyperdriveEnabled || player->state == PlayerState::AttackedByPlayer) { return; }
		hsInfo->hyperdriveEnabled = true;
		Player_CreateHyperSonicParticles(*player);
		PlayAudioFromDAT(Sound::SFX::HyperDrive);

		const auto selectedLine = lbl_RNG_Number(3) + 13;
		Sound::PlaySound(*player, selectedLine);
	}
	// Note: is this necessary now?
	// else if(bgInfo->currentGauge < 0) {
	// 	bgInfo->currentGauge = 0;
	// 	hsInfo->hyperdriveEnabled = false;
	// }

}

void Player_HyperSonicUpdatePlayerStats(Player &player) {
	if(player.character != Character::SuperSonic) { return; }

	if (player.extremeGear == ExtremeGear::ChaosEmerald) {
		SNDF_SeqIdStop(player.index + 50);
		SNDF_SeqIdStop(player.index + 46);
	}

	if(player.gearExload().exLoadID != EXLoad::HyperSonic) { return; }

	if(player.state == PlayerState::StartLine) {
		if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != StoryMode
		   && CurrentGameMode != TimeTrial) { return; }
		player.specialFlags |= SpecialFlags::ringGear;
		player.currentAir = 55000;
		player.specialFlags &= ~SpecialFlags::thirtyPercentAir;
		player.specialFlags |= SpecialFlags::fiftyPercentAir;
		player.characterArchetype = CharacterArchetype::Boost;
	}
	BlastGaugeInfo &bgInfo = PlayerBlastGaugeInfo[player.index];
	HyperSonicInfo &hsInfo = PlayerHyperSonicInfo[player.index];

	if(!player.aiControl &&
	   player.input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X) &&
	   !player.movementFlags.hasAny(MovementFlags::boosting) &&
	   player.state == PlayerState::Cruise &&
	   player.currentAir >= player.gearStats[player.level].boostCost &&
	   player.unkB90 <= 0 &&
	   player.unkB08 & 0x2400) {
		PlayAudioFromDAT(Sound::SFX::HyperBoost);
	}

	if(hsInfo.hyperdriveEnabled) {
		player.characterArchetype = CharacterArchetype::SuperSonic;
		// sets air to maximum on start, so no longer needed
		// if(player.currentAir <= 500) {
		// 	player.currentAir = 500;
		// }
		// no longer uses blast gauge when transformed
		// if(player.input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X) && !player.movementFlags.hasAny(MovementFlags::boosting) && player.state == PlayerState::Cruise) {
		// 	bgInfo.currentGauge -= 5500;
		// }

		// if(player.state == PlayerState::Cruise || player.state == PlayerState::Jump) {// Only drain when in jump and cruise
		// 	bgInfo.currentGauge -= 523;                      // should take 9 seconds to drain, 370
		// }

		// player.gearStats[player.level].baseTopSpeed = pSpeed(215.0f);
		s32 ringToAirCap = static_cast<s32>(player.rings * 2000);
		player.gearStats[player.level].topSpeed = pSpeed(220.0f);
		// maximum air cap is determined by ring count
		player.gearStats[player.level].maxAir = clamp(ringToAirCap, 80000); // minimum meter of 40 rings.
		player.gearStats[player.level].airDrain = 0xC;
		player.gearStats[player.level].driftCost = 20;
		player.gearStats[player.level].boostCost = 50000; // 52500
		player.gearStats[player.level].tornadoCost = 12000;
		player.gearStats[player.level].boostSpeed = pSpeed(260.0f);
		if (hsInfo.hyperchain && !player.movementFlags.hasAny(MovementFlags::boosting)) hsInfo.hyperchain = false;
		if (player.state == PlayerState::Cruise && player.movementFlags.hasAny(MovementFlags::boosting) && player.input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X)
			&& player.rings >= hypersonicdef::HYPERCHAINCOST && !hsInfo.hyperchain && player.unkB90 <= 0 && player.unkB08 & 0x2400) {
			// Hyperchain cancel, 5 ring cost
			player.gearStats[player.level].boostCost = 0;
			const s32 newRings = static_cast<s32>(player.rings) - hypersonicdef::HYPERCHAINCOST;
			player.rings = static_cast<u32>(clamp(newRings, 0));
			lbl_Player_BoostEndFunction(player);
			hsInfo.hyperchain = true;
			// player->speed += pSpeed(15.0f);
		}

		u32 ringDrainRate = 50;
		s32 newRings = static_cast<s32>(player.rings);
		ringDrainRate -= (player.movementFlags.hasAny(MovementFlags::drifting)) ? 15 : 0; // increase drain rate while drifting
		ringDrainRate -= (player.movementFlags.hasAny(MovementFlags::boosting)) ? 20 : 0; // increase drain rate while boosting
		if(hsInfo.hyperdriveFrameCounter % ringDrainRate == 0 && player.state == PlayerState::Cruise
			&& !(player.input->holdFaceButtons.hasAny(Buttons::A) && player.jumpCharge > 0)) {
			newRings -= 1;
			player.rings = static_cast<u32>(clamp(newRings, 0, 100));
		}
		hsInfo.hyperdriveFrameCounter++;
		hsInfo.previousHyperdrive = true;
	} else {// Hyperdrive disabled
		player.characterArchetype = CharacterArchetype::Boost;
		if(player.movementFlags.hasAny(MovementFlags::boosting)) {// has no air drain while boosting
			player.gearStats[player.level].airDrain = 0;
		} else {
			player.gearStats[player.level].airDrain = 65;
		}
		// player.gearStats[player.level].baseTopSpeed = pSpeed(230.0f);
		player.gearStats[player.level].topSpeed = pSpeed(220.0f);
		player.gearStats[player.level].driftCost = 0x5;
		player.gearStats[player.level].boostCost = 22000;
		player.gearStats[player.level].tornadoCost = 5500;
		if(!player.movementFlags.hasAny(MovementFlags::boosting)) {
			player.gearStats[player.level].boostSpeed = pSpeed(280.0f);
		}
	}
}

void Player_HyperSonicTotalLinkCalc(Player &player) {
	if(player.character != Character::SuperSonic) { return; }

	if(player.gearExload().exLoadID != EXLoad::HyperSonic) { return; }

	auto &[totalObjectLinks, extraObjectLinks, hyperdriveEnabled, previousHyperdrive, _, _] = PlayerHyperSonicInfo[player.index];
	if(player.state == PlayerState::StartLine) {
		totalObjectLinks = 0;
		extraObjectLinks = 0;
		hyperdriveEnabled = false;
		previousHyperdrive = false;
	}
	// removed for new design
	// else if(player->state == PlayerState::AttackedByPlayer) {
	// 	if(hsInfo->hyperdriveEnabled) {
	// 		hsInfo->hyperdriveEnabled = false;
	// 	}
	// }

	if(player.hyperSonic_totalLinkTimer > 0) {
		if(totalObjectLinks < player.objectLinkCount) {
			totalObjectLinks = player.objectLinkCount;
		} else if(totalObjectLinks > player.objectLinkCount)//  && (player.objectLinkCount == 0)
		{
			extraObjectLinks += totalObjectLinks;
			totalObjectLinks = 0;
		}
	}

	if(player.hyperSonic_totalLinkTimer == 1) {
		Player_CreateHyperSonicParticles(player);
		PlayAudioFromDAT(Sound::SFX::HyperStream);
		// NOTE: hsInfo->previousHyperdrive
		if(hyperdriveEnabled) {
			player.rings += static_cast<u32>(static_cast<f32>(extraObjectLinks + totalObjectLinks) * 2.5f);// NOTE: removed 50% bonus
			if(totalObjectLinks == 0) {
				player.rings += 2;
			}
			player.currentAir += player.gearStats[player.level].maxAir * static_cast<s32>((extraObjectLinks + totalObjectLinks) * 10) / 100;
		} else {
			player.currentAir += player.gearStats[player.level].maxAir *
				static_cast<s32>((extraObjectLinks + totalObjectLinks) * 2
					) / 100;
			if(totalObjectLinks == 0) {
				player.currentAir += (player.gearStats[player.level].maxAir * 2) / 100;
			}
		}
		// hsInfo->previousHyperdrive = false; // removed for new design
		totalObjectLinks = 0;
		extraObjectLinks = 0;
	}
}

void Player_HyperSonicRingStream(Player &player) {
	if(player.character != Character::SuperSonic) { return; }

	if(player.gearExload().exLoadID != EXLoad::HyperSonic) { return; }

	if(player.state == PlayerState::RailGrind
	   || player.state == PlayerState::Fly
	   || player.unkD90 >= 60) {
		player.hyperSonic_totalLinkTimer = 181;
	}
}

void Player_SuperShadow(Player &player) {
	if (!player.isSuperCharacter(Character::Shadow)) return;
	BlastGaugeInfo &bgInfo = PlayerBlastGaugeInfo[player.index];
	// HyperSonicInfo &hsInfo = PlayerHyperSonicInfo[player.index];
	SuperShadowInfo &sshInfo = PlayerSuperShadowInfo[player.index];

	if (player.state == PlayerState::StartLine) {
		// hsInfo.hyperdriveEnabled = false;
		player.supershadowboostflag = false;
		player.gearStats[player.level].boostCost = 22000; // 20 ring boost cost
		sshInfo.inhibitorRingsOff = false;
		sshInfo.chaosBoostControl = 0;
		// player->gearStats[player->level].topSpeed = pSpeed(175.0f);
	}

	// Gauge passively fills
	// if(player->state == PlayerState::Cruise && !sshInfo->inhibitorRingsOff) bgInfo->currentGauge += 333.3; // should be 10% per second
	// Cruise
	s32 driftMeterBonus = (player.movementFlags.hasAny(MovementFlags::drifting)) ? 111.1 : 0;
	s32 jumpChargeBonus = (player.input->holdFaceButtons.hasAny(Buttons::A) && player.jumpCharge > 0) ? 277.77 : 0;
	if(player.state == PlayerState::Cruise && !player.movementFlags.hasAny(MovementFlags::boosting))
	{
		bgInfo.currentGauge += (static_cast<s32>(555.5) + driftMeterBonus + jumpChargeBonus);
	}

	// meter cap
	if (bgInfo.currentGauge >= 200000) {
		bgInfo.currentGauge = 200000;
	}

	f32 meterPercent = (static_cast<f32>(bgInfo.currentGauge) / 200000.0f);

	// Top speed increases as gauge increases
	f32 topSpeedBonus = pSpeed(180.0f) + pSpeed(50.0f * meterPercent); // 20 ring boost cost;
	player.gearStats[player.level].topSpeed = topSpeedBonus;
	if (player.state == PlayerState::Cruise && !player.movementFlags.hasAny(MovementFlags::boosting)) {
		player.speedCap = topSpeedBonus;
	}
	// ring cost of boost is offset by meter amount
	s32 boostCostReduction = static_cast<s32>(22000.0f - (22000.0f * meterPercent)); // 20 ring boost cost
	player.gearStats[player.level].boostCost = boostCostReduction;

	// boost speed reduces with more meter
	f32 boostSpeedReduction = pSpeed(280.4f) - pSpeed(20.0f * meterPercent); // 20 ring boost cost;
	if (!player.movementFlags.hasAny(MovementFlags::boosting)) {
		player.gearStats[player.level].boostSpeed = boostSpeedReduction;
	} else bgInfo.currentGauge = 0;


	// Boost removes all meter
    // See customboostduration
	// if(player.input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X)
	// 	&& !player.movementFlags.hasAny(MovementFlags::boosting) && player.state == PlayerState::Cruise
	// 	&& player.currentAir >= player.gearStats[player.level].boostCost
	// 	&& player.unkB90 <= 0
	// 	&& player.unkB08 & 0x2400)
	// {
	// 	bgInfo.currentGauge = 0;
	// }

	//

	/*
	if (player->state == PlayerState::StartLine) {
		// hsInfo->hyperdriveEnabled = false;
		player->supershadowboostflag = false;
		player->gearStats[player->level].boostCost = 22000; // 20 ring boost cost
		sshInfo->inhibitorRingsOff = false;
		sshInfo->chaosBoostControl = 0;
	}

	// Player actions gain meter, not tricks
	
	// If max gauge, give ring bonus ONCE and enable the custom boost
	if (bgInfo.currentGauge >= 200000) {
		bgInfo.currentGauge = 200000;
		if (!sshInfo.inhibitorRingsOff)
		{
			PlayAudioFromDAT(Sound::SFX::HyperStream);
			Player_CreateHyperSonicParticles(player);
			player.currentAir += 11000; // 10 ring refund
		}
		sshInfo.inhibitorRingsOff = true; // activates if not already active
	}

	// While not boost controlling, let them gain meter
	if (!sshInfo.inhibitorRingsOff) {
		
		// Boost
		if(player.input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X)
			&& !player.movementFlags.hasAny(MovementFlags::boosting) && player.state == PlayerState::Cruise
			&& player.currentAir >= player.gearStats[player.level].boostCost
			&& player.unkB90 <= 0)
		{
			// s32 newGauge;
			// if (sshInfo.inhibitorRingsOff) {
			// 	newGauge = bgInfo.currentGauge - player.gearStats[player.level].boostCost * 2.1;
			// 	if (newGauge <= 0) sshInfo.inhibitorRingsOff = false;
			// 	bgInfo.currentGauge = clamp(newGauge, 0);
			// }
			// else if (!sshInfo.inhibitorRingsOff)
			bgInfo.currentGauge += (player.gearStats[player.level].boostCost * 3.33);
		}

		// Drifting
		if(player.movementFlags.hasAny(MovementFlags::drifting) && player.state == PlayerState::Cruise)
		{bgInfo.currentGauge += (player.gearStats[player.level].driftCost * 3.33);}

		// Tornado
		// if(player.input.toggleFaceButtons.hasAll(LButton | RButton) && player.state == Cruise
		// 	&& player.currentAir >= player.gearStats[player.level].tornadoCost)
		// {bgInfo.currentGauge += (player.gearStats[player.level].tornadoCost * 3.33);}

		// Cruise
		if(player.state == PlayerState::Cruise && !sshInfo.inhibitorRingsOff)
		{bgInfo.currentGauge += (player.gearStats[player.level].airDrain * 2.4975);}

	}

	// If boost is enabled, increase boost speed to 300 and allow boost control until button is released
	if (sshInfo.inhibitorRingsOff) {
		// player.gearStats[player.level].boostCost = 0; // 15 ring boost cost
		// player.supershadowboostflag = true;

		// If player presses button, remove gauge and remove boost flag if they're not holding the button
		if(!player.movementFlags.hasAny(MovementFlags::boosting) && player.state == PlayerState::Cruise
		&& player.currentAir >= player.gearStats[player.level].boostCost
		&& player.unkB90 <= 0
		&& !player.input->toggleFaceButtons.hasAny(Buttons::A))
		{
			// player.gearStats[player.level].boostCost = 16500; // 15 ring boost cost
			// player.gearStats[player.level].boostSpeed = pSpeed(265.0f);
			if (player.input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X)) {
				player.gearStats[player.level].boostSpeed = pSpeed(300.0f);
				bgInfo.currentGauge = 0;
				sshInfo.chaosBoostControl = 1;
				player.supershadowboostflag = true;
			}
			// if (player.input->toggleFaceButtons.hasAny(Buttons::B)) {
			// 	player.specialFlags &= ~SpecialFlags::stickyFingers;
			// } else if (player.input->toggleFaceButtons.hasAny(Buttons::X)) {
			// 	player.specialFlags |= SpecialFlags::stickyFingers;
			// 	player.gearStats[player.level].boostCost = 22000; // 15 ring boost cost
			// 	player.gearStats[player.level].boostSpeed = pSpeed(290.0f);
			// 	sshInfo.chaosBoostControl = 1;
			// 	// bgInfo.currentGauge = 0;
			// 	// player.supershadowboostflag = true; // If they toggled the boost, set to true
			// }
		}

		if ((player.boostDuration == 0 || player.state != PlayerState::Cruise) && sshInfo.chaosBoostControl) {
			sshInfo.inhibitorRingsOff = false;
			sshInfo.chaosBoostControl = 2;
			// player.supershadowboostflag = false;
		} else sshInfo.inhibitorRingsOff = true;

		// // Boost duration should be 1 if they're boost controlling, 0 otherwise
		// if (player.boostDuration > 0 && player.supershadowboostflag)
		// {
		// 	player.supershadowboostflag = false;
		// 	hsInfo.hyperdriveEnabled = false;
		// }
		// else 
		// if ((player.boostDuration == 0 || player.state != PlayerState::Cruise) && player.supershadowboostflag)
		// {
		// 	hsInfo.hyperdriveEnabled = false;
		// 	player.supershadowboostflag = false;
		// }
		// else hsInfo.hyperdriveEnabled = true;
	} else {
		// player.gearStats[player.level].boostCost = 16500; // 15 ring boost cost
		// player.gearStats[player.level].boostCost = 22000; // 15 ring boost cost
		player.gearStats[player.level].boostSpeed = pSpeed(275.0f);
		player.supershadowboostflag = false;
		sshInfo.chaosBoostControl = 0;
	}
	*/

}