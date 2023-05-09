#include "hangon.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

constexpr GearLevelStats Level1 = {
		100000, // max air
		8, // air drain
		10, // drift cost
		0x157C, // boost cost
		0x1130, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(270) // boost speed, unused
};
constexpr GearLevelStats Level2 = {
		133000, // max air
		18, // air drain
		50, // drift cost
		0x32C8, // boost cost
		0x1964, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(270) // boost speed, unused
};
constexpr GearLevelStats Level3 = {
		200000, // max air
		63, // air drain
		203, // drift cost
		0x7530, // boost cost
		0x4E20, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(300) // boost speed, unused
};

inline void Player_HangOn_UpdateStats(Player *player, const GearLevelStats *stats) {
    player->gearStats[player->level].maxAir = stats->maxAir;
    player->gearStats[player->level].driftCost = stats->driftingAirCost;
    player->gearStats[player->level].boostCost = stats->boostCost;
    player->gearStats[player->level].tornadoCost = stats->tornadoCost;
    // player->gearStats[player->level].boostSpeed = HangOn_Speeds[player->level][1];
    if (player->characterArchetype == BoostArchetype)
    {
        player->gearStats[player->level].boostSpeed = HangOn_Speeds[player->level][1] + BoostArchetypeBoostSpeeds[player->level];
    } else player->gearStats[player->level].boostSpeed = HangOn_Speeds[player->level][1];
    // player->gearStats[0].topSpeed = pSpeed(185); // level one top speed set manually
    player->gearStats[1].driftDashSpeed = pSpeed(100); // level two drift dash set manually
    player->gearStats[player->level].topSpeed = HangOn_Speeds[player->level][0];
    if (player->state == Cruise) player->speedCap = HangOn_Speeds[player->level][0];
    player->gearStats[player->level].baseTopSpeed = HangOn_Speeds[player->level][0];
}

void Player_HangOn(Player *player) {
    if (player->extremeGear != HangOn) return;

    if (player->level == 2 && player->movementFlags & 0x0400)  // level 3 has slightly less air drain while boosting
    {
        player->gearStats[2].airDrain = 52;
    }
    else {
        switch (player->level) {
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

    if (player->state == StartLine) {
        player->level = 0;
        Player_HangOn_UpdateStats(player, &Level1);
        return;
    }
    
    if (player->hangOn_infiniteRingTimer > 0) {
        player->level = 0;
        Player_HangOn_UpdateStats(player, &Level1);
        player->level4 = 1;
        return;
    } else player->level4 = 0;

    if (player->input->toggleFaceButtons & DPadLeft) {
        // Gear 1
        if (player->level != 0) PlayAudioFromDAT(Sound::SFX::HOTrans1);
        player->level = 0;
        Player_HangOn_UpdateStats(player, &Level1);
    } else if (player->input->toggleFaceButtons & DPadUp) {
        // Gear 2
        if (player->level != 1) PlayAudioFromDAT(Sound::SFX::HOTrans2);
        if (player->level == 0 && player->movementFlags & boosting)
        {player->boostDuration /= 2;}
        
        player->level = 1;
        Player_HangOn_UpdateStats(player, &Level2);
    } else if (player->input->toggleFaceButtons & DPadRight) {
        // Gear 3
        if (player->level != 2) PlayAudioFromDAT(Sound::SFX::HOTrans3);
        if (player->level < 2 && player->movementFlags & boosting)
        {player->boostDuration /= 3;}
        
        player->level = 2;
        Player_HangOn_UpdateStats(player, &Level3);
    }
}