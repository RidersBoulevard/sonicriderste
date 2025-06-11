#pragma once

#include "globalDefs.hpp"
#include "riders/player.hpp"

static constexpr bool enable_speed_shoes_recolor = false; // todo: fix recoloring then remove this so it always recolors

struct speed_shoes_data {
	static constexpr u8 double_tap_time = 20;
	static constexpr Flag<Buttons> valid_buttons = Buttons::L | Buttons::R | Buttons::Z;

	[[no_unique_address]] u8 double_tap_timer = 0;
	[[no_unique_address]] Flag<Buttons> last_button;
	[[no_unique_address]] bool has_speed_shoes = false;
	[[no_unique_address]] bool last_box_speed_shoes = false;
	[[no_unique_address]] bool cannot_use_speed_shoes = false;

	void tick_timer() {
		if(double_tap_timer <= 0) { return; }
		double_tap_timer--;
	}

	void reset(const Flag<Buttons> &buttons = {}, const u8 &time = double_tap_time) {
		double_tap_timer = time;
		last_button = buttons;
	}
};

extern std::array<speed_shoes_data, MaxPlayerCount> player_speed_shoes;

inline speed_shoes_data &get_speed_shoes_data(const Player &player){
	return player_speed_shoes[player.index];
}

void Player_HandleSpeedShoes(Player &player);

/**
* Triggers speed shoes effect and removes speed shoes from player.
* @note Does not check if player has speed shoes
*/
void Player_TriggerSpeedShoes(Player &player);
