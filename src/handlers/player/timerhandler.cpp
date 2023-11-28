#include "timerhandler.hpp"
#include "cosmetics/player/exloads.hpp"
#include "gears/hyperhangon.hpp"
#include "lib/sound.hpp"
#include "mechanics/magneticimpulse.hpp"

void Player_TimerHandler(Player &player) {
    EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(&player, exLoads);
    // decrement item box cooldown timer
    if (player.flags.hasAny(ItemBoxCooldownFlag)) {
        if (player.itemBox_cooldown != 0) {
            player.itemBox_cooldown -= 1;
        } else {
            player.flags &= ~ItemBoxCooldownFlag;
        }
    }

    // decrement tornado invincibility timer upon ignoring a tornado with Z button
    if (player.tornadoIgnore_invincibilityTimer != 0) {
        player.tornadoIgnore_invincibilityTimer -= 1;
    }

    // decrement tornado invuln timer
    if (player.tornadoInvulnerabilityTimer != 0) {
        if (player.tornadoInvulnerabilityTimer == 1) {
            player.statusEffectFlags &= ~InvincibilityStatus;
        } else if (!player.statusEffectFlags.hasAny(InvincibilityStatus)) {
            player.statusEffectFlags |= InvincibilityStatus;
        }
        
        player.tornadoInvulnerabilityTimer -= 1;
    }

    // decrement and apply dream trail timer
    if (player.dreamTrail_timer != 0) {
        player.dreamTrail_timer -= 1;
        Player_DreamTrailParticles(&player);
    }

    // decrement berserker counter attack timer
    if (player.state != AttackedByPlayer && player.berserkerCooldown != 0) {
        player.berserkerCooldown -= 1;
    }

    // decrement hang on max rings timer and set max rings
    if (player.extremeGear == ExtremeGear::HangOn) {
        if (player.hangOn_infiniteRingTimer > 0) { //  && player->state != AttackingPlayer
            if (player.state != AttackingPlayer) {
                player.hangOn_infiniteRingTimer -= 1;
                if(player.hangOn_infiniteRingTimer >= 179) {
					PlayAudioFromDAT(Sound::SFX::GoldExp);
				}
            }
            if (exLoads.gearExLoadID == HangOnATEXLoad) {
                player.rings = 100;
                player.currentAir += 750;
            } else {
				player.currentAir = player.gearStats[player.level].maxAir;
			}
        }
    }

    // decrement super tails transform cooldown
    if (isSuperCharacter(player, Tails)) {
        if (player.superTails_transformCooldown > 0) {
            player.superTails_transformCooldown -= 1;
        }
    }

	// EnabledEXLoads exLoads;
	// FetchEnabledEXLoadIDs(&player, exLoads);
    if (exLoads.gearExLoadID == HyperSonicEXLoad) {
        if (player.hyperSonic_totalLinkTimer > 0 && ((player.state == Cruise) || (player.state == Run)) && !player.movementFlags.hasAny(boosting)) { // && !(player->movementFlags & boosting)
            player.hyperSonic_totalLinkTimer -= 1;
        }
        if (player.hyperSonic_totalLinkTimer == 1 && ((player.state != Cruise && player.state != Run) || player.movementFlags.hasAny(boosting))) {
            player.hyperSonic_totalLinkTimer = 0;
        }
    }

    if (MI::impulseData[player.index].afterburnerTimer > 0) {
		MI::impulseData[player.index].afterburnerTimer -= 1;
	}

	//HHOInfo *hhoInfo = &PlayerHHOInfo[player.index];
    // if (exLoads.gearExLoadID == HyperHangOnEXLoad) {
    //     if (hhoInfo->saturnMegadriveStatus == 2) // active state
    //     {
    //         if(hhoInfo->HHOTimer > 0 && ((player->state == Cruise) || (player->state >= FrontflipRamp && player->state <= TurbulenceTrick2))) hhoInfo->HHOTimer -= 1;
    //         if (hhoInfo->saturnMegadriveStatus == 2)
    //         {
    //             if (hhoInfo->HHOTimer == 0 && (player->state == Cruise)) hhoInfo->saturnMegadriveStatus = 3;
    //         }
    //     }
    // }

    if (player.specialReciproExtend && player.reciproExtendTimer >= 60) {
        // reset special recipro extend if you lose original recipro extend
        player.specialReciproExtend = false;
    }
}