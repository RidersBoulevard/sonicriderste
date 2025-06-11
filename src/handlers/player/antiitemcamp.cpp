#include "antiitemcamp.hpp"
#include "mechanics/speed_shoes.hpp"
#include "lib/stdlib.hpp"
#include "riders/gamemode.hpp"
#include "gears/hangon.hpp"

ASMUsed ItemID AntiItemCampHandler(Player &player, ItemID item) {
	using enum ItemID;
	if (CurrentGameMode == TimeTrial) {
		player.last_itemID_lap = player.currentLap;
		return item;
	}
	if (player.state == PlayerState::StartLine) {
		return SpeedShoes;
	}
	if ((player.extremeGear == ExtremeGear::SuperHangOn && player.gearSpecificFlags[SuperHangOn::Level4]) && player.last_itemBox_random) {
		item = SpeedShoes;
	}

	if (const auto &exloadID = player.gearExload().exLoadID;
		exloadID == EXLoad::TheBeast
		&& player.state == PlayerState::Run
		&& (item == ThirtyAir || item == FiftyAir || item == HundredAir)
	)
	{
		switch (lbl_RNG_Number(3)) {
			case 1:
				item = TenRings;
				break;
			case 2:
				item = TwentyRings;
				break;
			case 3:
				item = ThirtyRings;
				break;
			default:
				item = SpeedShoes;
				break;
		}
	}

	// AirTankInfo *ATInfo = &PlayerAirTankInfo[player.index];
	// if (player.extremeGear == AirTank && ATInfo->itemUsed == 0) {

	//     // experimental, air tank item storage
	//     ATInfo->itemStorage[0] = item;
	//     // if (ATInfo->stoSize < 3)
	//     // {ATInfo->itemStorage[ATInfo->stoSize++] = item;}
	//     return -1;
	// } else if (player.extremeGear == AirTank && ATInfo->itemUsed == 1)
	// {
	//     ATInfo->itemUsed = 0;
	//     return item;
	// }

	// Fix for new speed shoe RNG, items will go OOB if the chance is zero, so this covers that case
	if (item > FiveRings) {
		switch (lbl_RNG_Number(3)) {
			case 0:
				item = TenRings;
				break;
			case 1:
				item = ThirtyAir;
				break;
			default:
				item = FiveRings;
				break;
		}
	}


	if (player.last_itemID_lap != player.currentLap) {
		player.current_itemID = player.last_itemID;
	} else if (player.last_itemID == player.current_itemID) {
		item = SpeedShoes;
	} else {
		player.current_itemID = player.last_itemID;
	}

	/*if(item == Magnet
		&& player.rings == player.max_rings()
		&& !player.specialFlags.hasAny(SpecialFlags::ringGear)) {
		item = MaxAir;
	}*/

	player.last_itemID_lap = player.currentLap; // Sets last item so that the previous if-block will give speed shoes on the same box until a new box is used
	get_speed_shoes_data(player).last_box_speed_shoes = item == SpeedShoes;

    if (item != SpeedShoes) get_speed_shoes_data(player).cannot_use_speed_shoes = false;
    // if you picked an item that isn't a shoe, you can use speed shoes.
    // if it is a shoe, don't do anything

	return item;
}
