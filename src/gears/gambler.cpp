#include "context.hpp"

ASMUsed void Gambler_GainTypeOnLevelup(Player *player) {
    if (player->extremeGear != Gambler) return;

    if (player->character == E10G) {
        if (player->level == 1) {
            // level up to level 2, gain random type
            player->typeAttributes = (1 << lbl_RNG_Number(3));

        } else if (player->level == 2) {
			u32 newType;
            if (player->level4 == 1) {
                // level up to level 4, gain the last two types
                newType = player->typeAttributes >> 1; // calculate last type
                if (newType == 0) newType = PowerType; // check for whether last type could be power
                player->typeAttributes |= newType;

            } else {
                // level up to level 3
                newType = player->typeAttributes << 1; // rotate to next type
                if (newType > PowerType) newType = SpeedType; // reset to speed type
                player->typeAttributes = newType;
            }
        }

    } else {
        if (player->level == 1) {
            // level up to level 2, gain type
            player->typeAttributes = (1 << player->characterptr->type);

        } else if (player->level == 2) {
            if (player->level4 == 1) {
                // level up to level 4
                player->typeAttributes |= AllType;

            } else {
				u32 newType;
                // level up to level 3
                newType = player->typeAttributes << 1; // rotate to next type
                if (newType > PowerType) newType = SpeedType; // reset to speed type
                player->typeAttributes |= newType;
            }
        }
    }
    
}

ASMUsed void Gambler_LoseTypeOnLeveldown(Player *player) {
    if (player->extremeGear != Gambler) return;

    if (player->character == E10G) {
        if (player->level == 1) {
			u32 newType;
            // level down to level 2
            newType = player->typeAttributes >> 1; // calculate last type
            if (newType == 0) newType = PowerType; // check for whether last type could be power
            player->typeAttributes = newType;

        } else if (player->level == 0) {
            // level down to level 1
            player->typeAttributes = 0; // no type
            
        }

    } else {
        if (player->level == 1) {
            // level down to level 2
            player->typeAttributes = (1 << player->characterptr->type);

        } else if (player->level == 0) {
            // level down to level 1
            player->typeAttributes = 0; // no type

        }
    }
}