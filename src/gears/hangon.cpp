#include "hangon.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"
#include "cosmetics/player/exloads.hpp"
#include "handlers/player/specialflagtweaks.hpp"

// u32 hoInfo->lockoutFrameCounter = 0;

std::array<HangOnInfo, MaxPlayerCount> PlayerHangOnInfo;

constexpr GearLevelStats Level1 = {
		100000,     // max air
		8,          // air drain
		10,         // drift cost
		0x157C,     // boost cost
		0x1130,     // tornado cost
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
		200000,     // max air
		63,         // air drain
		203,        // drift cost
		0x7530,     // boost cost
		0x4E20,     // tornado cost
		pSpeed(100),// drift dash speed, unused
		pSpeed(300) // boost speed, unused
};

inline void Player_HangOn_UpdateStats(Player *player, const GearLevelStats *stats) {
	player->gearStats[player->level].maxAir = stats->maxAir;
    player->gearStats[player->level].driftCost = (player->characterArchetype == DriftArchetype) ? (stats->driftingAirCost * 90) / 100 : stats->driftingAirCost;
	player->gearStats[player->level].boostCost = stats->boostCost;
	player->gearStats[player->level].tornadoCost = stats->tornadoCost;
	// player->gearStats[player->level].boostSpeed = HangOn_Speeds[player->level][1];
	if(player->characterArchetype == BoostArchetype) {
		player->gearStats[player->level].boostSpeed = HangOn_Speeds[player->level][1] + BoostArchetypeBoostSpeeds[player->level];
	} else {
		player->gearStats[player->level].boostSpeed = HangOn_Speeds[player->level][1];
	}
	// player->gearStats[0].topSpeed = pSpeed(185); // level one top speed set manually
    player->gearStats[player->level].topSpeed = HangOn_Speeds[player->level][0] + Archetype_TopSpeedBonus[player->characterArchetype];
    if (player->state == Cruise) { player->speedCap = HangOn_Speeds[player->level][0] + Archetype_TopSpeedBonus[player->characterArchetype]; }
    player->gearStats[player->level].baseTopSpeed = HangOn_Speeds[player->level][0] - pSpeed(10.0f);
}

void Player_HangOn(Player *player) {
    // s32 newAir;
    SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];
    const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
    if (player->extremeGear != ExtremeGear::HangOn) { return; }
    if (exLoads.gearExLoadID == HangOnATEXLoad) { return; }

	if(player->level == 2 && player->movementFlags.hasAny(boosting)) {// level 3 has slightly less air drain while boosting
		player->gearStats[2].airDrain = 52;
	} else {
		switch(player->level) {
			case 0:
				player->gearStats[player->level].airDrain = Level1.passiveAirDrain;
				break;
			case 1:
				player->gearStats[player->level].airDrain = Level2.passiveAirDrain;
				break;
			case 2:
				player->gearStats[player->level].airDrain = Level3.passiveAirDrain;
				break;
			default:
				break;
		}
	}

	HangOnInfo *hoInfo = &PlayerHangOnInfo[player->index];
	if(player->state == StartLine) {
		player->level = 0;
		Player_HangOn_UpdateStats(player, &Level1);
		hoInfo->lockoutFrameCounter = 0;
		return;
	}

	if(player->hangOn_infiniteRingTimer > 0) {
		player->level = 0;
		Player_HangOn_UpdateStats(player, &Level1);
		player->gearSpecificFlags[HangOn::Level4] = true;
		hoInfo->lockoutFrameCounter = 0;
		return;
	}
	player->gearSpecificFlags[HangOn::Level4] = false;

	if(hoInfo->lockoutFrameCounter > 0 && player->state == Cruise) {
		hoInfo->lockoutFrameCounter--;
	}

	if(player->input->toggleFaceButtons.hasAny(BButton, XButton) && !player->movementFlags.hasAny(boosting) && player->state == Cruise) {
		if(player->level < 1) hoInfo->lockoutFrameCounter = 30;
		else if(player->level == 1)
			hoInfo->lockoutFrameCounter = 40;// level 2->3
	}

	if(player->input->getFaceButtons().hasAny(DPadLeft)) {
		// Gear 1
		if(player->level > 0 && player->movementFlags.hasAny(boosting)) {
			hoInfo->lockoutFrameCounter = 30;
		}
		if(player->level != 0) {
			PlayAudioFromDAT(Sound::SFX::HOTrans1);
		}
		player->level = 0;
		Player_HangOn_UpdateStats(player, &Level1);
	} else if(player->input->getFaceButtons().hasAny(DPadUp)) {
		// Gear 2
		if(player->movementFlags.hasAny(boosting)) {
			if(player->level == 0) {// 1->2
				if(hoInfo->lockoutFrameCounter == 0) {
					if(player->level != 1) {
						PlayAudioFromDAT(Sound::SFX::HOTrans2);
					}
					hoInfo->lockoutFrameCounter = 30;
					player->level = 1;
					Player_HangOn_UpdateStats(player, &Level2);
				}
			} else if(player->level == 2) {// 3->2
				if(player->level != 1) {
					PlayAudioFromDAT(Sound::SFX::HOTrans2);
				}
				player->level = 1;
				Player_HangOn_UpdateStats(player, &Level2);
			}
		} else if(player->level != 1 && !player->movementFlags.hasAny(boosting)) {
			if(player->level != 1) {
				PlayAudioFromDAT(Sound::SFX::HOTrans2);
			}
			player->level = 1;
			Player_HangOn_UpdateStats(player, &Level2);
		}
	} else if(player->input->getFaceButtons().hasAny(DPadRight)) {
		// Gear 3
		if(player->movementFlags.hasAny(boosting)) {
			if(player->level == 1) {// 2->3
				if(hoInfo->lockoutFrameCounter == 0) {
					if(player->level != 2) {
						PlayAudioFromDAT(Sound::SFX::HOTrans3);
					}
					player->level = 2;
					Player_HangOn_UpdateStats(player, &Level3);
				}
			}
		} else if(player->level < 2 && !player->movementFlags.hasAny(boosting)) {// 1/2->3
			if(player->level != 2) {
				PlayAudioFromDAT(Sound::SFX::HOTrans3);
			}
			player->level = 2;
			Player_HangOn_UpdateStats(player, &Level3);
		}
	}
}

void Player_HangOnAT(Player *player) {
    if (player->extremeGear != ExtremeGear::HangOn) return;
	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
    if (exLoads.gearExLoadID != HangOnATEXLoad) return;
    f32 acceleration;
    f32 newSpeedBoost;
    f32 newSpeedDrift;
    f32 baseSpeed = pSpeed(165.0f);
    f32 highSpeed = pSpeed(255.5f);

    if (player->typeAttributes == Type::Power) {
        baseSpeed = pSpeed(161.0f); // -4
    } else if (player->typeAttributes == Type::Fly) {
        baseSpeed = pSpeed(158.0f); // -7
    }

    SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];
    player->specialFlags &= ~ringGear;
    player->specialFlags &= ~fiftyPercentAir;
    player->specialFlags &= ~noPits;
    player->specialFlags |= tornadoBoost;
    player->gearStats[player->level].baseTopSpeed = baseSpeed + Archetype_TopSpeedBonus[player->characterArchetype];
    player->gearStats[player->level].topSpeed = highSpeed;

    if (player->speed > highSpeed) {
        player->gearStats[player->level].boostSpeed = highSpeed;
    } else if (player->speed < baseSpeed + Archetype_TopSpeedBonus[player->characterArchetype]) {
        player->gearStats[player->level].boostSpeed = baseSpeed + Archetype_TopSpeedBonus[player->characterArchetype];
    } else {
		player->gearStats[player->level].boostSpeed = player->speed;
	}

    if (player->state == StartLine) {
        player->gearStats[0].maxAir = 200000; // Level 3 air tank size
        player->gearStats[1].maxAir = 200000; // Level 3 air tank size
        player->gearStats[2].maxAir = 200000; // Level 3 air tank size
        player->currentAir = player->gearStats[player->level].maxAir;
    }
    player->unk9A8 = 0.0399999991059F;// 0x3D23D70A;

    switch (player->level) {
            case 0:
                player->gearStats[player->level].boostCost = 30000;
                player->gearStats[player->level].tornadoCost = 20000;
                if (player->movementFlags.hasAny(boosting))
                {newSpeedBoost = player->speed + 0.00810185185f;} // per frame
                if (player->movementFlags.hasAny(drifting, braking))
                {newSpeedDrift = player->speed - 0.00135030864f;}
                break;

            case 1:
                player->gearStats[player->level].boostCost = 55000;
                player->gearStats[player->level].tornadoCost = 35000;
                if (player->movementFlags.hasAny(boosting))
                {newSpeedBoost = player->speed + 0.0027006172839506f;} // per frame
                if (player->movementFlags.hasAny(drifting, braking))
                {newSpeedDrift = player->speed - 0.0027006172839506f;}
                break;

            case 2:
                player->gearStats[player->level].boostCost = 80000;
                player->gearStats[player->level].tornadoCost = 50000;
                if (player->movementFlags.hasAny(boosting))
                {newSpeedBoost = player->speed + 0.00135030864f;} // per frame
                if (player->movementFlags.hasAny(drifting, braking)) {
					newSpeedDrift = player->speed - 0.00810185185f;
				}
                break;

            default:
                break;
        }

    if (player->movementFlags.hasAny(drifting, braking) && player->state == Cruise) {
        if (player->movementFlags.hasAny(boosting)) {
            if (newSpeedDrift > baseSpeed + Archetype_TopSpeedBonus[player->characterArchetype]) player->speed = newSpeedDrift;
        } else if (newSpeedDrift > baseSpeed + Archetype_TopSpeedBonus[player->characterArchetype]) player->speed = newSpeedDrift;
    }
    else if (player->movementFlags.hasAny(boosting) && player->state == Cruise) {
        if (newSpeedBoost < highSpeed) player->speed = newSpeedBoost;
        player->speedCap = highSpeed;
    }

    if (player->speed > (highSpeed - pSpeed(40.0f))
        && player->speed < (highSpeed - pSpeed(10.0f))) {
        player->level = 1;
    } else if (player->speed > (highSpeed - pSpeed(10.0f))) {
        player->level = 2;
    } else player->level = 0;

}