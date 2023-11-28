#include "zbutton.hpp"
#include "lib/stdlib.hpp"

// Z Button Out Of Turb Retracts Air/Rings

inline void ZTurbulenceRetractAir(Player *player) {
    if (player->specialFlags.hasAny(ringGear)) {
	    // s32 air = player->currentAir - 3300; // 3 rings
	    s32 air = player->currentAir - (player->gearStats[player->level].maxAir * 3) / 100; // 3 rings
        player->currentAir = clamp(air, 0);
        
    } else {
	    s32 air = player->currentAir - 10000; // 10 air
        if (air > -100000) player->currentAir = air;
    }
}

ASMUsed void Player_ZTurbulenceRetractAir(Player *player) {
    if (player->unk104C != 1 && player->ignoreTurbulence) {
        // turbulence was left via clutch button
        ZTurbulenceRetractAir(player);
    }
    
    // else, turbulence was ridden all the way to the end

}

ASMUsed void Player_SetInvulnerabilityFromZTurbulence(Player *player) {
    if (player->previousState == TurbulenceRide && player->unk105D == 1) 
    // turbulence was left via L and R exit
    {
        player->tornadoInvulnerabilityTimer = 30;
        const Player &turbPlayer = players[player->closestTurbulenceIndex];
        if (turbPlayer.extremeGear == ExtremeGear::SlideBooster) ZTurbulenceRetractAir(player);
    }
}

void Player_IgnoreTurbulenceHandler(Player *player) {
    Controller *controls = player->input;
    const auto &holdButtons = controls->holdFaceButtons;
    const auto &toggleButtons = controls->toggleFaceButtons;
    const bool isZButton = holdButtons.hasAny(ZButton);
    const bool isYButton = toggleButtons.hasAny(YButton);

	const bool updatedYButton = player->y_toggle ^= isYButton;
    if (player->statusEffectFlags.hasAny(BallAndChainStatus)) player->ignoreTurbulence = true; //  ball and chain disables turb
    else player->ignoreTurbulence = isZButton ^ updatedYButton;
}