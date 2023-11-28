#include "supermetal.hpp"
#include "containers/rgba.hpp"
#include "handlers/player/voicelines.hpp"
#include "lib/stdlib.hpp"
#include "supers.hpp"

ASMDefined void lbl_Player_BoostEndFunction(Player *);

constexpr GearLevelStats SuperMetal_SuperStats = {
        100000,    // max air
        0x10,      // air drain
        200,       // drift cost
        0x61A8,    // boost cost
        0x61A8,    // tornado cost
        pSpeed(0), // drift dash speed, unused
        pSpeed(250)// boost speed
};

constexpr GearLevelStats SuperMetal_NonSuperStats = {
        100000,    // max air
        0x10,      // air drain
        200,       // drift cost
        0x61A8,    // boost cost
        0x61A8,    // tornado cost
        pSpeed(0), // drift dash speed, unused
        pSpeed(235)// boost speed
};

inline void SuperMetal_UpdatePlayerStats(Player &player, const GearLevelStats &stats, f32 topSpeed) {
	player.gearStats[0].updateGearLevelStats(stats);
	player.gearStats[0].topSpeed = topSpeed;
	if(player.state == Cruise) { player.speedCap = topSpeed; }
}

inline void SuperMetal_PlayTransformationSound(const Player &player){
	if(!player.aiControl) { PlayAudioFromDAT(Sound::SFX::SuperTransformation); }// super transformation sfx
	bool scheck = false;
	for(const auto &curPlayer: getCurrentPlayerList()) {
		if(curPlayer.character == Sonic) {
			scheck = true;
			break;
		}
	}
	if(scheck && lbl_RNG_Number(10) == 1) {
		Sound::PlaySound(player, 19);
	} else {
		constexpr auto TransformationStart = 13;
		constexpr auto TransformationLen = 6;
		Sound::PlaySound(player, lbl_RNG_Number(TransformationLen) + TransformationStart);
	}
}

void Player_SuperMetalTransformation(Player &player) {
	if(player.playerType) { return; }
	if(!isSuperCharacter(player, MetalSonic)) { return; }
	if(player.flags.hasAny(InAPit)) { return; }// if in a pit

	if(player.rings >= 50 && player.superFormState == 0) {
		// transform
		player.superFormState = 1;
		player.specialFlags ^= (berserkerEffect);
		player.typeAttributes ^= (Type::Fly);
		constexpr RGBA SuperNeoMetalHUDColor = 0xFFC400FF;
		SuperNeoMetalHUDColors[player.index] = SuperNeoMetalHUDColor;
		SuperMetal_UpdatePlayerStats(player, SuperMetal_SuperStats, pSpeed(245));
		SuperMetal_PlayTransformationSound(player);
		if(player.movementFlags.hasAny(boosting)) { lbl_Player_BoostEndFunction(&player); }
	}
	if(player.rings == 0 && player.superFormState != 0) {
		// untransform
		player.superFormState = 0;
		player.SuperMetalFrameCounter = 0;
		player.specialFlags ^= (berserkerEffect);
		player.typeAttributes ^= (Type::Fly);
		constexpr RGBA NeoMetalHUDColor = 0x1239B8FF;
		SuperNeoMetalHUDColors[player.index] = NeoMetalHUDColor;
		SuperMetal_UpdatePlayerStats(player, SuperMetal_NonSuperStats, pSpeed(200));
	}
	if((player.state >= Cruise && player.state <= Jump) || player.state == Run) {// todo: check if this is correct
		if(player.superFormState != 0) {
			if(player.SuperMetalFrameCounter % 6 == 0) {
				player.rings -= 1;
			}
			player.SuperMetalFrameCounter++;
		}
	}
}
