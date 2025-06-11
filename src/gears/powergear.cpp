#include "powergear.hpp"
#include "optional"

void Player_PowerGear(Player *player) {
    // if (player->extremeGear != PowerGear) return;
	std::optional<s32> archetypeDriftCostDiff; // Todo: give this a better name
    if (player->characterArchetype == CharacterArchetype::Drift && player->extremeGear != ExtremeGear::TheCrazy && player->extremeGear != ExtremeGear::HangOn){
		archetypeDriftCostDiff = 90;
    } else if (player->extremeGear == ExtremeGear::PowerGear) {
	   archetypeDriftCostDiff = 95;
	}
	if(archetypeDriftCostDiff.has_value()){
	   auto &driftCost = player->gearStats[player->level].driftCost;
	   const auto &driftingAirCost = player->gearptr->levelStats[player->level].driftingAirCost;

	   driftCost = (driftingAirCost * *archetypeDriftCostDiff) / 100;
	}
}