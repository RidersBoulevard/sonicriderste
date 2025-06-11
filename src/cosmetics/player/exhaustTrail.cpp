#define ASMHELPER_MAINFUNCNAME CallPlayer_ExhaustTrailColors
#define ASMHELPER_FUNCARGUMENTSETUP "mr r3, r27\n"
#define ASMHELPER_FUNCTOCALL Player_ExhaustTrailColors
#define ASMHELPER_RETURNVALUEFROMFUNC

#include "exloads.hpp"
#include "gears/gambler.hpp"
#include "gears/hangon.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/ASMHelper.hpp"
#include "riders/gamemode.hpp"
#include "riders/gear.hpp"
#include "handlers/files/separatemodelloading.hpp"
#include "gears/omnipotence.hpp"
#include "gears/turbostar.hpp"
#include "handlers/player/specialflagtweaks.hpp"
#include "gears/advantagep.hpp"
#include "gears/supermetal.hpp"

ASMDefined std::array<char, 4> bss_C24D91DC; /*asm*/

constexpr std::array<u32, 3> CoverF_ExhaustTrailColors = {
	0x0040FFFF, // cruising archetype
	0x00FF00FF, // high boosting archetype
	0xFF0000FF // combat archetype
};

constexpr std::array<u32, 4> Beginner_ExhaustTrailColors = {
	0xFF8200FF,
	0x00ffffff,
	0x5E03FCFF,
	0xFFFFFFFF
};

constexpr std::array<f32, 3> Beginner_BaseDriftSpeeds = {
	pSpeed(50.0f),
	pSpeed(55.0f),
	pSpeed(60.0f)
};

constexpr std::array<f32, 4> Beginner_DriftLevelSpeeds = {
	0.0f,
	0.25f,
	0.5f,
	0.75f
};

constexpr std::array<GearExhaustTrail, 2> EggmanExhaustTrailData = {
	{
		// whilst cruising
		{
			{0.4, 0.4, 0.0, 0.0},
			{},
			{{{0.18, -0.05, -0.75}, {-0.18, -0.05, -0.75}}},
			{{{2, 2}, {2, 2}}},
			0.0,
			0x000A0001,
			0,
			2,
			0,
			0
		},

		// whilst tricking
		{
			{0.4, 0.4, 0.0, 0.0},
			{},
			{{{0.18, -0.05, -0.75}, {-0.18, -0.05, -0.75}}},
			{{{2, 2}, {2, 2}}},
			0.08,
			0x00280100,
			2,
			2,
			0,
			0
		},
	}
};

void lbl_RainbowTrail(Player &Player) {
	const std::span bss = bss_C24D91DC;
	if(!Player.playerType) {
		const u8 &index = Player.index;
		if(!Player.rainbowTrailState) {
			Player.exhaustTrailColor = 0xFF0000FF;
			Player.rainbowTrailState = true;
			bss[index]                = 1;
		}

		auto &[r, g, b, a] = Player.exhaustTrailColor;
		//u8 r = (color & 0xFF000000UL) >> 24;
		//u8 g = (color & 0x00FF0000UL) >> 16;
		//u8 b = (color & 0x0000FF00UL) >> 8;

		switch(bss[index]) {
			case 1:
				g += 3;
				if(g == 0xFF) {
					bss[index] = 2;
				}
				break;
			case 2:
				r -= 3;
				if(r == 0) {
					bss[index] = 3;
				}
				break;
			case 3:
				b += 3;
				if(b == 0xFF) {
					bss[index] = 4;
				}
				break;
			case 4:
				g -= 3;
				if(g == 0) {
					bss[index] = 5;
				}
				break;
			case 5:
				r += 3;
				if(r == 0xFF) {
					bss[index] = 6;
				}
				break;
			case 6:
				b -= 3;
				if(b == 0) {
					bss[index] = 1;
				}
				break;
			default: break;
		}

		//color = static_cast<u32>((r << 24) | (g << 16) | (b << 8) | 0xFF);
		//Player.exhaustTrailColor = color;
	}
}

ASMUsed u32 Player_ExhaustTrailColors(Player &Player) {
	u32 color          = 0; // will stay 0 if color wasn't changed from default
	SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[Player.index];
	//u8 controllerPort = Player.input.port;

	if(Player.playerType == 0 && (CurrentGameMode == 0x2BC || CurrentGameMode == 0x320 || CurrentGameMode == 0x258 || CurrentGameMode == 0x12C ||
								   CurrentGameMode == 0x64)) {
		bool trailOverride = false;
		// if(Player.movementFlags.hasAny(drifting)) {
		// 	if(Player.driftDashFrames >= Player.requiredDriftDashFrames) {
		// 		color = 0x20DEDEFF;
		// 	}
		// }

		const auto &gear = Player.extremeGear;

		if(Player.specialFlags.hasAny(SpecialFlags::ringGear) && !Player.isSuperCharacter(true)) {
			color = 0xD5BD56FF;
		}

		if(Player.character == Character::Emerl || gear == ExtremeGear::CoverS) {
			if(Player.typeAttributes == Type::Speed) {
				color = 0x034EFDFF;
			} else if(Player.typeAttributes == Type::Fly) {
				color = 0xEBD63DFF;
			} else if(Player.typeAttributes == Type::Power) {
				color = 0xFF0000FF;
			} else if(Player.typeAttributes == Type::All) {
				lbl_RainbowTrail(Player);
				color = Player.exhaustTrailColor;
			}
		}

		switch(gear) {
			using namespace ExtremeGear;
			case ExtremeGear::TurboStar:
				if(color == 0) {
					if(Player.rings >= 90) {
						color = 0xC8C8C8E8;
					}
				}
				break;

			case CoverF: {
				const u8 &coverfstate = Player.coverF_archetype;
				if(coverfstate == 0) {
					break;
				}
				color = CoverF_ExhaustTrailColors[coverfstate - 1];
				break;
			}

			case CoverP: {
				f32 changedWeight = CharacterWeights[Player.character] + 0.4f;
				// TODO: change below to when we fix exload unloading

				if(Player.characterExload().exLoadID != EXLoad::None && Player.characterExload().weight() != -1.0f) {
					changedWeight = Player.characterExload().weight() + 0.4f;
				}
				if(Player.weight == changedWeight) {
					// color = 0xFFFFFFFF;
					break;
				}
				if(Player.weight < changedWeight) {
					color = 0x0040FFFF;
					break;
				}
				if(Player.weight > changedWeight) {
					color = 0xFF0000FF;
					break;
				}
				break;
			}

			case TheCrazy: {
				const u32 playerTopSpeed = static_cast<u32>(Player.gearStats[Player.level].topSpeed * SPEED_DIVISOR);
				if(Player.unkABC >= playerTopSpeed - 1) {
					break;
				}
				if((Player.unkABC < (playerTopSpeed - 1) && Player.unkABC >= (playerTopSpeed - 35)) && Player.rings >= 15) {
					color         = Beginner_ExhaustTrailColors[0];
					trailOverride = true;
					break;
				}
				if((Player.unkABC < (playerTopSpeed - 35) && Player.unkABC >= (playerTopSpeed - 85)) && Player.rings >= 20) {
					color         = 0xFF0000FF;
					trailOverride = true;
					break;
				}
				if((Player.unkABC < (playerTopSpeed - 85)) && Player.rings >= 30) {
					color         = 0xFFFFFFFF;
					trailOverride = true;
					break;
				}
				break;
			}

			case GunGear: {
				if(Player.rings >= 25) {
					color = 0xFF0000FF;
				}
				break;
			}

			case ExtremeGear::HangOn: {
				HangOnInfo &hoInfo = PlayerHangOnInfo[Player.index];
				if(hoInfo.lockoutFrameCounter > 0 && Player.movementFlags.hasAny(MovementFlags::boosting)) {
					color         = 0xFF0000FF;
					trailOverride = true;
				}
				break;
			}
			case ExtremeGear::SuperHangOn:
				if(color == 0) {
					if(Player.rings >= 90) {
						color = 0xC8C8C8E8;
					}
				}
				// struct HHOInfo *hhoInfo = &PlayerHHOInfo[Player.index];
				// if (exLoads.gearExLoadID == EXLoad::HyperHangOn)
				// {
				// 	if (hhoInfo.extraType & SpeedType) {
				// 		color = 0x034EFDFF;
				// 	}
				// 	if (hhoInfo.extraType & FlyType) {
				// 		color = 0xEBD63DFF;
				// 	}
				// 	if (hhoInfo.extraType & PowerType) {
				// 		color = 0xFF0000FF;
				// 	}
				// }
				break;

			case Omnipotence: {
				OmnipotenceInfo &OMNInfo = PlayerOMNInfo[Player.index];
				if (OMNInfo.lastShortcutType == 1) color = 0x034EFDFF; // Last path SPEED, BLUE
				if (OMNInfo.lastShortcutType == 2) color = 0xEBD63DFF; // Last path FLY, YELLOW
				if (OMNInfo.lastShortcutType == 3) color = 0xFF0000FF; // Last path POWER, RED
				break;
			}

			case ExtremeGear::Gambler: {
				// if(Player.character == Character::E10G && !Player.gearSpecificFlags[Gambler::Level4]) {
				// 	if(Player.typeAttributes.hasAny(Type::Speed)) {
				// 		color = 0x034EFDFF;
				// 	}
				// 	if(Player.typeAttributes.hasAny(Type::Fly)) {
				// 		color = 0xEBD63DFF;
				// 	}
				// 	if(Player.typeAttributes.hasAny(Type::Power)) {
				// 		color = 0xFF0000FF;
				// 	}
				// }
				// if(Player.typeAttributes == Type::All) {
				// 	lbl_RainbowTrail(Player);
				// 	color = Player.exhaustTrailColor;
				// }
				if(color == 0) {
					if(Player.rings >= 90) {
						color = 0xC8C8C8E8;
					}
				}
				break;
			}

		    case ChaosEmerald: if(color != 0) {
					break;
				}
				if(Player.character == Character::Tails) {
					if(Player.superFormState != 0) {
						lbl_RainbowTrail(Player);
						color = Player.exhaustTrailColor;
					}
				} else if (Player.character == Character::MetalSonic && Player.gearExload().exLoadID == EXLoad::StardustSpeeder) {
                    const NeoMetalInfo *neoInfo = &PlayerNeoMetalInfo[Player.index];
				    // if in burn state, show alternate type given
				    if (neoInfo->formState == 1) {
				        if (Player.typeAttributes.hasAny(Type::Speed)) color = 0x034EFDFF;
				        if (Player.typeAttributes.hasAny(Type::Fly)) color = 0xEBD63DFF;
				        if (Player.typeAttributes.hasAny(Type::Power)) color = 0xFF0000FF;
				    } else if (neoInfo->formState == 2) {
				        lbl_RainbowTrail(Player);
				        color = Player.exhaustTrailColor;
				    }
				} else {
					lbl_RainbowTrail(Player);
					color = Player.exhaustTrailColor;
				}
				break;

			case Beginner: {
				for(u32 i = 0; i < 3; i++) {
					Player.gearStats[i].driftDashSpeed = Beginner_BaseDriftSpeeds[i];
				}

				if(Player.driftDashFrames <= 0) {
					break;
				}

				if(Player.movementFlags.hasAny(MovementFlags::drifting)) {
					trailOverride = true;
				}
				u32 beginnerDriftLevel = static_cast<u32>(Player.driftDashFrames / 30);
				if(beginnerDriftLevel == 0) {
					color = 0xFF0000FF;
					break;
				}
				if(beginnerDriftLevel > 4) {
					beginnerDriftLevel = 4;
				}

				color = Beginner_ExhaustTrailColors[beginnerDriftLevel - 1];

				for(u32 i = 0; i < 3; i++) {
					Player.gearStats[i].driftDashSpeed += Beginner_DriftLevelSpeeds[beginnerDriftLevel - 1];
				}
				break;
			}

			case AdvantageP: {
    			const AdvantagePInfo &advpInfo = PlayerAdvantagePInfo[Player.index];
				if (advpInfo.isInTopMode)
				{color = 0xFF0000FF;}
				break;
			}

			default:
				break;
		}

        if(!trailOverride && Player.ignoreTurbulence) {
            color = 0xBE1DEEFF;
        }

		if (spfInfo->typeRemovalTimer > 0.0f) {
			color = 0xC8C8C8E8;
		}

		if(Player.movementFlags.hasAny(MovementFlags::drifting) && Player.extremeGear != ExtremeGear::Beginner) {
			if(Player.driftDashFrames >= 150 && Player.extremeGear == ExtremeGear::TheCrazy) {
				color = 0x00FF00FF;
			} else if(Player.driftDashFrames >= Player.requiredDriftDashFrames) {
				color = 0x20DEDEFF;
			}
		}
	}

	return color;
}

ASMHELPER_FUNCTION

// for the fucking life of me I can't assign a pointer to trailData if it were to be the correct type, so it's an u32 :)

inline void SetCustomExhaustTrail(Player &player, u32 &trailData, const u32 index) {
	if(PlayerIsEggmeister(player)) {
		trailData = reinterpret_cast<u32>(&EggmanExhaustTrailData[index]);
	}
}

ASMUsed void Player_CustomExhaustTrail_Cruising(Player &player, u32 &trailData) {
	SetCustomExhaustTrail(player, trailData, 0);
}

ASMUsed void Player_CustomExhaustTrail_Tricking(Player &player, u32 &trailData) {
	SetCustomExhaustTrail(player, trailData, 1);
}
