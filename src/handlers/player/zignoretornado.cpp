#include "context.hpp"
#include "gears/berserker.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"
#include "lib/sound.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "cosmetics/player/exloads.hpp"
#include "handlers/player/initgeardata.hpp"

namespace ZIgnore {
	constexpr s32 AcceleratorRingCost = 12;
	constexpr s32 BaseRingCost = 8;
	constexpr s32 HighBoostingExtraCost = 7;
}

global void lbl_Player_BoostEndFunction(Player *);

constexpr f32 ZIgnore_AverageBoostSpeeds[3] = {
        pSpeed(200), pSpeed(230), pSpeed(250)};

constexpr s32 ZIgnore_IgnoreCosts[2][3] = {
        {30000, 50000, 70000},// not high boosting
        {33000, 55000, 77000} // high boosting
};

inline void Player_DecreaseMI(Player *player, f32 percentage) {
	if(player->magneticImpulse_timer >= MI_MaximumCap && magneticImpulseData[player->index].maxMItimer < MI_MaxMITime)
		return;

	player->magneticImpulse_timer *= percentage;
}

ASMUsed bool Player_ZIgnoreTornado(Player *player) {
	bool isTornadoIgnored = FALSE;
	// f32 minWeight = 0.8f; // cream light board
	f32 baseWeight = 1.1f; // sonic
	// f32 maxWeight = 2.05f; // eggman heavy bike
	f32 weightDiff;
	f32 airMultiplier;
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if(!DebugMenu_CheckOption(TornadoIgnore)) return isTornadoIgnored;

	bool isHighBoosting = ((player->gearStats[player->level].boostSpeed > ZIgnore_AverageBoostSpeeds[player->level]) && (player->movementFlags & boosting)) ? TRUE : FALSE;
	if (player->characterArchetype == BoostArchetype) 
	{isHighBoosting = ((player->gearStats[player->level].boostSpeed > (ZIgnore_AverageBoostSpeeds[player->level] + BoostArchetypeBoostSpeeds[player->level])) && (player->movementFlags & boosting)) ? TRUE : FALSE;}

	u32 zInput = (player->input->holdFaceButtons & ZButton) ? TRUE : FALSE;
	u8 yInput = player->y_toggle;
	u32 ignoreCount = zInput + yInput;

	if(!(ignoreCount == 0 || ignoreCount == 2)) {
		if(player->tornadoIgnore_invincibilityTimer <= 0) {
			if(player->specialFlags & ringGear) {
				isHighBoosting = ((player->gearStats[0].boostSpeed > ZIgnore_AverageBoostSpeeds[2]) && (player->movementFlags & boosting)) ? TRUE : FALSE;
				if (player->characterArchetype == BoostArchetype) 
				{isHighBoosting = ((player->gearStats[0].boostSpeed > (ZIgnore_AverageBoostSpeeds[2] + BoostArchetypeBoostSpeeds[2])) && (player->movementFlags & boosting)) ? TRUE : FALSE;}

				bool acceleratorCheck = (player->extremeGear == Accelerator) ? TRUE : FALSE;
				s32 oneRing = player->gearStats[0].maxAir / 100;

				s32 ringCost = ZIgnore::BaseRingCost;

				weightDiff = (CharacterWeights[player->character] + player->gearptr->weight) - baseWeight;
				if (exLoads.characterExLoadID == RealaEXLoad || 
				exLoads.characterExLoadID == GonGonEXLoad || 
				exLoads.characterExLoadID == HatsuneMikuEXLoad) {
					weightDiff = (EXLoadWeights[exLoads.characterExLoadID] + player->gearptr->weight) - baseWeight;
				}

				if (weightDiff >= 0) {
					f32 multiplier = weightDiff * 0.1f;
					airMultiplier = 1.0f + multiplier;
				} else {
					airMultiplier = baseWeight + weightDiff; // inherently already scales down to 20% less cuz cream light board is 0.8f
					airMultiplier *= -1.0f;
				}

				if(isHighBoosting) {
					ringCost += ZIgnore::HighBoostingExtraCost;
					airMultiplier *= 3; 
					ringCost += airMultiplier;
				} else {
					airMultiplier *= 2; 
					ringCost += airMultiplier;
				}
				if(acceleratorCheck) ringCost += ZIgnore::AcceleratorRingCost;
				ringCost *= oneRing;

				//ringCost = (ZIgnore::BaseRingCost +
				//(isHighBoosting * ZIgnore::HighBoostingExtraCost) +
				//(acceleratorCheck * ZIgnore::AcceleratorRingCost)) * oneRing;

				s32 newAir = player->currentAir - ringCost;
				if(newAir < 0) {
					isTornadoIgnored = FALSE;
				} else {
					isTornadoIgnored = TRUE;
					player->currentAir = newAir;
				}
			} else {
				weightDiff = (CharacterWeights[player->character] + player->gearptr->weight) - baseWeight;
				if (exLoads.characterExLoadID == RealaEXLoad || 
				exLoads.characterExLoadID == GonGonEXLoad || 
				exLoads.characterExLoadID == HatsuneMikuEXLoad) {
					weightDiff = (EXLoadWeights[exLoads.characterExLoadID] + player->gearptr->weight) - baseWeight;
				}

				if (weightDiff >= 0) {
					f32 multiplier = weightDiff * 0.1f;
					airMultiplier = 1.0f + multiplier;
				} else {
					airMultiplier = baseWeight + weightDiff; // inherently already scales down to 20% less cuz cream light board is 0.8f
				}

				s32 newAir = player->currentAir - ZIgnore_IgnoreCosts[isHighBoosting][player->level] * airMultiplier;
				// s32 newAir = player->currentAir - ZIgnore_IgnoreCosts[isHighBoosting][player->level];

				if(newAir < 0) {
					isTornadoIgnored = FALSE;
				} else {
					isTornadoIgnored = TRUE;
					player->currentAir = newAir;
				}
			}

			if(isTornadoIgnored) {
				player->tornadoIgnore_invincibilityTimer = 30;
				if(!(isHighBoosting)) {
					lbl_Player_BoostEndFunction(player);
					Player_DecreaseMI(player, 0.85f);
				} else {
					Player_DecreaseMI(player, 0.9f);
				}

				PlayAudioFromDAT(Sound::SFX::TornadoIgnore);// play tornado ignore sfx
			} else if(player->extremeGear == Berserker) {
				Player_BerserkerStatResetTornado(player);
			}
		} else {
			isTornadoIgnored = TRUE;
			player->tornadoIgnore_invincibilityTimer = 30;
			if(!(isHighBoosting)) {
				lbl_Player_BoostEndFunction(player);
				Player_DecreaseMI(player, 0.85f);
			} else {
				Player_DecreaseMI(player, 0.9f);
			}
		}
	}

	return isTornadoIgnored;
}