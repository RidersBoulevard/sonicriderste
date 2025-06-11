#include "riders/gamemode.hpp"
#include "riders/player.hpp"
#include "riders/object.hpp"
#include "itemrngchange.hpp"

constinit auto ItemRNGChange_level3AirGearRNG = DefaultItemRNGChange_level3AirGearRNG;

constinit auto ItemRNGChange_RingGearRNG = DefaultItemRNGChange_RingGearRNG;

ASMUsed const void *ItemRNGChange(const Player *player, const u8 * const rngChances, const u32 currentItem) {
	if(currentItem == 0) {
		return rngChances;
	}
	if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix) [[unlikely]] {
		return rngChances;
	}
	if (player->specialFlags.hasAny(SpecialFlags::ringGear)) {
		// Chances of speed shoes scale with ring amount on a ring gear, any position
		switch (player->rings) {
			case 0 ... 29:
				ItemRNGChange_RingGearRNG[6] = 0;
				break;
			case 30 ... 40:
				ItemRNGChange_RingGearRNG[6] = 10;
				break;
			case 41 ... 60:
				ItemRNGChange_RingGearRNG[6] = 15;
				break;
			case 61 ... 79:
				ItemRNGChange_RingGearRNG[6] = 20;
				break;
			case 80 ... 1000:
				ItemRNGChange_RingGearRNG[6] = 30;
				break;
		}
	}
	// Set magnet chance to zero in first.
	if (player->placement == 0) {
		ItemRNGChange_RingGearRNG[7]      = 0;
		ItemRNGChange_level3AirGearRNG[7] = 0;
	}

	if(player->placement != 0) {
		const u32 behindPlacement	      = player->placement - 1;
		const Player *player2		      = players.data();

		while(player2->placement != behindPlacement) {
			player2++;
		}

		// Handle magnet, 2nd place only
		if (player->currentLap == 1) {
			ItemRNGChange_RingGearRNG[7]      = 0;
			ItemRNGChange_level3AirGearRNG[7] = 0;
		} else {
			// Chance of magnet scales with ring amount
			auto rings = player->rings;

			if(player->extremeGear == ExtremeGear::CoverP) {
				rings /= 3; // Scale triple ring pickup back to normal for this check
			}

			switch (rings) {
				case 0 ... 29:
					ItemRNGChange_RingGearRNG[7]		= 20;
					ItemRNGChange_level3AirGearRNG[7]	= 20;
				break;
				case 30 ... 40:
					ItemRNGChange_RingGearRNG[7]		= 15;
					ItemRNGChange_level3AirGearRNG[7]	= 15;
				break;
				case 41 ... 60:
					ItemRNGChange_RingGearRNG[7]		= 10;
					ItemRNGChange_level3AirGearRNG[7]	= 10;
				break;
				case 61 ... 79:
					ItemRNGChange_RingGearRNG[7]		= 5;
					ItemRNGChange_level3AirGearRNG[7]	= 5;
				break;
				case 80 ... 1000:
					ItemRNGChange_RingGearRNG[7]		= 0;
					ItemRNGChange_level3AirGearRNG[7]	= 0;
				break;
			}
		}

		if(const f32 distance = std::abs(player->stageProgress - player2->stageProgress); distance > 3000) {
			return rngChances + 0xB;
		}
	}

	if(!player->specialFlags.hasAny(SpecialFlags::ringGear)) {
		if(player->level >= 2 && player->placement == 1) {
			return ItemRNGChange_level3AirGearRNG.data();
		}
	} else {
		if(player->placement_counter <= 1) {
			return ItemRNGChange_RingGearRNG.data();
		}
	}
	return rngChances;
}

void RNGChangesReset(const Player *player) {
	if (player->state == PlayerState::StartLine) {
		ItemRNGChange_RingGearRNG = DefaultItemRNGChange_RingGearRNG;
		ItemRNGChange_level3AirGearRNG = DefaultItemRNGChange_level3AirGearRNG;
	}

}