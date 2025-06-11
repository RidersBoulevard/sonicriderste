#include "itemrngchange.hpp"
#include "riders/gamemode.hpp"
#include "riders/object.hpp"
#include "riders/player.hpp"

#include <gears/gambler.hpp>
#include <gears/hangon.hpp>
#include <gears/turbostar.hpp>
#include <mechanics/speed_shoes.hpp>

//constinit auto ItemRNGChange_level3AirGearRNG = DefaultItemRNGChange_level3AirGearRNG;
//constinit auto ItemRNGChange_RingGearRNG = DefaultItemRNGChange_RingGearRNG;

void handle_magnet_chance(ItemRNGChange_t &rng, const Player &player) {
	using enum ItemID;
	auto &magnet_chance = rng[std::to_underlying(Magnet)];
	if (player.placement == 0) { // Set magnet chance to zero in first.
		magnet_chance = 0;
		return;
	}
	if (player.currentLap == 1) { // Handle magnet, 2nd place only
		magnet_chance = 0;
		return;
	}
	// Chance of magnet scales with ring amount
	auto rings = player.rings;

	if(player.extremeGear == ExtremeGear::CoverP) {
		rings /= 2; // Scale double ring pickup back to normal for this check
	}

	switch (rings) {
		case 0 ... 29:
			magnet_chance = 20;
			break;
		case 30 ... 40:
			magnet_chance = 15;
			break;
		case 41 ... 60:
			magnet_chance = 10;
			break;
		case 61 ... 79:
			magnet_chance = 5;
			break;
		case 80 ... 1000:
			magnet_chance = 0;
			break;
		default:
			break;
	}
}

//constexpr auto test = std::bit_cast<s32>(RuleSettings{.max_lap = 0x7f});
//static_assert(test == 0x7f);

void handle_speed_shoes_chance(ItemRNGChange_t &rng, const Player &player) {
	using enum ItemID;
	auto &speed_shoes_chance = rng[std::to_underlying(SpeedShoes)];
	// if(get_speed_shoes_data(player).last_box_speed_shoes) {
	// 	speed_shoes_chance = 0;
	// }

    if(get_speed_shoes_data(player).cannot_use_speed_shoes) {
        speed_shoes_chance = 0;
    }

    switch (player.extremeGear) {
        using namespace ExtremeGear;
        case ExtremeGear::Gambler:
            if (player.gearSpecificFlags[Gambler::Level4]) break;
            if (player.level == 2) speed_shoes_chance = 0;
        case ExtremeGear::TurboStar:
            if (player.gearSpecificFlags[TurboStar::Level4]) break;
            if (player.level == 2) speed_shoes_chance = 0;
        case ExtremeGear::SuperHangOn:
            if (player.gearSpecificFlags[SuperHangOn::Level4]) break;
            if (player.level == 2) speed_shoes_chance = 0;
        default: break;
    }

	if(InGamePlayerCount == 1) { return; }
    // && player.currentLap == ruleSettings.max_lap
	if(player.placement == 0 && !player.specialFlags.hasAny(SpecialFlags::ringGear)) {
		speed_shoes_chance = 0;
	}
}

void handle_speed_shoes_ring_gear_chance(ItemRNGChange_t &rng, const Player &player) {
	using enum ItemID;
	// Chances of speed shoes scale with ring amount on a ring gear, any position
	auto &speed_shoes_chance = rng[std::to_underlying(SpeedShoes)];
	switch (player.rings) {
		case 0 ... 20:
			speed_shoes_chance = 0;
			break;
		case 21 ... 40:
			speed_shoes_chance = 10;
			break;
		case 41 ... 60:
			speed_shoes_chance = 15;
			break;
		case 61 ... 79:
			speed_shoes_chance = 20;
			break;
		case 80 ... 1000:
			speed_shoes_chance = 30;
			break;
		default:
			break;
	}

	handle_speed_shoes_chance(rng, player);
}

void handle_air_gear_ring_cap(ItemRNGChange_t &rng, const Player &player) {
	using enum ItemID;

	if(player.rings < player.max_rings()) {
		return;
	}

	auto air_index = std::to_underlying(ThirtyAir);
	auto ring_index = std::to_underlying(TenRings);

	for(int i = 0; i < 3; ++i) {
		rng[air_index++] += rng[ring_index]; // Copy ring box chances to air box
		rng[ring_index++] = 0; // Clear out air chance
	}

	rng[std::to_underlying(HundredAir)] += rng[std::to_underlying(Magnet)];
	rng[std::to_underlying(Magnet)] = 0;
}

const ItemRNGChange_t& handle_rng(ItemRNGChange_t &rng, const Player &player) {

}

// rngChances = lbl_001C7018 + 0xC?
ASMUsed const ItemRNGChange_t& ItemRNGChange(const Player &player, const ItemRNGChange_t &rngChances, const u32 /*currentItem*/) {
	/*if(currentItem == 0) {
		return rngChances;
	}*/
	if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix) [[unlikely]] {
		return rngChances;
	}

	using enum ItemID;
	static ItemRNGChange_t rng{};

	std::ranges::copy(rngChances, rng.begin());


	if (player.specialFlags.hasAny(SpecialFlags::ringGear)) {
		rng = DefaultItemRNGChange_RingGearRNG;

		handle_speed_shoes_ring_gear_chance(rng, player);

		handle_magnet_chance(rng, player);

		if(player.placement_counter <= 1) {
			return rng;
		}
	} else if(player.level >= 2) {
		rng = DefaultItemRNGChange_level3AirGearRNG;

		handle_speed_shoes_chance(rng, player);

		handle_magnet_chance(rng, player);

		handle_air_gear_ring_cap(rng, player);

		if(player.placement == 1) {
			return rng;
		}
	} else {
		const Player *player2 = nullptr;
		if(player.placement != 0) {
			const u32 behindPlacement = player.placement - 1;
			player2 = players.data();

			while(player2->placement != behindPlacement) {
				player2++;
			}
		}

		if(player2 != nullptr) {
			if(const f32 distance = std::abs(player.stageProgress - player2->stageProgress); distance > 3000) {
				std::copy(rngChances.begin() + ItemBoxCount, rngChances.end() + ItemBoxCount, rng.begin());
			}
		}

		handle_speed_shoes_chance(rng, player);

		handle_air_gear_ring_cap(rng, player);

		return rng;
	}

	return rng;
}

/*void RNGChangesReset(const Player &player) {
	if (player.state == PlayerState::StartLine) {
		ItemRNGChange_RingGearRNG = DefaultItemRNGChange_RingGearRNG;
		ItemRNGChange_level3AirGearRNG = DefaultItemRNGChange_level3AirGearRNG;
	}
}*/