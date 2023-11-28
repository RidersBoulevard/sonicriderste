#include "beginner.hpp"

constexpr m2darray<s32, 3, 3> Beginner_AirThreshholds = {{
        {99000, 75000, 50000},
        {148500, 112500, 75000},
        {198000, 150000, 100000},
}};

constexpr m2darray<f32, 3, 4> Beginner_BoostSpeeds = {{
        {pSpeed(220.0f), pSpeed(210.0f), pSpeed(205.0f), pSpeed(185.0f)},
        {pSpeed(250.0f), pSpeed(240.0f), pSpeed(235.0f), pSpeed(215.0f)},
        {pSpeed(270.0f), pSpeed(260.0f), pSpeed(255.0f), pSpeed(235.0f)},
}};

void Player_BeginnerOmegaBoost(Player *player) {
	if(player->extremeGear != ExtremeGear::Beginner) { return; }
	if(player->movementFlags.hasAny(static_cast<MovementFlags>(0x0400))) { return; }

	f32 boostSpeed;
	if(player->currentAir > Beginner_AirThreshholds[player->level][0]) {
		boostSpeed = Beginner_BoostSpeeds[player->level][0];
	} else if(player->currentAir > Beginner_AirThreshholds[player->level][1]) {
		boostSpeed = Beginner_BoostSpeeds[player->level][1];
	} else if(player->currentAir > Beginner_AirThreshholds[player->level][2]) {
		boostSpeed = Beginner_BoostSpeeds[player->level][2];
	} else {
		boostSpeed = Beginner_BoostSpeeds[player->level][3];
	}
	player->gearStats[player->level].boostSpeed = boostSpeed;
}

void Player_CheckBeginnerAirOut(Player *player) {
	if(!(player->extremeGear == ExtremeGear::Beginner)) { return; }
	if(player->currentAir >= 500) { return; }
	player->currentAir = 500;
}