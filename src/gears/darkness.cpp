#include "darkness.hpp"
#include "cosmetics/player/supersonicaura.hpp"

void Player_Darkness(Player &player) {
	if (!DebugMenu_CheckOption(DebugMenuOptions::PTRMode)) return;
	if (player.extremeGear != ExtremeGear::Darkness) { return; }
    if (player.gearExload().exLoadID != EXLoad::TheProfessional) {
        // player.gearStats[player.level].driftCost = (player.gearptr->levelStats[player.level].driftingAirCost * 80) / 100; // 20% lower cost
        player.gearptr->jumpChargeCostMultiplier = (player.gearptr->jumpChargeCostMultiplier * 70.0f) / 100.0f; // 30% lower cost. ASM reads this, weirdly enough...
        player.gearStats[player.level].boostCost = (player.gearptr->levelStats[player.level].boostCost * 90) / 100; // 10% lower cost
        player.gearStats[player.level].tornadoCost = (player.gearptr->levelStats[player.level].tornadoCost * 80) / 100; // 20% lower cost
        return;
    }

    player.gearptr->jumpChargeCostMultiplier = 2.0f; // Default value.

	if (if_any(player.state, std::is_eq, PlayerState::RailGrind, PlayerState::Fly)) {
		if (player.DarknessFrameCounter % 24 == 0) { player.rings++; }
		player.DarknessFrameCounter++;
		SuperSonicAuraCXX(player); // visible aura for this mode
	}

	if (player.level == 2 && player.rings > 60) {
		if (player.state == PlayerState::Cruise && !(player.input->holdFaceButtons.hasAny(Buttons::A) && player.jumpCharge > 0)) {
			if (player.DarknessFrameCounter % 30 == 0) { player.rings -= 1; }
			player.DarknessFrameCounter++;
		}
		const auto ringsOverSixty = clamp(static_cast<s32>(player.rings - 60), 0, 40);
		player.shortcutAirGainMultiplier = 1.0f + (static_cast<f32>(ringsOverSixty) * 0.00375f);
	} else {
		player.shortcutAirGainMultiplier = 1.0f;
	}

}