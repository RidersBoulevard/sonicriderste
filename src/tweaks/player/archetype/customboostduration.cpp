#include "context.hpp"
#include "cosmetics/player/exloads.hpp"

constexpr u8 Archetype_BoostDuration[ArchetypeCount][3] = {
		{0x14, 0x14, 0x14}, // all rounder
		{0x00, 0x3c, 0x3c}, // late booster
		{0x28, 0x28, 0x28}, // top speed
		{0x46, 0x46, 0x00}, // combat
		{0x00, 0x00, 0x00}, // drift
		{0x46, 0x46, 0x46}, // no type
		{0x00, 0x00, 0x00}, // super sonic
		{0x00, 0x00, 0x00}, // boost
		{0x00, 0x00, 0x00}, // trickster
		{0x00, 0x00, 0x00}, // none archetype
};

ASMUsed u32 CustomBoostDuration(Player *player, u32 currentBoostDuration) {
    //u8 *data = rodata_CustomBoostDuration[0];
    u8 addBoostDuration = Archetype_BoostDuration[player->characterArchetype][player->level];
    //u8 controllerPort = player->input->port;
    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(player, exLoads);
    // struct HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
    // struct OKGInfo *okgInfo = &PlayerOKGInfo[player->index];

    if (CurrentGameMode == BattleMode || CurrentGameMode == TagMode || CurrentGameMode == EmeraldChase)
    {
        if (player->character != E10G)
        {
            if (addBoostDuration >= 0x28)
            {
                currentBoostDuration += 0x3C;
            }
            else
            {
                currentBoostDuration += addBoostDuration;
            }
        }
    }
    else
    {
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
        if (player->character == SuperSonic && exLoads.gearExLoadID != HyperSonicEXLoad) {
            currentBoostDuration = 150;
        }
		// todo replace with new super check
        u16 gearCharacter = (u16)((player->character << 8) | player->extremeGear);
        if (gearCharacter == 0x212) // super knuckles
        {
            currentBoostDuration = 210;
        }
        else if (gearCharacter == 0xA12) // super shadow
        {
            currentBoostDuration = 60;
        }

        switch (player->extremeGear) {
            case HighBooster:
            {
                if (exLoads.gearExLoadID == StardustSpeederEXLoad)
                {
                    if ((player->input->toggleFaceButtons & XButton) && (player->fastest_superCruise == TRUE)) 
                    {currentBoostDuration = 1;}
                }
                break;
            }
            case ERider:
            {
                currentBoostDuration += 20;
                break;
            }
            case OpaOpa:
            {
                currentBoostDuration /= 2;
                break;
            }
            case BlueStar2:
            {
                if (player->level == 0x2)
                {
                    currentBoostDuration += 30;
                }
                break;
            }
            case TurboStar:
            {
                if (player->level4 == 1)
                {
                    currentBoostDuration += 30;
                }
                break;
            }
            case SlideBooster:
            {
                if (player->characterArchetype != CombatArchetype) {
                    currentBoostDuration = 1;
                } else currentBoostDuration = 5;
                break;
            }
			default:
				break;
        }

        if (player->specialFlags & lowBoost)
        {
            currentBoostDuration = 120;
        }
    }
    return currentBoostDuration;
}
