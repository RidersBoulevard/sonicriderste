#include "zbutton.hpp"
#include "lib/stdlib.hpp"

std::array<u8, MaxPlayerCount> PlayerZButtonHeldTimers;

// Z Button Out Of Turb Retracts Air/Rings

inline void ZTurbulenceRetractAir(Player *player){
	if(player->specialFlags.hasAny(SpecialFlags::ringGear)){
		// s32 air = player->currentAir - 3300; // 3 rings
		s32 air = player->currentAir - (player->gearStats[player->level].maxAir * 3) / 100; // 3 rings
		player->currentAir = clamp(air);

	}else{
		s32 air = player->currentAir - 10000; // 10 air
		if(air > -100000){ player->currentAir = air; }
	}
}

ASMUsed void Player_ZTurbulenceRetractAir(Player *player){
	if(player->unk104C != 1 && player->ignoreTurbulence){
		// turbulence was left via clutch button
		ZTurbulenceRetractAir(player);
	}

	// else, turbulence was ridden all the way to the end

}

ASMUsed void Player_SetInvulnerabilityFromZTurbulence(Player *player){
	if(player->previousState == PlayerState::TurbulenceRide && player->unk105D == 1){ // turbulence was left via L and R exit
		player->tornadoInvulnerabilityTimer = 30;
		const Player &turbPlayer = players[player->closestTurbulenceIndex];
		if(turbPlayer.extremeGear == ExtremeGear::SlideBooster){
			ZTurbulenceRetractAir(player);
		}
	}
}

void Player_IgnoreTurbulenceHandler(Player *player){
	Controller *controls = player->input;
	const auto &holdButtons = controls->holdFaceButtons;
	const auto &toggleButtons = controls->toggleFaceButtons;
	const bool isZButton = holdButtons.hasAny(Buttons::Z);
	const bool isYButton = toggleButtons.hasAny(Buttons::Y);

	const bool updatedYButton = player->y_toggle ^= isYButton;
    if (player->statusEffectFlags.hasAny(PlayerStatus::BallAndChainStatus)) player->ignoreTurbulence = true; //  ball and chain disables turb
    else player->ignoreTurbulence = isZButton ^ updatedYButton;
	
    // constexpr auto ZTOGGLE_FRAMES = 15;

    // auto &zHeldTimer = PlayerZButtonHeldTimers[player->index];

    // if (player->input->toggleFaceButtons.hasAny(Buttons::Z)) {
    //     // reset timer and toggle turb ignore
    //     zHeldTimer = 0;
    //     player->ignoreTurbulence = !player->ignoreTurbulence;
	// 	player->y_toggle = player->ignoreTurbulence;
    // }

    // if (player->input->holdFaceButtons.hasAny(Buttons::Z)) {
    //     // increment timer while button is held
    //     if (zHeldTimer != UINT8_MAX) zHeldTimer++;
	// 	if (zHeldTimer > ZTOGGLE_FRAMES && zHeldTimer != 0) player->y_toggle = false;
    // } else {
    //     // check if z was held for less than ZTOGGLE_FRAMES, then it's a toggle,
    //     // and we don't have to worry about resetting ignore turb
    //     const bool isToggle = zHeldTimer < ZTOGGLE_FRAMES && zHeldTimer != 0;
    //     if (!isToggle) {
    //         player->ignoreTurbulence = false;
    //     }
    // }
}