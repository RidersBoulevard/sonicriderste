#include "cosmetics/player/exloads.hpp"
#include "lib/stdlib.hpp"
#include "gears/hangon.hpp"
#include "gears/blastGaugeGears.hpp"
// #include "gears/hypersonic.hpp"
#include "gears/supers.hpp"

constexpr std::array<s16, 3> BoostControl_PowerGearAir = {0xA5, 0x14A, 0x1F4}; // for each level
constexpr std::array<s16, 3> BoostControl_AdvantageSAir = {0xF5, 0x1AA, 0x1F4};// for each level
constexpr std::array<s16, 3> BoostControl_FasterAir = {0x64, 0xFA, 0x190};     // for each level
constexpr std::array<s16, 3> BoostControl_HHOAir = {0x47, 0xDA, 0x0FA};        // for each level

ASMUsed void Player_BoostControl(Player *player) {
	// HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
	// HyperSonicInfo* hsInfo = &PlayerHyperSonicInfo[player->index];
	SuperShadowInfo* sshInfo = &PlayerSuperShadowInfo[player->index];

	bool isBoostControlActive = false;
	if(player->boostDuration == 1) {
		// see if player is initiating boost control
		if(player->input->holdFaceButtons.hasAny(Buttons::B, Buttons::X)) {
			if(player->isSuperCharacter(Character::Shadow)
			&& sshInfo->inhibitorRingsOff && sshInfo->chaosBoostControl == 1
			// && player->input->holdFaceButtons.hasAny(Buttons::X)
			) {
				BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
				// isBoostControlActive = bgInfo->currentGauge > 0;
				isBoostControlActive = true;
				const s32 newAir = player->currentAir - 128;// subtract roughly 10 rings for each second
				// const s32 newAir = bgInfo->currentGauge - 512;// subtract roughly 10 rings for each second
				player->currentAir = clamp(newAir);
				// bgInfo->currentGauge = clamp(newAir);
				// if (!isBoostControlActive) sshInfo->inhibitorRingsOff = false; // remove hyperdrive if empty
				// sshInfo->chaosBoostControl = isBoostControlActive;

			} /*else if (player->character == SuperSonic) {
                if (exLoads.gearExLoadID == DarkSonic) {
                    HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];
                    if (hsInfo->hyperdriveEnabled) {
                        isBoostControlActive = true;
						const s32 newAir = player->currentAir - 92;
                        player->currentAir = clamp(newAir, 0);
                    }
                }
            }*/ else {
				s32 newAir;
				switch(player->extremeGear) {
					using namespace ExtremeGear;
					case PowerGear:
						newAir = player->currentAir - (BoostControl_PowerGearAir[player->level] * 95) / 100;

						player->currentAir = clamp(newAir);
						isBoostControlActive = true;
						break;

					case AdvantageS:
						newAir = player->currentAir - (BoostControl_AdvantageSAir[player->level] * 80) / 100;
						newAir = clamp(newAir);
						player->currentAir = newAir;

						isBoostControlActive = newAir != 0;
						break;

					case OpaOpa:
						newAir = player->currentAir - BoostControl_FasterAir[player->level];
						newAir = clamp(newAir);
						player->currentAir = newAir;

						isBoostControlActive = newAir != 0;
						break;

					// case Faster:
					// 	if(player->characterArchetype == BoostArchetype || player->characterArchetype == TricksterArchetype) {
					// 		break;
					// 	}
					// 	newAir = player->currentAir - BoostControl_AdvantageSAir[player->level];

					// 	if(newAir < 0) {
					// 		newAir = 0;
					// 		isBoostControlActive = false;
					// 	} else {
					// 		isBoostControlActive = true;
					// 	}

					// 	player->currentAir = newAir;
					// 	break;

					default:
						break;
				}
			}
		}
	}

	if(!isBoostControlActive) {
		player->boostDuration -= 1;
	}
}