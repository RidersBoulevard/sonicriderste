#include "speed_shoes.hpp"
#include "globalDefs.hpp"
#include "handlers/player/ItemBox.hpp"
#include "mechanics/magneticimpulse.hpp"

std::array<speed_shoes_data, MaxPlayerCount> player_speed_shoes{};

[[gnu::hot]]
void Player_HandleSpeedShoes(Player &player) {
	if(!player.isRealPlayer()){
		return;
	}
	auto &speed_shoes = player_speed_shoes[player.index];

    // Below is the code for handling "tap shoulder button to speed shoe" (see handlers.cpp)

 //    if (player.state == PlayerState::Death) {
 //        speed_shoes.reset({}, 0);
 //        return;
 //    }
 //    speed_shoes.tick_timer();
 //    if(!speed_shoes.has_speed_shoes || player.state != PlayerState::Cruise) {
 //        speed_shoes.last_button = {};
 //        return;
 //    }
 //    const auto &toggleFaceButtons = player.input->toggleFaceButtons;
	// if(!toggleFaceButtons.hasAny(speed_shoes_data::valid_buttons)) {
	// 	return;
	// }
	// if(speed_shoes.double_tap_timer == 0) {
	// 	speed_shoes.reset(toggleFaceButtons);
	//     if (player.driftDashFrames > 0) { // If drifting, cut the activation window in half
	//         speed_shoes.double_tap_timer /= 2;
	//     }
	// 	return;
	// }
 //    if (speed_shoes.last_button.operator<=>(toggleFaceButtons) != std::strong_ordering::equal) { // Todo: Figure out why normal == is ambiguous
 //        speed_shoes.reset(toggleFaceButtons);
 //        return;
 //    }

    // if you get a speed shoe, set that here

    // if (speed_shoes.last_box_speed_shoes) return;
    // speed_shoes.last_box_speed_shoes = true; // just got speed shoes, so prevent this next time.
    if (speed_shoes.cannot_use_speed_shoes) return;
    Sound::PlaySound(Sound::ID::IDKSFX, 0x3d);
	Player_TriggerSpeedShoes(player);
    speed_shoes.reset({}, 0);
}

void Player_TriggerSpeedShoes(Player &player) {
	auto &speed_shoes = player_speed_shoes[player.index];

	Player_MISpeedShoes(player);
	ApplyCameraPreset(player.index, 0xa);
	ItemBox_SpeedShoes_PlayADX();
	speed_shoes.double_tap_timer = 0;
	speed_shoes.has_speed_shoes = false;
    speed_shoes.cannot_use_speed_shoes = true; // disables using speed shoes until a new, shoeless box is picked

	if constexpr(enable_speed_shoes_recolor) {
		for(auto &material : gpsaObject_Player[player.index]->materials()) {
			auto &[r, g, b] = material.materialData->color.diffuse;
			b = g = 1.f;
		}
	}
}
