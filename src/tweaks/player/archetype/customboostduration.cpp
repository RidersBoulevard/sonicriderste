#include "cosmetics/player/exloads.hpp"
#include "gears/turbostar.hpp"
#include "riders/gamemode.hpp"
#include "handlers/player/specialflagtweaks.hpp"

constexpr m2darray<u8, ArchetypeCount, 3> Archetype_BoostDuration = {{
		{0x14, 0x14, 0x14},// all rounder
		{0x00, 0x3c, 0x3c},// late booster
		{0x1E, 0x1E, 0x1E}, // top speed
		{0x46, 0x46, 0x00},// combat
		{0x00, 0x00, 0x00},// drift
		{0x46, 0x46, 0x46},// no type
		{0x00, 0x00, 0x00},// super sonic
		{0x00, 0x00, 0x00},// boost
		{0x00, 0x00, 0x00},// trickster
		{0x00, 0x00, 0x00},// none archetype
		{0x00, 0x00, 0x00},// Windcatcher
		{0x00, 0x00, 0x00},// Turning
		{0x00, 0x00, 0x00},// Mechanic
}};

ASMUsed u32 CustomBoostDuration(Player *player, u32 currentBoostDuration) {
	//u8 *data = rodata_CustomBoostDuration[0];
	u8 addBoostDuration = Archetype_BoostDuration[player->characterArchetype][player->level];
	//u8 controllerPort = player->input->port;
	EnabledEXLoads exLoads{};
	FetchEnabledEXLoadIDs(player, exLoads);
	// HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
	// OKGInfo *okgInfo = &PlayerOKGInfo[player->index];

	if(CurrentGameMode == BattleMode || CurrentGameMode == TagMode || CurrentGameMode == EmeraldChase) {
		if(player->character != E10G) {
			if(addBoostDuration >= 0x28) {
				currentBoostDuration += 0x3C;
			} else {
				currentBoostDuration += addBoostDuration;
			}
		}
	} else {
		currentBoostDuration += addBoostDuration;
		// if (exLoads.gearExLoadID == OllieKingGearEXLoad)
		//         {
		//             if (okgInfo->OllieBurstStatus == 0)
		//             {
		//                 currentBoostDuration = 0;
		//             } else if (okgInfo->OllieBurstStatus == 1)
		//             {
		//                 switch (player->trickRank)
		//                 {
		//                     case XRank:
		//                         currentBoostDuration = 210;
		//                         break;
		//                     case SPlusRank:
		//                         currentBoostDuration = 180;
		//                         break;
		//                     case SRank:
		//                         currentBoostDuration = 120;
		//                         break;
		//                     case APlusRank:
		//                     case ARank:
		//                         currentBoostDuration = 90;
		//                         break;
		//                     case BRank:
		//                         currentBoostDuration = 60;
		//                         break;
		//                     case CRank:
		//                         break;
		//                 }
		//                 if (player->characterArchetype == AllRounder) currentBoostDuration += 15;
		//                 if (player->characterArchetype == LateBooster || player->characterArchetype == CombatArchetype) currentBoostDuration += 30;
		//                 okgInfo->OllieBurstStatus = 0;
		//             }
		//         }
		// if (exLoads.gearExLoadID == HyperHangOnEXLoad)
		// {if (hhoInfo->saturnMegadriveStatus != 2) currentBoostDuration /= 2;}
		if(player->character == SuperSonic && exLoads.gearExLoadID != HyperSonicEXLoad) {
			currentBoostDuration = 150;
		}
		if(isSuperCharacter(*player, Knuckles)){
			currentBoostDuration = 210;
		} else if(isSuperCharacter(*player, Shadow)){
			currentBoostDuration = 60;
		}

		switch(player->extremeGear) {
			using namespace ExtremeGear;
			case HighBooster: {
				if(exLoads.gearExLoadID == StardustSpeederEXLoad) {
					if(player->input->toggleFaceButtons.hasAny(XButton) && player->fastest_superCruise) {
						currentBoostDuration = 1;
					}
				}
				break;
			}
			case ERider: {
				currentBoostDuration += 20;
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
				if(player->characterArchetype != CombatArchetype) {
					currentBoostDuration = 1;
				} else
					currentBoostDuration = 5;
				break;
			}
			case Default: {
				if(exLoads.gearExLoadID == SuperStormEXLoad) {
					if(player->speed >= pSpeed(310.0f)) {
						currentBoostDuration = 180;
					} else
						currentBoostDuration = 30;
				}
				break;
			}
			case HangOn: {
				if(exLoads.gearExLoadID == HangOnATEXLoad) {
					currentBoostDuration += player->rings;
					player->rings = 0;
				}
				break;
			}
			default:
				break;
		}

		if(player->specialFlags.hasAny(lowBoost)) {
			currentBoostDuration = 120;
		}
	}
	return currentBoostDuration;
}
