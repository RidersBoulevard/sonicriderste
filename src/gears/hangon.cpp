#include "hangon.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"
#include "cosmetics/player/exloads.hpp"
#include "handlers/player/specialflagtweaks.hpp"

// u32 hoInfo->lockoutFrameCounter = 0;

std::array<HangOnInfo, MaxPlayerCount> PlayerHangOnInfo;

constexpr GearLevelStats Level1 = {
		110000,     // max air
		10,          // air drain
		12,         // drift cost
		0x157C,     // boost cost
		0x157C,     // tornado cost
		pSpeed(100),// drift dash speed, unused
		pSpeed(270) // boost speed, unused
};
constexpr GearLevelStats Level2 = {
		133000,     // max air
		18,         // air drain
		50,         // drift cost
		0x32C8,     // boost cost
		0x1964,     // tornado cost
		pSpeed(100),// drift dash speed, unused
		pSpeed(270) // boost speed, unused
};
constexpr GearLevelStats Level3 = {
		220000,     // max air
		69,         // air drain
		176,        // drift cost
		0x80E8,     // boost cost
		0x55F0,     // tornado cost
		pSpeed(100),// drift dash speed, unused
		pSpeed(300) // boost speed, unused
};

inline void Player_HangOn_UpdateStats(Player &player, const GearLevelStats &stats) {
	player.gearStats[player.level].maxAir = stats.maxAir;
    player.gearStats[player.level].driftCost = (player.characterArchetype == CharacterArchetype::Drift) ? (stats.driftingAirCost * 90) / 100 : stats.driftingAirCost;
	player.gearStats[player.level].boostCost = stats.boostCost;
	player.gearStats[player.level].tornadoCost = stats.tornadoCost;
	// player.gearStats[player.level].boostSpeed = HangOn_Speeds[player.level][1];
	// if(player.characterArchetype == CharacterArchetype::Boost) {
	// 	player.gearStats[player.level].boostSpeed = HangOn_Speeds[player.level][1] + BoostArchetypeBoostSpeeds[player.level];
	// } else {
	// 	player.gearStats[player.level].boostSpeed = HangOn_Speeds[player.level][1];
	// }
	// player->gearStats[0].topSpeed = pSpeed(185); // level one top speed set manually
	f32 typeTopSpeedDebuff = 0.0f;
	if (player.characterptr->type == Character::Type::Fly) typeTopSpeedDebuff = pSpeed(7.0f);
	if (player.characterptr->type == Character::Type::Power) typeTopSpeedDebuff = pSpeed(4.0f);
    player.gearStats[player.level].topSpeed = HangOn_Speeds[player.level][0] + Archetype_TopSpeedBonus[std::to_underlying(player.characterArchetype)] - typeTopSpeedDebuff;
    if (player.state == PlayerState::Cruise) player.speedCap = HangOn_Speeds[player.level][0] + Archetype_TopSpeedBonus[std::to_underlying(player.characterArchetype)] - typeTopSpeedDebuff;
    player.gearStats[player.level].baseTopSpeed = HangOn_Speeds[player.level][0] - pSpeed(10.0f);
}

inline void Player_HandleShiftboosting(Player &player, const u8 prevLevel, const u8 shiftState) {
	// if(player.characterArchetype == BoostArchetype) {
	// 	player.gearStats[player.level].boostSpeed = HangOn_Speeds[prevLevel][1] + (pSpeed(5.3f)*shiftCount) + BoostArchetypeBoostSpeeds[prevLevel];
	// } else {
	// 	player.gearStats[player.level].boostSpeed = HangOn_Speeds[prevLevel][1] + (pSpeed(5.3f)*shiftCount);
	// }

	if(player.characterArchetype == CharacterArchetype::Boost) {
		player.gearStats[player.level].boostSpeed = HangOn_ShiftSpeeds[shiftState] + BoostArchetypeBoostSpeeds[prevLevel];
	} else {
		player.gearStats[player.level].boostSpeed = HangOn_ShiftSpeeds[shiftState];
	}
}

void Player_HangOn(Player &player) {
    // s32 newAir;
    //SpecialFlagInfo &spfInfo = PlayerSpecialFlagInfo[player.index];
    if (player.extremeGear != ExtremeGear::HangOn) { return; }

	if(player.level == 2 && player.movementFlags.hasAny(MovementFlags::boosting)) {// level 3 has slightly less air drain while boosting
		player.gearStats[2].airDrain = 52;
	} else {
		switch(player.level) {
			case 0:
				player.gearStats[player.level].airDrain = Level1.passiveAirDrain;
				break;
			case 1:
				player.gearStats[player.level].airDrain = Level2.passiveAirDrain;
				break;
			case 2:
				player.gearStats[player.level].airDrain = Level3.passiveAirDrain;
				break;
			default:
				break;
		}
	}

	auto &[lockoutFrameCounter, shiftCount, shiftState] = PlayerHangOnInfo[player.index];
	if (!player.movementFlags.hasAny(MovementFlags::boosting)) {
		shiftState = 0; // ensure there is no shiftboost bonus speed
		if(player.characterArchetype == CharacterArchetype::Boost) {
			player.gearStats[player.level].boostSpeed = HangOn_Speeds[player.level][1] + BoostArchetypeBoostSpeeds[player.level];
		} else {
			player.gearStats[player.level].boostSpeed = HangOn_Speeds[player.level][1];
		}
	}

	if(player.state == PlayerState::StartLine || player.state == PlayerState::AttackedByPlayer) {
		player.level = 0;
		Player_HangOn_UpdateStats(player, Level1);
		lockoutFrameCounter = 0;
		shiftCount = 0.0f;
		shiftState = 0;
		return;
	}


	if(player.hangOn_infiniteRingTimer > 0) {
		player.level = 0;
		Player_HangOn_UpdateStats(player, Level1);
		player.gearSpecificFlags[HangOn::Level4] = true;
		lockoutFrameCounter = 0;
		return;
	}
	player.gearSpecificFlags[HangOn::Level4] = false;

	if(lockoutFrameCounter > 0 && player.state == PlayerState::Cruise) {
		lockoutFrameCounter--;
	} else if (lockoutFrameCounter == 0) {
		shiftCount = 0.0f;
	}

	if(player.input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X) && !player.movementFlags.hasAny(MovementFlags::boosting) && player.state == PlayerState::Cruise) {
		if(player.level < 1) lockoutFrameCounter = 30;
		else if(player.level == 1)
			lockoutFrameCounter = 40;// level 2.3
	}

	if(player.input->getFaceButtons().hasAny(Buttons::DPadLeft)) {
		// Gear 1
		if(player.level > 0 && player.movementFlags.hasAny(MovementFlags::boosting)) {
			lockoutFrameCounter = 30;
		}
		if(player.level != 0) {
			PlayAudioFromDAT(Sound::SFX::HOTrans1);
		}
		player.level = 0;
		Player_HangOn_UpdateStats(player, Level1);
	} else if(player.input->getFaceButtons().hasAny(Buttons::DPadUp)) {
		// Gear 2
		if(player.movementFlags.hasAny(MovementFlags::boosting)) {
			if(player.level == 0) {// 1.2
				if(lockoutFrameCounter == 0) {
					if(player.level != 1) {
						PlayAudioFromDAT(Sound::SFX::HOTrans2);
						// hoInfo.shiftCount += 1.00f;
						shiftState = 1;
					}
					lockoutFrameCounter = 30;
					player.level = 1;
					Player_HangOn_UpdateStats(player, Level2);
					Player_HandleShiftboosting(player, 0, shiftState);
				}
			} else if(player.level == 2) {// 3->2
				if(player.level != 1) {
					PlayAudioFromDAT(Sound::SFX::HOTrans2);
				}
				player.level = 1;
				Player_HangOn_UpdateStats(player, Level2);
			}
		} else if(player.level != 1 && !player.movementFlags.hasAny(MovementFlags::boosting)) {
			if(player.level != 1) {
				PlayAudioFromDAT(Sound::SFX::HOTrans2);
			}
			player.level = 1;
			Player_HangOn_UpdateStats(player, Level2);
		}
	} else if(player.input->getFaceButtons().hasAny(Buttons::DPadRight)) {
		// Gear 3
		if(player.movementFlags.hasAny(MovementFlags::boosting)) {
			if(player.level == 1) {// 2.3
				if(lockoutFrameCounter == 0) {
					if(player.level != 2) {
						PlayAudioFromDAT(Sound::SFX::HOTrans3);
						if (shiftState == 1) {shiftState = 2;} // if 1->2, this is 1->2->3
						else shiftState = 3; // this is 2->3
						// if (hoInfo.shiftCount >= 1.00f) hoInfo.shiftCount -= 2.00f; // 1->2->3 (235->245)
						// else hoInfo.shiftCount += 2.00f; // 2->3 (250->260)
					}
					player.level = 2;
					Player_HangOn_UpdateStats(player, Level3);
					Player_HandleShiftboosting(player, 1, shiftState);
				}
			}
		} else if(player.level < 2 && !player.movementFlags.hasAny(MovementFlags::boosting)) {// 1/2->3
			if(player.level != 2) {
				PlayAudioFromDAT(Sound::SFX::HOTrans3);
			}
			player.level = 2;
			Player_HangOn_UpdateStats(player, Level3);
		}
	}
}