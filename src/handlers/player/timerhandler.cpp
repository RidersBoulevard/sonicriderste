#include "timerhandler.hpp"
#include "cosmetics/player/exloads.hpp"
#include "gears/hyperhangon.hpp"
#include "lib/sound.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "gears/supertails.hpp"
#include "gears/blastGaugeGears.hpp"
#include "lib/stdlib.hpp"
#include "cosmetics/player/dreamtrail.hpp"
#include "handlers/player/specialflagtweaks.hpp"

void Player_TimerHandler(Player &player) {
    // decrement tornado invincibility timer upon ignoring a tornado with Z button
    if (player.tornadoIgnore_invincibilityTimer != 0) {
        player.tornadoIgnore_invincibilityTimer -= 1;
    }

    // decrement tornado invuln timer
    if (player.tornadoInvulnerabilityTimer != 0) {
        if (player.tornadoInvulnerabilityTimer == 1) {
            player.statusEffectFlags &= ~PlayerStatus::InvincibilityStatus;
        } else if (!player.statusEffectFlags.hasAny(PlayerStatus::InvincibilityStatus)) {
            player.statusEffectFlags |= PlayerStatus::InvincibilityStatus;
        }
        
        player.tornadoInvulnerabilityTimer -= 1;
    }

    // decrement and apply dream trail timer
    if (player.dreamTrail_timer != 0) {
        player.dreamTrail_timer -= 1;
        Player_DreamTrailParticles(player);
    }

    // decrement berserker counter attack timer
    if ((player.state != PlayerState::AttackedByPlayer && player.state != PlayerState::AttackingPlayer) && player.berserkerCooldown != 0) {
        player.berserkerCooldown -= 1;
    }

    // decrement hang on max rings timer and set max rings
    if (player.extremeGear == ExtremeGear::HangOn) {
        if (player.hangOn_infiniteRingTimer > 0) { //  && player->state != AttackingPlayer
            if (player.state != PlayerState::AttackingPlayer) {
                player.hangOn_infiniteRingTimer -= 1;
                if(player.hangOn_infiniteRingTimer >= 179) {
					PlayAudioFromDAT(Sound::SFX::GoldExp);
				}
            }
            player.currentAir = player.gearStats[player.level].maxAir;
        }
    }

    // decrement super tails transform cooldown
    if (player.isSuperCharacter(Character::Tails)) {
	    SuperTailsInfo *stInfo = &PlayerSuperTailsInfo[player.index];
        if (player.superTails_transformCooldown > 0) {
            player.superTails_transformCooldown -= 1;
        } else if (player.superFormState == 1 && stInfo->transformCheck == 1) {
            // changes super state to 2 when cooldown is done and you're in 1
            stInfo->transformCheck = 0;
            player.superFormState = 2;
        }
    }

    // decrement SS meter
    // if (player.character == Character::SuperSonic && player.extremeGear == ExtremeGear::ChaosEmerald
    //     && player.gearExload().exLoadID != EXLoad::HyperSonic && player.gearExload().exLoadID != EXLoad::DarkSonic) {
    //     BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player.index];
    //     if (player.state == PlayerState::Cruise && !(player.movementFlags.hasAny(MovementFlags::boosting) && player.unkD90 > 0)) {
    //         s32 newGauge = (player.movementFlags.hasAny(MovementFlags::boosting))
    //         ? bgInfo->currentGauge - 45
    //         : bgInfo->currentGauge - 62.475;
    //         bgInfo->currentGauge = clamp(newGauge);
    //     }
    // }

	// EnabledEXLoads exLoads;
	// FetchEnabledEXLoadIDs(&player, exLoads);
    if (player.gearExload().exLoadID == EXLoad::HyperSonic) {
        if (player.hyperSonic_totalLinkTimer > 0 && (player.state == PlayerState::Cruise || player.state == PlayerState::Run) && !player.movementFlags.hasAny(MovementFlags::boosting)) { // && !(player->movementFlags & boosting)
            player.hyperSonic_totalLinkTimer -= 1;
        }
        if (player.hyperSonic_totalLinkTimer == 1 && ((player.state != PlayerState::Cruise && player.state != PlayerState::Run) || player.movementFlags.hasAny(MovementFlags::boosting))) {
            player.hyperSonic_totalLinkTimer = 0;
        }
    }

    if (MI::impulseData[player.index].afterburnerTimer > 0) {
		MI::impulseData[player.index].afterburnerTimer -= 1;
	}

	//HHOInfo *hhoInfo = &PlayerHHOInfo[player.index];
    // if (exLoads.gearExLoadID == HyperHangOn) {
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

	SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player.index];
	if (spfInfo->typeRemovalTimer > 0.0f && player.state != PlayerState::Death) {
		player.specialFlags |= SpecialFlags::lowBoost; // Should remove type shortcuts, but that doesn't work rn so this is just as funny lmao
		spfInfo->typeRemovalTimer -= 1.0f;
	} else if (spfInfo->typeRemovalTimer <= 0.0f && !player.gearptr->specialFlags.hasAny(SpecialFlags::lowBoost)) {
		player.specialFlags &= ~SpecialFlags::lowBoost;
	}
}