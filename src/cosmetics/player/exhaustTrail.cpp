#define ASMHELPER_MAINFUNCNAME CallPlayer_ExhaustTrailColors
#define ASMHELPER_FUNCARGUMENTSETUP "mr r3, r27\n"
#define ASMHELPER_FUNCTOCALL Player_ExhaustTrailColors
#define ASMHELPER_RETURNVALUEFROMFUNC

#include "lib/ASMHelper.hpp"
#include "context.hpp"
#include "handlers/player/initgeardata.hpp"
#include "exloads.hpp"
#include "gears/hyperhangon.hpp"
#include "gears/dynamo.hpp"

global char bss_C24D91DC[4]; /*asm*/

constexpr u32 CoverF_ExhaustTrailColors[3] = {
		0x0040FFFF, // cruising archetype
		0x00FF00FF, // high boosting archetype
		0xFF0000FF // combat archetype
};

constexpr u32 Beginner_ExhaustTrailColors[4] = {
		0xFF8200FF,
		0x00ffffff,
		0x5E03FCFF,
		0xFFFFFFFF
};

constexpr f32 Beginner_BaseDriftSpeeds[3] = {
		pSpeed(50.0f),
		pSpeed(55.0f),
		pSpeed(60.0f)
};

constexpr f32 Beginner_DriftLevelSpeeds[4] = {
		0.0f,
		0.25f,
		0.5f,
		0.75f
};

void lbl_RainbowTrail(Player *Player) {
	u32 color;
	u8 r, g, b;
	char *bss = bss_C24D91DC;
	if (Player->playerType == 0) {
		u8 index = Player->index;
		if (Player->rainbowTrailState != 1) {
			Player->exhaustTrailColor = 0xFF0000FF;
			Player->rainbowTrailState = 1;
			bss[index] = 1;
		}

		color = Player->exhaustTrailColor;
		r = (color & 0xFF000000UL) >> 24;
		g = (color & 0x00FF0000UL) >> 16;
		b = (color & 0x0000FF00UL) >> 8;

		switch (bss[index]) {
			case 1:
				g += 3;
				if (g == 0xFF) {
					bss[index] = 2;
				}
				break;
			case 2:
				r -= 3;
				if (r == 0) {
					bss[index] = 3;
				}
				break;
			case 3:
				b += 3;
				if (b == 0xFF) {
					bss[index] = 4;
				}
				break;
			case 4:
				g -= 3;
				if (g == 0) {
					bss[index] = 5;
				}
				break;
			case 5:
				r += 3;
				if (r == 0xFF) {
					bss[index] = 6;
				}
				break;
			case 6:
				b -= 3;
				if (b == 0) {
					bss[index] = 1;
				}
				break;
			default:
				break;
		}

		color = static_cast<u32>((r << 24) | (g << 16) | (b << 8) | 0xFF);
		Player->exhaustTrailColor = color;
	}
}

ASMUsed u32 Player_ExhaustTrailColors(Player *Player) {
	u32 color = 0; // will stay 0 if color wasn't changed from default
	u32 beginnerDriftLevel, i;
	//u8 controllerPort = Player->input->port;
	EnabledEXLoads exLoads{};
	FetchEnabledEXLoadIDs(Player, exLoads);

	if (Player->playerType == 0 && (CurrentGameMode == 0x2BC || CurrentGameMode == 0x320 || CurrentGameMode == 0x258 ||
									CurrentGameMode == 0x12C || CurrentGameMode == 0x64)) {
		u8 gear = Player->extremeGear;

		if (Player->y_toggle != 0) {
			color = 0xBE1DEEFF;
		}

		if (Player->movementFlags & drifting) {
			if (Player->driftDashFrames >= Player->requiredDriftDashFrames) {
				color = 0x20DEDEFF;
			}
		}

		if (Player->character == Emerl || Player->extremeGear == CoverS) {
			if (Player->typeAttributes == SpeedType) {
				color = 0x034EFDFF;
			} else if (Player->typeAttributes == FlyType) {
				color = 0xEBD63DFF;
			} else if (Player->typeAttributes == PowerType) {
				color = 0xFF0000FF;
			}
		}

		switch (gear) {
			// case DefaultGear:
			// 	if (exLoads.gearExLoadID == WindmasterJetEXLoad)
			// 	{
			// 		struct WindmasterInfo *wmInfo = &PlayerWMInfo[Player->index];
			// 		if (wmInfo->lastShortcutType == 2) color = 0x034EFDFF; // FLY, BLUE
			// 		if (wmInfo->lastShortcutType == 1) color = 0xEBD63DFF; // SPEED, YELLOW
			// 	}
			// 	break;

			case TurboStar:
				if (color == 0) {
					if (Player->rings >= 90) {
						color = 0xC8C8C8E8;
					}
				}
				break;

			case CoverF: {
				u8 coverfstate = Player->coverF_archetype;
				if (coverfstate == 0) {
					break;
				}
				color = CoverF_ExhaustTrailColors[coverfstate - 1];
				break;
			}

			case CoverP: {
				f32 changedWeight = CharacterWeights[Player->character] + 0.4f;
				// TODO: change below to when we fix exload unloading
    			
				if (exLoads.characterExLoadID == RealaEXLoad || exLoads.characterExLoadID == GonGonEXLoad) changedWeight = EXLoadWeights[exLoads.characterExLoadID] + 0.4f;
				if (Player->weight == changedWeight) {
					// color = 0xFFFFFFFF;
					break;
				}
				if (Player->weight < changedWeight) {
					color = 0x0040FFFF;
					break;
				}
				if (Player->weight > changedWeight) {
					color = 0xFF0000FF;
					break;
				}
				break;
			}

			case TheCrazy: {
				u32 playerTopSpeed = u32(Player->gearStats[Player->level].topSpeed * SPEED_DIVISOR);
				if (Player->unkABC >= playerTopSpeed - 1) {
					break;
				} else if ((Player->unkABC < (playerTopSpeed - 1) && Player->unkABC >= (playerTopSpeed - 35)) && Player->rings >= 15)
				 {
					color = Beginner_ExhaustTrailColors[0];
				} // else if (Player->unkABC >= playerTopSpeed - 60 && Player->rings >= 30)
				// {
				// 	color = 0xFF0000FF;
				// }
				// else if (Player->unkABC >= playerTopSpeed - 60)
				// {
				// 	color = Beginner_ExhaustTrailColors[2];
				// }
				else if ((Player->unkABC < playerTopSpeed - 35) && Player->rings >= 25) {
					color = 0xFF0000FF;
				}
				break;
			}

			case HighBooster: {
				if (exLoads.gearExLoadID == StardustSpeederEXLoad) {
					if (Player->rings >= 25) {
						color = 0xEBD63DFF;
					}
				}
				break;
			}

			// case SuperHangOn:
			// 	struct HHOInfo *hhoInfo = &PlayerHHOInfo[Player->index];
			// 	if (exLoads.gearExLoadID == HyperHangOnEXLoad)
			// 	{
			// 		if (hhoInfo->extraType & SpeedType) {
			// 			color = 0x034EFDFF;
			// 		}
			// 		if (hhoInfo->extraType & FlyType) {
			// 			color = 0xEBD63DFF;
			// 		}
			// 		if (hhoInfo->extraType & PowerType) {
			// 			color = 0xFF0000FF;
			// 		}
			// 	}
			// 	break;

			case Gambler: {
				if (Player->character == E10G && Player->level4 == 0) {
					if (Player->typeAttributes & SpeedType) {
						color = 0x034EFDFF;
					}
					if (Player->typeAttributes & FlyType) {
						color = 0xEBD63DFF;
					}
					if (Player->typeAttributes & PowerType) {
						color = 0xFF0000FF;
					}
				}
				break;
			}

			case ChaosEmerald:
				if (color == 0) {
					if (Player->character == Tails) {
						if (Player->superFormState) {
							lbl_RainbowTrail(Player);
							color = Player->exhaustTrailColor;
						}
					} else {
						lbl_RainbowTrail(Player);
						color = Player->exhaustTrailColor;
					}

				}
				break;

			case Beginner:
				for (i = 0; i < 3; i++) {
					Player->gearStats[i].driftDashSpeed = Beginner_BaseDriftSpeeds[i];
				}

				if (Player->driftDashFrames <= 0) break;
				beginnerDriftLevel = static_cast<u32>(Player->driftDashFrames / 30);
				if (beginnerDriftLevel == 0) {
					color = 0xFF0000FF;
					break;
				}
				if (beginnerDriftLevel > 4) {
					beginnerDriftLevel = 4;
				}

				color = Beginner_ExhaustTrailColors[beginnerDriftLevel - 1];

				for (i = 0; i < 3; i++) {
					Player->gearStats[i].driftDashSpeed += Beginner_DriftLevelSpeeds[beginnerDriftLevel - 1];
				}
				break;
			default:
				break;
		}
	}

	return color;
}

ASMHELPER_FUNCTION