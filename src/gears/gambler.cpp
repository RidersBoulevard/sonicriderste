#include "gambler.hpp"
#include "riders/player.hpp"
#include "lib/stdlib.hpp"

ASMUsed void Gambler_GainTypeOnLevelup(Player *player) {
	return;
	if(player->extremeGear != ExtremeGear::Gambler) { return; }

	if(player->character == Character::E10G) {
		if(player->level == 1) {// level up to level 2, gain random type
			player->typeAttributes = static_cast<Type>(1 << lbl_RNG_Number(3));
		} else if(player->level == 2) {
			if(!player->gearSpecificFlags[Gambler::Level4]) {                           // level up to level 3
				Flag<Type> newType = player->typeAttributes;
				newType = newType << static_cast<Type>(1);// rotate to next type
				if(newType > Type::Power) { newType = Type::Speed; }// reset to speed type
				player->typeAttributes = newType;
			} else {                                      // level up to level 4, gain the last two types
				Flag<Type> newType = player->typeAttributes;
				newType = newType >> static_cast<Type>(1);// calculate last type
				if(newType == Type::None) { newType = Type::Power; } // check for whether last type could be power
				player->typeAttributes |= static_cast<Type>(newType);
			}
		}
	} else {
		if(player->level == 1) {
			// level up to level 2, gain type
			player->typeAttributes = toGearType(player->characterptr->type);
			if (player->characterExload().exLoadID == EXLoad::E10R) {
				player->typeAttributes = Type::Power;
			}
			else if (player->characterExload().exLoadID == EXLoad::E10Y) {
				player->typeAttributes = Type::Fly; // SYB: Sweet Jesus we're going to hate this Gear in the future!
			}
		} else if(player->level == 2) {
			if(!player->gearSpecificFlags[Gambler::Level4]) { // level up to level 3
				Flag<Type> newType = player->typeAttributes;
				newType = newType << static_cast<Type>(1);// rotate to next type
				if(newType > Type::Power) { newType = Type::Speed; }// reset to speed type
				player->typeAttributes |= newType;
			} else {// level up to level 4
				player->typeAttributes |= Type::All;
			}
		}
	}
}

ASMUsed void Gambler_LoseTypeOnLeveldown(Player *player) {
	return;
	if(player->extremeGear != ExtremeGear::Gambler) { return; }

	if(player->character == Character::E10G) {
		if(player->level == 1) {
			// level down to level 2
			Flag<Type> newType = player->typeAttributes;
			newType = newType >> static_cast<Type>(1);// rotate to next type
			if(newType == Type::None) { newType = Type::Power; } // check for whether last type could be power
			player->typeAttributes = newType;
		} else if(player->level == 0) {
			// level down to level 1
			player->typeAttributes = Type::None;
		}

	} else {
		if(player->level == 1) {
			// level down to level 2
			player->typeAttributes = toGearType(player->characterptr->type);
			if (player->characterExload().exLoadID == EXLoad::E10R) {
				player->typeAttributes = Type::Power;
			}
			else if (player->characterExload().exLoadID == EXLoad::E10Y) {
				player->typeAttributes = Type::Fly;
			}

		} else if(player->level == 0) {
			// level down to level 1
			player->typeAttributes = Type::None;
		}
	}
}

void Player_GamblerBoost(Player *player) {
	if (player->extremeGear != ExtremeGear::Gambler) { return; }

	if (player->gearSpecificFlags[Gambler::Level4]
		&& player->movementFlags.hasAny(MovementFlags::boosting)) {
			player->gearStats[player->level].boostSpeed = (player->characterArchetype == CharacterArchetype::Boost) 
			? pSpeed(260.0f) + BoostArchetypeBoostSpeeds[player->level]
			: pSpeed(260.0f);
		}

		// player->rings = (player->rings - 10 < 90) ? 90 : player->rings - 10;

	if (player->state == PlayerState::Cruise
		&& player->input->toggleFaceButtons.hasAny(Buttons::X, Buttons::B)
		&& !player->movementFlags.hasAny(MovementFlags::boosting)
		&& player->unkB90 <= 0) {
		// Boost speed at level 4 scales with rings above 90
		if (player->gearSpecificFlags[Gambler::Level4]) {
			player->gearStats[player->level].boostSpeed = (player->characterArchetype == CharacterArchetype::Boost) 
			? pSpeed(260.0f) + pSpeed(static_cast<f32>(player->rings - 90)) + BoostArchetypeBoostSpeeds[player->level]:
			pSpeed(260.0f) + pSpeed(static_cast<f32>(player->rings - 90));
			player->rings = (player->rings - 10 < 90) ? 90 : player->rings - 10;
		} 
		// else if (player->level >= 1) {
		// 	player->rings -= 10;
		// }
		
		// Remove half of those rings above 90
		// player->rings = (player->rings - 10 < 90) ? 90 : player->rings - 10;
		// player->rings -= (150 - player->rings < 60) ? ((player->rings - 90) / 2) : 0;
	}
}