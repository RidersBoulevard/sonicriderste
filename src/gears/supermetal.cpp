#include "supermetal.hpp"
#include "containers/rgba.hpp"
#include "handlers/player/voicelines.hpp"
#include "lib/stdlib.hpp"
#include "supers.hpp"

ASMDefined void lbl_Player_BoostEndFunction(Player *);

constexpr GearLevelStats SuperMetal_SuperStats = {
        100000,    // max air
        0x14,      // air drain
        200,       // drift cost
        0x7530,    // boost cost
        0x61A8,    // tornado cost
        pSpeed(100), // drift dash speed
        pSpeed(245)// boost speed
};

constexpr GearLevelStats SuperMetal_NonSuperStats = {
        100000,    // max air
        0x14,      // air drain
        200,       // drift cost
        0x7530,    // boost cost
        0x61A8,    // tornado cost
        pSpeed(100), // drift dash speed
        pSpeed(235)// boost speed
};

inline void SuperMetal_UpdatePlayerStats(Player &player, const GearLevelStats &stats, f32 topSpeed) {
	player.gearStats[0].updateGearLevelStats(stats);
	player.gearStats[0].topSpeed = topSpeed;
	if(player.state == PlayerState::Cruise) { player.speedCap = topSpeed; }
}

inline void SuperMetal_PlayTransformationSound(const Player &player){
	if(!player.aiControl) { PlayAudioFromDAT(Sound::SFX::SuperTransformation); }// super transformation sfx
	bool scheck = false;
	for(const auto &curPlayer: getCurrentPlayerList()) {
		if(curPlayer.character == Character::Sonic) {
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
	if(!isSuperCharacter(player, Character::MetalSonic)) { return; }
	if(player.flags.hasAny(PlayerFlags::InAPit)) { return; }// if in a pit

	if(player.rings >= 50 && player.superFormState == 0) {
		// transform
		player.superFormState = 1;
		// player.specialFlags ^= SpecialFlags::berserkerEffect;
		player.typeAttributes ^= Type::Fly;
		constexpr RGBA32 SuperNeoMetalHUDColor = 0xFFC400FF;
		SuperNeoMetalHUDColors[player.index] = SuperNeoMetalHUDColor;
		SuperMetal_UpdatePlayerStats(player, SuperMetal_SuperStats, pSpeed(230));
		SuperMetal_PlayTransformationSound(player);
		if(player.movementFlags.hasAny(MovementFlags::boosting)) { lbl_Player_BoostEndFunction(&player); }
	}
	if(player.rings == 0 && player.superFormState != 0) {
		// untransform
		player.superFormState = 0;
		player.SuperMetalFrameCounter = 0;
		// player.specialFlags ^= SpecialFlags::berserkerEffect;
		player.typeAttributes ^= Type::Fly;
		constexpr RGBA32 NeoMetalHUDColor = 0x1239B8FF;
		SuperNeoMetalHUDColors[player.index] = NeoMetalHUDColor;
		SuperMetal_UpdatePlayerStats(player, SuperMetal_NonSuperStats, pSpeed(190));
	}
	if((player.state >= PlayerState::Cruise && player.state <= PlayerState::Jump) || player.state == PlayerState::Run) {// todo: check if this is correct
		if(player.superFormState != 0) {
			if(player.SuperMetalFrameCounter % 6 == 0) {
				player.rings -= 1;
			}
			player.SuperMetalFrameCounter++;
		}
	}
}
