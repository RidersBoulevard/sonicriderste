#include "cosmetics/player/exloads.hpp"
#include "gears/berserker.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"
#include "mechanics/magneticimpulse.hpp"

namespace ZIgnore {
	constexpr s32 AcceleratorRingCost = 12;
	constexpr s32 BaseRingCost = 8;
	constexpr s32 HighBoostingExtraCost = 7;
}// namespace ZIgnore

ASMDefined void lbl_Player_BoostEndFunction(Player *);

constexpr std::array<f32, 3> ZIgnore_AverageBoostSpeeds = {
		pSpeed(200), pSpeed(230), pSpeed(250)};

constexpr m2darray<s32, 2, 3> ZIgnore_IgnoreCosts = {{
		{30000, 50000, 70000},// not high boosting
		{33000, 55000, 77000} // high boosting
}};

inline void Player_DecreaseMI(Player *player, f32 percentage) {
	if(player->magneticImpulse_timer >= MI::MaximumCap && MI::impulseData[player->index].maxMItimer < MI::MaxMITime) {
		return;
	}

	player->magneticImpulse_timer *= percentage;
}

ASMUsed bool Player_ZIgnoreTornado(Player *player) {
	bool isTornadoIgnored = FALSE;
	// f32 minWeight = 0.8f; // cream light board
	constexpr f32 baseWeight = 1.1f;// sonic
	// f32 maxWeight = 2.05f; // eggman heavy bike
	f32 weightDiff;
	f32 airMultiplier;

	if(!DebugMenu_CheckOption(DebugMenuOptions::TornadoIgnore)) {
		return isTornadoIgnored;
	}

	bool isHighBoosting = player->gearStats[player->level].boostSpeed > ZIgnore_AverageBoostSpeeds[player->level] && player->movementFlags.hasAny(MovementFlags::boosting);
	if(player->characterArchetype == CharacterArchetype::Boost) {
		isHighBoosting = player->gearStats[player->level].boostSpeed > (ZIgnore_AverageBoostSpeeds[player->level] + BoostArchetypeBoostSpeeds[player->level]) && player->movementFlags.hasAny(MovementFlags::boosting);
	}

	const auto zInput = player->input->holdFaceButtons.hasAny(Buttons::Z);
	const u8 &yInput = player->y_toggle;
	const u32 ignoreCount = static_cast<u8>(zInput) + yInput;

	if(ignoreCount != 0 && ignoreCount != 2) {
		if(player->tornadoIgnore_invincibilityTimer <= 0) {
			if(player->specialFlags.hasAny(SpecialFlags::ringGear)) {
				u8 ignoreLevel = (player->extremeGear == ExtremeGear::HangOn) ? player->level : 2;
				isHighBoosting = player->gearStats[player->level].boostSpeed > ZIgnore_AverageBoostSpeeds[ignoreLevel] && player->movementFlags.hasAny(MovementFlags::boosting);
				if(player->characterArchetype == CharacterArchetype::Boost) {
					isHighBoosting = player->gearStats[player->level].boostSpeed > (ZIgnore_AverageBoostSpeeds[ignoreLevel] + BoostArchetypeBoostSpeeds[ignoreLevel]) && player->movementFlags.hasAny(MovementFlags::boosting);
				}

				const bool acceleratorCheck = player->extremeGear == ExtremeGear::Accelerator;
				const s32 oneRing = (player->extremeGear == ExtremeGear::HangOn) ? player->gearStats[player->level].maxAir / 100 : player->gearStats[0].maxAir / 100;

				s32 ringCost = ZIgnore::BaseRingCost;

				// weightDiff = (CharacterWeights[player->character] + player->gearptr->weight) - baseWeight;
				//if(exLoads.characterExLoadID == Reala || //exLoads.characterExLoadID == GonGon || // exLoads.characterExLoadID == HatsuneMiku) {
				// 	weightDiff = (EXLoadWeights[exLoads.characterExLoadID] + player->gearptr->weight) - baseWeight;
				// }

				//if(weightDiff >= 0) {
				//	const f32 multiplier = weightDiff * 0.1f;
				//	airMultiplier = 1.0f + multiplier;
				//} else {
				//	airMultiplier = baseWeight + weightDiff;// inherently already scales down to 20% less cuz cream light board is 0.8f
				//	airMultiplier *= -1.0f;
				//}
				if(isHighBoosting) {
					ringCost += ZIgnore::HighBoostingExtraCost;
					//airMultiplier *= 3;
				} else {
					//airMultiplier *= 2;
				}
				//ringCost += airMultiplier;
				if(acceleratorCheck) {
					ringCost += ZIgnore::AcceleratorRingCost;
				}
				ringCost *= oneRing;

				//ringCost = (ZIgnore::BaseRingCost +
				//(isHighBoosting * ZIgnore::HighBoostingExtraCost) +
				//(acceleratorCheck * ZIgnore::AcceleratorRingCost)) * oneRing;

				const s32 newAir = player->currentAir - ringCost;
				if(newAir < 0) {
					isTornadoIgnored = FALSE;
				} else {
					isTornadoIgnored = TRUE;
					player->currentAir = newAir;
				}
			} else {
				// weightDiff = (CharacterWeights[player->character] + player->gearptr->weight) - baseWeight;
				//if(exLoads.characterExLoadID == Reala || //exLoads.characterExLoadID == GonGon || // exLoads.characterExLoadID == HatsuneMiku) {
				// 	weightDiff = (EXLoadWeights[exLoads.characterExLoadID] + player->gearptr->weight) - baseWeight;
				// }

				//if(weightDiff >= 0) {
				//	const f32 multiplier = weightDiff * 0.1f;
				//	airMultiplier = 1.0f + multiplier;
				//} else {
				//	airMultiplier = baseWeight + weightDiff;// inherently already scales down to 20% less cuz cream light board is 0.8f
				//}

				const s32 newAir = player->currentAir - ZIgnore_IgnoreCosts[isHighBoosting][player->level];
				// * airMultiplier;
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
					Player_DecreaseMI(player, 0.65f);
				} else {
					Player_DecreaseMI(player, 0.65f);
				}

				PlayAudioFromDAT(Sound::SFX::TornadoIgnore);// play tornado ignore sfx
			} else if(player->extremeGear == ExtremeGear::Berserker) {
				Player_BerserkerStatResetTornado(player);
			}
		} else {
			isTornadoIgnored = TRUE;
			player->tornadoIgnore_invincibilityTimer = 30;
			if(!(isHighBoosting)) {
				lbl_Player_BoostEndFunction(player);
				Player_DecreaseMI(player, 0.65f);
			} else {
				Player_DecreaseMI(player, 0.65f);
			}
		}
	}

	return isTornadoIgnored;
}