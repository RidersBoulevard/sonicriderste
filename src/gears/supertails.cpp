#include "supertails.hpp"
#include "lib/sound.hpp"
#include "cosmetics/player/exloads.hpp"
#include "eggsterminator.hpp"

constexpr GearLevelStats SuperTails_SuperStats = {
		220000, // max air
		0x16, // air drain
		0xA, // drift cost
		0x6B6C, // boost cost
		0x2260, // tornado cost
		pSpeed(0), // drift dash speed, unused
		pSpeed(270) // boost speed
};

constexpr GearLevelStats SuperTails_NonSuperStats = {
		110000, // max air
		0x10, // air drain
		0xA6, // drift cost
		0x61A8, // boost cost
		0x61A8, // tornado cost
		pSpeed(0), // drift dash speed, unused
		pSpeed(200) // boost speed
};

ASMUsed void SuperTails_SpawnMagnetAtStart(Player *player){
	if(isSuperCharacter(*player, Tails)){
		lbl_0008CC74(player->index); // spawn magnet
		player->superFormState = 0;
		player->typeAttributes &= ~Type::Power;
	}
}

ASMUsed void SuperTails_SpawnMagnetAfterDeath(Player *player){
	if(isSuperCharacter(*player, Tails) && player->superFormState == 0){
		lbl_0008CC74(player->index); // spawn magnet
	}
	if(player->extremeGear == ExtremeGear::CoverP){ // temporary, move to different function later
		player->level = 1;
		player->currentAir = player->gearStats[player->level].maxAir;
	}
	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
	if(player->extremeGear == ExtremeGear::CoverF
	   || player->extremeGear == ExtremeGear::CoverS
	   || exLoads.gearExLoadID == StardustSpeederEXLoad){ // temporary, move to different function later
		player->level = 1;
		player->currentAir = player->gearStats[player->level].maxAir;
	}

	player->reciproExtendTimer = 0; // grant recipro extend

	if(player->extremeGear == ExtremeGear::CoverF){
		player->level = 1;
		player->currentAir = player->gearStats[player->level].maxAir;
	}
}

inline void SuperTails_UpdatePlayerStats(Player &player, const GearLevelStats *stats, f32 topSpeed){
	player.gearStats[0].maxAir = stats->maxAir;
	player.gearStats[0].airDrain = stats->passiveAirDrain;
	player.gearStats[0].driftCost = stats->driftingAirCost;
	player.gearStats[0].boostCost = stats->boostCost;
	player.gearStats[0].tornadoCost = stats->tornadoCost;
	player.gearStats[0].boostSpeed = stats->boostSpeed;
	player.gearStats[0].topSpeed = topSpeed;
	if(player.state == Cruise) player.speedCap = topSpeed;
}

void Player_SuperTailsTransformation(Player &player){
	if(player.playerType) { return; }
	if(!isSuperCharacter(player, Tails)) { return; }
	if(player.superTails_transformCooldown > 0) { return; }
	if(player.flags.hasAny(InAPit)) { return; }
	constexpr auto transformationCost = 5;

	if(player.superFormState == 2 || player.rings >= 50){
		if((player.input->toggleFaceButtons & XButton) != 0u){
			if(player.superFormState == 0){
				// transform
				if(player.rings < transformationCost) { return; }

				player.superFormState = 1;
				player.superTails_transformCooldown = 180;
				player.rings -= transformationCost; // transform cost
				player.currentAir = static_cast<s32>(player.rings * 1100);
				player.specialFlags ^= (iceImmunity
				                        | noSpeedLossChargingJump
				                        | ringGear
				                        | tornadoBoost
				                        | thirtyPercentAir
				);

				SuperTails_UpdatePlayerStats(player, &SuperTails_SuperStats, pSpeed(215));

				// stop the beeping sound when low on air
				SNDF_SeqIdStop(player.index + 50);
				SNDF_SeqIdStop(player.index + 46);

				PlayAudioFromDAT(Sound::SFX::SuperTransformation); // super transformation sfx

				player.typeAttributes |= Type::Speed;
				if(player.movementFlags.hasAny(boosting)) { lbl_Player_BoostEndFunction(&player); }

			}else{
				// untransform
				if(player.state == Run) { return; }

				player.superFormState = 0;
				player.superTails_transformCooldown = 180;
				if(player.rings <= 20){
					player.currentAir = static_cast<s32>(player.rings * 5500);
				}else{
					player.currentAir = SuperTails_NonSuperStats.maxAir;
				}

				player.specialFlags ^= (iceImmunity
				                        | noSpeedLossChargingJump
				                        | ringGear
				                        | tornadoBoost
				                        | thirtyPercentAir
				);
				SuperTails_UpdatePlayerStats(player, &SuperTails_NonSuperStats, pSpeed(200));
				lbl_0008CC74(player.index); // spawn magnet
				player.typeAttributes &= ~Type::Speed;
			}
		}
	}
}