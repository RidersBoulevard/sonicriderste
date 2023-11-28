#include "hyperhangon.hpp"
#include "cosmetics/player/exloads.hpp"
#include "tweaks/player/archetype/character_archetype.hpp"
#include "lib/sound.hpp"

HHOInfo PlayerHHOInfo[8];

const f32 HHOBoostControlSpeedsBoostArchetype[4] = {pSpeed(230.0f), pSpeed(260.0f), pSpeed(275.0f), pSpeed(290.0f)};
const f32 HHOBoostControlSpeeds[4] = {pSpeed(225.0f), pSpeed(255.0f), pSpeed(270.0f), pSpeed(285.0f)};
const f32 HHO_BoostSpeeds[4] = {pSpeed(220.0f), pSpeed(250.0f), pSpeed(265.0f), pSpeed(270.0f)};

void Player_HyperHangOn(Player *player) {
    u8 controllerPort = player->input->port;
    EnabledEXLoads exLoads;
    FetchEnabledEXLoadIDs(player, &exLoads);
    HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
    if (exLoads.gearExLoadID != HyperHangOnEXLoad) return;

    if (player->state == StartLine)
    {
        hhoInfo->attackCount = 0;
        hhoInfo->trickCount = 0;
        hhoInfo->pitCount = 0;
        hhoInfo->saturnMegadriveStatus = 0;
        hhoInfo->extraType = 0;
        player->gearStats[player->level].airDrain = 10;
        // player->gearStats[0].boostSpeed = HHO_BoostSpeeds[0];
        // player->gearStats[1].boostSpeed = HHO_BoostSpeeds[1];
        // player->gearStats[2].boostSpeed = HHO_BoostSpeeds[2];
    }

    if ((hhoInfo->attackCount + hhoInfo->trickCount + hhoInfo->pitCount) >= (u8)5)
    {
        if (hhoInfo->saturnMegadriveStatus == 2) return;
        hhoInfo->attackCount = 0;
        hhoInfo->trickCount = 0;
        hhoInfo->pitCount = 0;
        hhoInfo->saturnMegadriveStatus = 1;
    }
    
    if (hhoInfo->saturnMegadriveStatus == 1) // ready state
    {
        hhoInfo->saturnMegadriveStatus = 2;
        PlayAudioFromDAT(Sound::SFX::SuperTransformation);
        if (AllCharacterArchetypes[player->character] != BoostArchetype)
        {
            player->characterArchetype = BoostArchetype;
        } else // generate a random type for boost characters
        {
            hhoInfo->extraType = (1 << lbl_RNG_Number(3));
            if (hhoInfo->extraType > PowerType) hhoInfo->extraType = SpeedType; // reset to speed type
            player->typeAttributes |= hhoInfo->extraType;
        }
    } 

    if (hhoInfo->saturnMegadriveStatus == 2) // active state
    {
        player->gearStats[player->level].airDrain = 80;
        player->gearStats[player->level].boostSpeed = HHO_BoostSpeeds[3];
        if (AllCharacterArchetypes[player->character] != BoostArchetype)
        {
            player->shortcutAirGainMultiplier = (s32)0x3E99999A;
        } else player->shortcutAirGainMultiplier = (s32)0x3E4CCCCD;
        if (AllCharacterArchetypes[player->character] != BoostArchetype)
        {
            player->unk9C8 = (u32)0x3E99999A;   
        } else player->unk9C8 = (u32)0x3E4CCCCD;
        hhoInfo->attackCount = 0;
        hhoInfo->trickCount = 0;
        hhoInfo->pitCount = 0;
    }

    if (hhoInfo->saturnMegadriveStatus == 3)
    {
        hhoInfo->saturnMegadriveStatus = 0;
        lbl_Player_BoostEndFunction(player);
        PlayAudioFromDAT(Sound::SFX::EmerlLoseType);
        // player->specialFlags &= ~noPits;
        player->gearStats[player->level].airDrain = 10;
        player->gearStats[player->level].boostSpeed = player->gearptr->levelStats[player->level].boostSpeed;
        player->gearStats[player->level].boostCost = player->gearptr->levelStats[player->level].boostCost;
        player->characterArchetype = AllCharacterArchetypes[player->character];
        player->gearStats[player->level].airDrain = player->gearptr->levelStats[player->level].passiveAirDrain;
        player->shortcutAirGainMultiplier = (s32)0x3DCCCCCD;
        player->unk9C8 = (u32)0x3DCCCCCD; // trick air gain
        player->gearStats[player->level].baseAccel = player->gearptr->acceleration;
    }
}