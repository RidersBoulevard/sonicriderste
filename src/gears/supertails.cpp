#include "supertails.hpp"
#include "lib/sound.hpp"
#include "cosmetics/player/exloads.hpp"
#include "gears/blastGaugeGears.hpp"
#include "lib/stdlib.hpp"

std::array<SuperTailsInfo, MaxPlayerCount> PlayerSuperTailsInfo;

constexpr GearLevelStats SuperTails_SuperStats = {
		110000, // max air 220000
		0x16, // air drain 0x16
		0xA, // drift cost 0xA
		0x55F0, // boost cost 0x6B6C 0x55F0
		0x2260, // tornado cost
		pSpeed(100), // drift dash speed
		pSpeed(270) // boost speed
};

constexpr GearLevelStats SuperTails_NonSuperStats = {
		110000, // max air
		0x10, // air drain
		0xA6, // drift cost
		0x4E20, // boost cost 0x61A8
		0x4E20, // tornado cost 0x61A8
		pSpeed(100), // drift dash speed
		pSpeed(200) // boost speed
};

ASMUsed void SuperTails_SpawnMagnetAtStart(Player *player){
	if(player->isSuperCharacter(Character::Tails)){
		SuperTailsInfo *stInfo = &PlayerSuperTailsInfo[player->index];
		lbl_0008CC74(player->index); // spawn magnet
		stInfo->playerMagnet = true; // Use this for magnet handler
		player->superFormState = 0;
		player->typeAttributes &= ~Type::Power;
	}
}

ASMUsed void SuperTails_SpawnMagnetAfterDeath(Player *player){
	// if(player->isSuperCharacter(Character::Tails) && player->superFormState == 0){
	// 	SuperTailsInfo *stInfo = &PlayerSuperTailsInfo[player->index];
	// 	lbl_0008CC74(player->index); // spawn magnet
	// 	stInfo->playerMagnet = true; // Use this for magnet handler
	// }

	if(player->extremeGear == ExtremeGear::CoverF
	   || player->extremeGear == ExtremeGear::CoverS
	   || player->extremeGear == ExtremeGear::CoverP
	   || player->extremeGear == ExtremeGear::GunGear){ // temporary, move to different function later
		player->level = 1;
		player->currentAir = player->gearStats[player->level].maxAir;
	}

	player->reciproExtendTimer = 0; // grant recipro extend

}

inline void SuperTails_UpdatePlayerStats(Player &player, const GearLevelStats *stats, f32 topSpeed){
	player.gearStats[0].updateGearLevelStats(*stats);
	player.gearStats[0].topSpeed = topSpeed;
	if(player.state == PlayerState::Cruise) {
		player.speedCap = topSpeed;
	}
}

void Player_SuperTailsTransformation(Player &player){
	if(player.playerType) { return; }
	if(!player.isSuperCharacter(Character::Tails)) { return; }
	SuperTailsInfo *stInfo = &PlayerSuperTailsInfo[player.index];
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player.index];
	if(player.flags.hasAny(PlayerFlags::InAPit)) { return; }
	const auto transformationCost = 5;
	const f32 tailwindSpeed = pSpeed(240.0f);
	const u32 transformTime = 1;

	// Handle timer in super state with gauge
	if (player.superFormState >= 1 && player.state == PlayerState::Cruise) {
		// Timer goes up when under maximum
		if (bgInfo->currentGauge <= 200000) {
			bgInfo->currentGauge += 305;
		} else {
			// Cap gauge
			bgInfo->currentGauge = 200000;
		}
	    // See customboostduration
		// if (player.input->toggleFaceButtons.hasAny(Buttons::B)
		// && !player.movementFlags.hasAny(MovementFlags::boosting)
		// && player.unkB90 <= 0 && player.currentAir >= SuperTails_SuperStats.boostCost
		// && player.unkB08 & 0x2400) {
		// 	// Take away timer if he boosts
		// 	s32 newGauge = bgInfo->currentGauge - SuperTails_SuperStats.boostCost * 1.2;
		// 	bgInfo->currentGauge = clamp(newGauge);
		// }
	}

	if (player.superFormState == 0 && player.state == PlayerState::Cruise) {
		// Tailwind active
		if (stInfo->tailwindTimer > 0) {
			stInfo->tailwindTimer -= 195;
			player.fastest_superCruise = true;
			const f32 newSpeed = player.speed + 0.0027006172839506F;// per frame
			if(newSpeed < tailwindSpeed) { player.speed = newSpeed; }
			player.speedCap = tailwindSpeed;
			player.specialFlags |= SpecialFlags::berserkerEffect; 
			player.gearStats[player.level].boostSpeed = pSpeed(245.0f);
		} else {
			stInfo->tailwindTimer = 0;
			player.fastest_superCruise = false;
			player.speedCap = pSpeed(200);
			if (player.specialFlags.hasAny(SpecialFlags::berserkerEffect) 
			&& !player.movementFlags.hasAny(MovementFlags::boosting) && player.state != PlayerState::AttackingPlayer) 
			{player.attackProperties = nullptr;}
			player.specialFlags &= ~SpecialFlags::berserkerEffect; 
			player.gearStats[player.level].boostSpeed = pSpeed(200.0f);
		}
	}

	if(player.superTails_transformCooldown > 0) { return; }

	if (player.superFormState == 0 && (player.rings >= supertailsdef::MAGNETCOST && player.rings < supertailsdef::REQUIREDTRANSFORMRINGS) && player.state != PlayerState::Run) {
		// When in base state, let them toggle magnet if below the amount of rings needed for super form
		if (player.input->toggleFaceButtons.hasAny(Buttons::X) && !player.statusEffectFlags.hasAny(PlayerStatus::MagnetStatus)) {
			lbl_0008CC74(player.index); // spawn magnet
			player.typeAttributes &= ~Type::Speed;
			stInfo->playerMagnet = true; // Use this for magnet handler
			player.rings -= supertailsdef::MAGNETCOST;
			stInfo->tailwindTimer = 0;
			player.fastest_superCruise = false;
			if (player.specialFlags.hasAny(SpecialFlags::berserkerEffect) 
			&& !player.movementFlags.hasAny(MovementFlags::boosting)
			&& player.state != PlayerState::AttackingPlayer
			&& player.state != PlayerState::AttackedByPlayer)
			{player.attackProperties = nullptr;}
			player.specialFlags &= ~SpecialFlags::berserkerEffect; 
			player.gearStats[player.level].boostSpeed = pSpeed(200.0f);
			return;
		}
	}

	if(player.superFormState == 2 || player.rings >= supertailsdef::REQUIREDTRANSFORMRINGS){
		if(player.input->toggleFaceButtons.hasAny(Buttons::X)){
			if(player.superFormState == 0){
				// transform
				if(player.rings < transformationCost) { return; }
				if (stInfo->tailwindTimer > 0) {
					stInfo->tailwindTimer = 0;
					player.fastest_superCruise = false;
					if (player.specialFlags.hasAny(SpecialFlags::berserkerEffect) 
					&& !player.movementFlags.hasAny(MovementFlags::boosting) && player.state != PlayerState::AttackingPlayer) 
					{player.attackProperties = nullptr;}
					player.specialFlags &= ~SpecialFlags::berserkerEffect; 
					// player.gearStats[player.level].boostSpeed = pSpeed(200.0f);
				}

				player.superFormState = 1;
				player.superTails_transformCooldown = transformTime;
				stInfo->transformCheck = 1;
				bgInfo->currentGauge = player.currentAir; // Store this for his timer
				player.rings -= transformationCost; // transform cost
				player.currentAir = static_cast<s32>(player.rings * 1100);
				player.specialFlags ^= SpecialFlags::iceImmunity
						| SpecialFlags::noSpeedLossChargingJump
						| SpecialFlags::ringGear
						| SpecialFlags::tornadoBoost
						| SpecialFlags::fiftyPercentAir;

				SuperTails_UpdatePlayerStats(player, &SuperTails_SuperStats, pSpeed(220));

				// stop the beeping sound when low on air
				SNDF_SeqIdStop(player.index + 50);
				SNDF_SeqIdStop(player.index + 46);

				PlayAudioFromDAT(Sound::SFX::SuperTransformation); // super transformation sfx

				player.typeAttributes |= Type::Speed;
				if(player.movementFlags.hasAny(MovementFlags::boosting)) { lbl_Player_BoostEndFunction(player); }

			}else if (player.superFormState >= 1){
				// untransform
				if(player.state == PlayerState::Run) { return; }

				player.superFormState = 0;
				player.superTails_transformCooldown = transformTime;
				// stInfo->playerMagnet = false; // Use this for magnet handler
				player.currentAir = (bgInfo->currentGauge > SuperTails_NonSuperStats.maxAir)
				? SuperTails_NonSuperStats.maxAir 
				: bgInfo->currentGauge; // set current gauge to air amount.
				// if(player.rings <= 20){
				// 	player.currentAir = static_cast<s32>(player.rings * 5500);
				// }else{
				// 	player.currentAir = SuperTails_NonSuperStats.maxAir;
				// }

				player.specialFlags ^= (SpecialFlags::iceImmunity
				                        | SpecialFlags::noSpeedLossChargingJump
				                        | SpecialFlags::ringGear
				                        | SpecialFlags::tornadoBoost
				                        | SpecialFlags::thirtyPercentAir
				);
				SuperTails_UpdatePlayerStats(player, &SuperTails_NonSuperStats, pSpeed(200));

				// Take excess gauge and convert it to tailwind timer (if not, 0 to turn it off)
				stInfo->tailwindTimer = (bgInfo->currentGauge - SuperTails_NonSuperStats.maxAir > 0) 
				? bgInfo->currentGauge - SuperTails_NonSuperStats.maxAir : 0;
				bgInfo->currentGauge = 0;
				// lbl_0008CC74(player.index); // spawn magnet
				player.typeAttributes &= ~Type::Speed;
			}
		}
	}
}