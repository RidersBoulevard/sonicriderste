#include "gambler.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/stdlib.hpp"

ASMUsed void Gambler_GainTypeOnLevelup(Player *player) {
	if(player->extremeGear != ExtremeGear::Gambler) { return; }

	if(player->character == E10G) {
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
			player->typeAttributes = static_cast<Type>(1 << player->characterptr->type);
			if (FetchEnabledEXLoadIDs(*player).characterExLoadID == E10REXLoad) {
				player->typeAttributes = Type::Power;
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
	if(player->extremeGear != ExtremeGear::Gambler) { return; }

	if(player->character == E10G) {
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
			player->typeAttributes = static_cast<Type>(1 << player->characterptr->type);
			if (FetchEnabledEXLoadIDs(*player).characterExLoadID == E10REXLoad) {
				player->typeAttributes = Type::Power;
			}

		} else if(player->level == 0) {
			// level down to level 1
			player->typeAttributes = Type::None;
		}
	}
}