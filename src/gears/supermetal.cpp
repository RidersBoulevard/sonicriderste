#include "supermetal.hpp"
#include "supers.hpp"
#include "lib/sound.hpp"

global void lbl_Player_BoostEndFunction(Player *);

u32 SuperMetalFrameCounter = 0;

#define TransformationStart 12
#define TransformationLen 6
/*
 	0_Ramp_Jump (1).wav
 	1_C rank.wav
 	2_Successful_landing.wav
 	3_Getting_Attacked.wav
 	4_Passing_another_racer.wav
 	5_Level 3 Attack.wav
 	6_Max MI.wav
 	7_level 4 (MI).wav
 	8_level 4 (MI) alt.wav
 	9_NeoMaxMI.wav
 	10_NeoMaxMI2.wav
 	11_NeoMaxMI3.wav
 	12_Transformation.wav
 	13_Transformation2.wav
 	14_Transformation3.wav
 	15_Transformation4.wav
 	16_Transformation4alt.wav
 	17_Transformation5.wav
 	18_TransformationSonic.wav
 	19_NeoWin.wav
 	20_NEORaceStart.wav
*/

const struct GearLevelStats SuperMetal_SuperStats = {
		100000, // max air
		0x10, // air drain
		200, // drift cost
		0x61A8, // boost cost
		0x61A8, // tornado cost
		pSpeed(0), // drift dash speed, unused
		pSpeed(250) // boost speed
};

const struct GearLevelStats SuperMetal_NonSuperStats = {
		100000, // max air
		0x10, // air drain
		200, // drift cost
		0x61A8, // boost cost
		0x61A8, // tornado cost
		pSpeed(0), // drift dash speed, unused
		pSpeed(235) // boost speed
};

inline void SuperMetal_UpdatePlayerStats(Player &player, const GearLevelStats *stats, f32 topSpeed){
	player.gearStats[0].updateGearLevelStats(*stats);
	player.gearStats[0].topSpeed = topSpeed;
	if(player.state == Cruise) player.speedCap = topSpeed;
}

void Player_SuperMetalTransformation(Player &player){
	bool scheck = false;
	if(player.playerType == 1) return;
	if(!isSuperCharacter(player, MetalSonic)) return;
	if(player.flags & InAPit) return; // if in a pit

	if(player.rings >= 50 && player.superFormState == 0){
		// transform
		player.superFormState = 1;
		player.specialFlags ^= (berserkerEffect);
		player.typeAttributes ^= (FlyType);
		SuperNeoMetalHUDColors[player.index] = 0xFFC400FF;
		SuperMetal_UpdatePlayerStats(player, &SuperMetal_SuperStats, pSpeed(245));
		if(!player.aiControl) PlayAudioFromDAT(Sound::SFX::SuperTransformation); // super transformation sfx
		for (u32 i = 0; i < InGamePlayerCount; i++) {
        	if (player.index == i && player.character == Sonic) {
				bool scheck = true;
            	break;
        	}
		}
		if(scheck == true && lbl_RNG_Number(10) == 1) {
			Sound::PlaySound(player, 18);
		} else {
			Sound::PlaySound(player, lbl_RNG_Number(TransformationLen) + TransformationStart);
		}
		if (player.movementFlags & boosting) lbl_Player_BoostEndFunction(&player);
	}
	if(player.rings == 0 && player.superFormState == 1){
		// untransform
		player.superFormState = 0;
		SuperMetalFrameCounter = 0;
		player.specialFlags ^= (berserkerEffect);
		player.typeAttributes ^= (FlyType);
		SuperNeoMetalHUDColors[player.index] = 0x1239B8FF;
		SuperMetal_UpdatePlayerStats(player, &SuperMetal_NonSuperStats, pSpeed(200));
	}
	if((player.state >= Cruise && player.state <= Jump) || player.state == Run){ // todo: check if this is correct
		if(player.superFormState == 1){
			if(SuperMetalFrameCounter % 6 == 0){
				player.rings -= 1;
			}
			SuperMetalFrameCounter++;
		}

	}

}

