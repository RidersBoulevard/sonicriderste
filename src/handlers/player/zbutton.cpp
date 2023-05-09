#include "zbutton.hpp"

// Z Button Out Of Turb Retracts Air/Rings

inline void ZTurbulenceRetractAir(Player *player) {
    if (player->specialFlags & ringGear) {
	    s32 air = player->currentAir - 3300; // 3 rings
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
    player->tornadoInvulnerabilityTimer = 30;
}

void Player_IgnoreTurbulenceHandler(Player *player) {
    Controller *controls = player->input;
    BOOL updatedYButton;
    u32 holdButtons = controls->holdFaceButtons;
    u32 toggleButtons = controls->toggleFaceButtons;
    BOOL isZButton = (holdButtons & ZButton) ? TRUE : FALSE;
    BOOL isYButton = (toggleButtons & YButton) ? TRUE : FALSE;

    updatedYButton = player->y_toggle ^= isYButton;
    player->ignoreTurbulence = isZButton ^ updatedYButton;
}