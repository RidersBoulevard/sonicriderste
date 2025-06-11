#include "gears/turbostar.hpp"
#include "gears/blastGaugeGears.hpp"
#include "lib/stdlib.hpp"

constexpr std::array<f32, 3> EmerlDriftCapBonus = {
		pSpeed(30), pSpeed(15), pSpeed(5)
};

ASMUsed void *Player_DriftCap(Player *player, f32 return_array[2], f32 currentDriftCap, f32 f3) {
	f32 addCap;
	// f32 return_array[2];
	const PlayerGearStats &gearstats = player->gearStats[player->level];

	currentDriftCap += player->level * pSpeed(10);

	if(player->character == Character::Emerl) {
		if (!player->specialFlags.hasAny(SpecialFlags::ringGear))
		{currentDriftCap += EmerlDriftCapBonus[player->level];}
	}

	// if(player->extremeGear == ExtremeGear::SlideBooster) {
	// 	currentDriftCap -= pSpeed(10);
	// } else {
		switch(player->characterArchetype) {
			case CharacterArchetype::Drift:
			case CharacterArchetype::NoType:
				if(player->specialFlags.hasAny(static_cast<SpecialFlags>(0x00200000U))) {
					addCap = pSpeed(10);
				} else {
					addCap = pSpeed(20);
				}

				currentDriftCap += addCap;
				f3 += addCap;
				break;

			case CharacterArchetype::AllRounder:
				if(player->specialFlags.hasAny(static_cast<SpecialFlags>(0x00200000U))) {
					addCap = pSpeed(5);
				} else {
					addCap = pSpeed(10);
				}

				currentDriftCap += addCap;
				f3 += addCap;
				break;
			default:
				break;
		}

		if(!(player->specialFlags.hasAny(static_cast<SpecialFlags>(0x00400000U)))) {
			switch(player->extremeGear) {
				using namespace ExtremeGear;
				case Beginner:
					currentDriftCap -= pSpeed(50);
					currentDriftCap += gearstats.driftDashSpeed;
					currentDriftCap += pSpeed(10);
					break;

				case ExtremeGear::TurboStar:
					if(player->gearSpecificFlags[TurboStar::Level4]) currentDriftCap += pSpeed(10);
					break;

				case Grinder:
					currentDriftCap += pSpeed(15);
					break;
				case MagicCarpet:
					currentDriftCap += pSpeed(10);
					break;
				case AutoSlider:
					currentDriftCap += pSpeed(20);
					break;

				case SlideBooster:
					currentDriftCap -= pSpeed(10);
					if (player->characterArchetype == CharacterArchetype::Drift) {currentDriftCap -= pSpeed(10);}
					break;
				
				case ChaosEmerald:
				    currentDriftCap += pSpeed(10);
					if (player->character == Character::SuperSonic) {currentDriftCap += pSpeed(15);}
					if (player->isSuperCharacter(Character::Shadow)) {
						BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
						const f32 meterPercent = static_cast<f32>(bgInfo->currentGauge) / 200000.0f;
						currentDriftCap += pSpeed(10) * meterPercent; // up to +10 drift cap, depending on meter
					}
			        break;

				case AirTank:
					// Less air, more drift cap
					currentDriftCap += pSpeed(10.0f) * (1.0f - (static_cast<f32>(player->currentAir) / static_cast<f32>(player->gearStats[player->level].maxAir)));
					break;

			    case Accelerator:
                    if (player->gearExload().exLoadID == EXLoad::HyperHangOn) {
                        currentDriftCap += pSpeed(20.0f);
                    }
			        break;

				default:
					if(player->specialFlags.hasAny(SpecialFlags::ringGear)) {
						currentDriftCap += pSpeed(10);
					}
					break;
				
				case TheCrazy:
					currentDriftCap += pSpeed(10);
					s32 driftTips = ((player->driftDashFrames - 50) * player->gearStats[player->level].maxAir) / 1000;
					driftTips = clamp(driftTips, (player->gearStats[player->level].maxAir / 100), (player->gearStats[player->level].maxAir * 10 / 100));
					player->currentAir += driftTips;
					break;
			}
		} else {
			currentDriftCap += pSpeed(10);
		}
	// }

	return_array[0] = currentDriftCap;
	return_array[1] = f3;
	return return_array;
}

ASMUsed void Player_DriftCapOvercharge(Player* player, f32 currentDriftCap) {
	// f0 should be currentDriftCap
	// If current speed is less than drift cap, set it to drift cap
	if (player->speed <= currentDriftCap) {
		player->speed = currentDriftCap;
		return;
	}
	f32 isSlideBooster = (player->extremeGear == ExtremeGear::SlideBooster) ? 0.2 : 1;
	f32 newDriftCap = (player->speed - currentDriftCap) * (0.5 * isSlideBooster);
	// Else if current speed is greater than drift cap, add drift cap to it
	player->speed += (player->characterArchetype == CharacterArchetype::Drift) ? newDriftCap * 2: newDriftCap; 
}