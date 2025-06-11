#include "fastest.hpp"

constexpr m2darray<f32, 2, 3> Fastest_SuperCruiseSpeeds = {{
        // level 1      level 2      level 3
        {pSpeed(240), pSpeed(250), pSpeed(260)},// normal
        {pSpeed(245), pSpeed(255), pSpeed(265)},// late booster and combat archetype
}};

void Player_Fastest(Player &player) {
	if(player.extremeGear != ExtremeGear::Fastest) { return; }

	if(player.currentAir > 0 && player.state != PlayerState::Run && player.input->holdFaceButtons.hasAny(Buttons::B, Buttons::X)) {
		if(player.state != PlayerState::Cruise) { return; }
		const bool isLateBooster = player.characterArchetype == CharacterArchetype::LateBooster;
		const bool isCombat = player.characterArchetype == CharacterArchetype::Combat;
		if(player.input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X)) {
			if(isCombat) {
				player.currentAir -= 4000;// 20% reduction
			} else if(isLateBooster) {
				player.currentAir -= 3500;// 30% reduction
			} else {
				player.currentAir -= 5000;
			}
		}

		const bool isBerserker = player.fastest_timer >= 90;
		if(!isBerserker) { player.fastest_timer += 1; }

		player.fastest_superCruise = true;
		const f32 newSpeed = player.speed + 0.0027006172839506F;// per frame

		const auto isBoostArchetype = player.characterArchetype == CharacterArchetype::Boost ? 1U : 0U;
		const f32 cruiseSpeed = Fastest_SuperCruiseSpeeds[isBoostArchetype][player.level];
		// cruiseSpeed = Fastest_SuperCruiseSpeeds[(isCombat | isLateBooster)][player.level];
		if(newSpeed < cruiseSpeed) { player.speed = newSpeed; }
		player.speedCap = cruiseSpeed;

		if(isCombat) {
			player.currentAir -= 320;// 20% reduction
		} else if(isLateBooster) {
			player.currentAir -= 280;// 30% reduction
		} else {
			player.currentAir -= 400;
		}

		if(isBerserker) { player.specialFlags |= SpecialFlags::berserkerEffect; }
	} else {
		player.fastest_superCruise = false;
		player.fastest_timer = 0;
		player.specialFlags &= ~SpecialFlags::berserkerEffect;
		if(player.state == PlayerState::Cruise) {
			player.attackProperties = nullptr;
			player.speedCap = player.gearStats[player.level].topSpeed;
		}
	}
}