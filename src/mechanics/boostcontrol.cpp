#include "cosmetics/player/exloads.hpp"
#include "lib/stdlib.hpp"
#include "gears/blastGaugeGears.hpp"
#include "gears/hypersonic.hpp"
#include "gears/hangon.hpp"

constexpr std::array<s16, 3> BoostControl_PowerGearAir = {0xA5, 0x14A, 0x1F4}; // for each level
constexpr std::array<s16, 3> BoostControl_AdvantageSAir = {0xF5, 0x1AA, 0x1F4};// for each level
constexpr std::array<s16, 3> BoostControl_FasterAir = {0x64, 0xFA, 0x190};     // for each level
constexpr std::array<s16, 3> BoostControl_HHOAir = {0x47, 0xDA, 0x0FA};        // for each level

ASMUsed void Player_BoostControl(Player *player) {
	const u8 &controllerPort = player->input->port;
	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
	// HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];

	bool isBoostControlActive = false;
	if(player->boostDuration == 1) {
		// see if player is initiating boost control
		if(player->input->holdFaceButtons.hasAny(BButton, XButton)) {
			if(isSuperCharacter(*player, Shadow)) {
				isBoostControlActive = true;
				const s32 newAir = player->currentAir - 183;// subtract roughly 10 rings for each second

				player->currentAir = clamp(newAir, 0);

			} /*else if (player->character == SuperSonic) {
                if (exLoads.gearExLoadID == DarkSonicEXLoad) {
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

						player->currentAir = clamp(newAir, 0);
						isBoostControlActive = true;
						break;

					case AdvantageS:
						newAir = player->currentAir - (BoostControl_AdvantageSAir[player->level] * 80) / 100;
						newAir = clamp(newAir, 0);
						player->currentAir = newAir;

						isBoostControlActive = newAir != 0;
						break;

					case Faster:
						if(player->characterArchetype == BoostArchetype || player->characterArchetype == TricksterArchetype) {
							break;
						}
						newAir = player->currentAir - BoostControl_AdvantageSAir[player->level];

						if(newAir < 0) {
							newAir = 0;
							isBoostControlActive = false;
						} else {
							isBoostControlActive = true;
						}

						player->currentAir = newAir;
						break;

					case ExtremeGear::SuperHangOn:
						if(exLoads.gearExLoadID == E99EXLoad) {
							if(player->gearSpecificFlags[SuperHangOn::Level4]) {
								newAir = player->currentAir - BoostControl_FasterAir[player->level];
								if(newAir < 0) newAir = 0;
								player->currentAir = newAir;
								isBoostControlActive = true;
								break;
							}
							break;
						}
						break;
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