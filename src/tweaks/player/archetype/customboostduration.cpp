#include "cosmetics/player/exloads.hpp"
#include "gears/turbostar.hpp"
#include "riders/gamemode.hpp"
#include "handlers/player/specialflagtweaks.hpp"
#include "gears/faster.hpp"
#include "tweaks/player/archetype/boostarchetypejcbc.hpp"

constexpr m2darray<u8, std::to_underlying(CharacterArchetype::Count), 3> Archetype_BoostDuration = {{
		{0x14, 0x14, 0x14},// all rounder
		{0x00, 0x3c, 0x3c},// late booster
		{0x1E, 0x1E, 0x1E},// top speed
		{0x46, 0x46, 0x00},// combat
		{0x00, 0x00, 0x00},// drift
		{0x46, 0x46, 0x46},// no type
		{0x00, 0x00, 0x00},// super sonic
		{0x00, 0x00, 0x00},// boost
		{0x00, 0x00, 0x00},// trickster
		{0x00, 0x00, 0x00},// none archetype
		{0x00, 0x00, 0x00},// Windcatcher
		{0x2D, 0x2D, 0x0A},// Turning (WIP new combat)
		{0x00, 0x00, 0x00},// Mechanic
}};

ASMUsed u32 CustomBoostDuration(Player *player, u32 currentBoostDuration) {
	//u8 *data = rodata_CustomBoostDuration[0];
	u8 addBoostDuration = Archetype_BoostDuration[std::to_underlying(player->characterArchetype)][player->level];
	//u8 controllerPort = player->input->port;
	// HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];

	if(CurrentGameMode == BattleMode || CurrentGameMode == TagMode || CurrentGameMode == EmeraldChase) {
		if(player->character != Character::E10G) {
			if(addBoostDuration >= 0x28) {
				currentBoostDuration += 0x3C;
			} else {
				currentBoostDuration += addBoostDuration;
			}
		}
	} else {
		currentBoostDuration += addBoostDuration;
		// if (exLoads.gearExLoadID == HyperHangOnEXLoad)
		// {if (hhoInfo->saturnMegadriveStatus != 2) currentBoostDuration /= 2;}
		const auto &exloadID = player->gearExload().exLoadID;
		if(player->character == Character::SuperSonic && player->extremeGear == ExtremeGear::ChaosEmerald
			&& exloadID != EXLoad::HyperSonic) {
			currentBoostDuration = 150;
		}
		if(player->isSuperCharacter(Character::Knuckles)){
			currentBoostDuration = 210;
		} else if(player->isSuperCharacter(Character::Shadow)){
			currentBoostDuration = 120;
			// if (player->supershadowboostflag) currentBoostDuration = 60;
		} else if(player->isSuperCharacter(Character::MetalSonic)){
			currentBoostDuration = 180;
		}

		switch(player->extremeGear) {
			using namespace ExtremeGear;
			case GunGear: {
				// KC: Replaced the bool variable with the one in the union used everywhere else. Must've never gotten around to fixing this on my part.
				if(player->input->toggleFaceButtons.hasAny(Buttons::X) && player->stardustspeederII_1frameboost) {
					currentBoostDuration = 1;
				}
				break;
			}
			case ERider: {
				currentBoostDuration += 20;
				if (player->characterArchetype == CharacterArchetype::Boost) currentBoostDuration += 20;
				break;
			}
			case OpaOpa: {
				currentBoostDuration /= 2;
				break;
			}
			case BlueStar2: {
				if(player->level == 0x2) {
					currentBoostDuration += 30;
				}
				break;
			}
			case ExtremeGear::TurboStar: {
				if(player->gearSpecificFlags[TurboStar::Level4]) {
					currentBoostDuration += 30;
				}
				break;
			}
			case SlideBooster: {
				if(player->characterArchetype != CharacterArchetype::Combat) {
					currentBoostDuration = 1;
				} else
					currentBoostDuration = 5;
				break;
			}
			// case Default: {
			// 	if(exloadID == EXLoad::SuperStorm) {
			// 		if(player->speed >= pSpeed(310.0f)) {
			// 			currentBoostDuration = 180;
			// 		} else
			// 			currentBoostDuration = 30;
			// 	}
			// 	break;
			// }
			case Accelerator: {
				if (player->gearExload().exLoadID == EXLoad::HyperHangOn) {
				    AcceleratorInfo &aclInfo = PlayerAcceleratorInfo[player->index];
				    if (aclInfo.isOverheated) currentBoostDuration = 0; // Kickdash if overheated
				}
				break;
			}
			default:
				break;
		}

		if(player->specialFlags.hasAny(SpecialFlags::lowBoost)) {
			currentBoostDuration = 120;
		}
	}
	return currentBoostDuration;
}
