#include "omnipotence.hpp"

std::array<OmnipotenceInfo, MaxPlayerCount> PlayerOMNInfo;

void Player_Omnipotence(Player &player) {
	if(player.extremeGear != ExtremeGear::Omnipotence)
		return;
	auto &[lastShortcutType, FirstRail, jumpedFromRail, hasFlown, qteEntered, firstPowerObj] = PlayerOMNInfo[player.index];

	if(player.state == PlayerState::StartLine) {
		lastShortcutType = 0;
		FirstRail        = false;
		jumpedFromRail   = false;
		hasFlown         = false;
		qteEntered       = false;
		firstPowerObj    = false;
	}

	if(player.character == Character::E10G)
		return;

	// if(player.state == RailGrind
	//    || player.state == Fly
	//    || player.unkD90 >= 1) {
	// 	player.omnipotence_Timer = 180;
	// } else if (player.omnipotence_Timer != 0
	//             && (player.state == Cruise || player.state == Run)
	//             && !player.movementFlags.hasAny(boosting)) {player.omnipotence_Timer -= 1;}

	if(player.unkD90 != 0 && (player.state == PlayerState::Cruise)) {
		firstPowerObj = true;
	} else if(firstPowerObj == true && (player.state != PlayerState::Cruise || player.unkD90 == 0)) {
		lastShortcutType = 3;
		firstPowerObj    = false;
	}

	// If power link timer is written and not grinding or flying, you took power type shortcut last.
	// if (player.unkD90 > 0 && (player.state != RailGrind || player.state != Fly)) {
	// if (player.omnipotence_Timer == 0) OMNInfo.lastShortcutType = 3;
	else {
		switch(player.previousState) {
			case PlayerState::Fly:
				if(hasFlown == true)
					lastShortcutType = 2;
				FirstRail = false;
				break;

			case PlayerState::RailGrind:
				if(player.state == PlayerState::Cruise || player.state == PlayerState::Fall) {
					lastShortcutType = 1;
					FirstRail        = true;
				} else
					if(player.state == PlayerState::Jump)
						jumpedFromRail = true;
				break;
			default: break;
		}

		switch(player.state) {
			case PlayerState::RailGrind:
				if(lastShortcutType == 2)
					break;
				break;

			case PlayerState::Fly:
				hasFlown = true;
				if(lastShortcutType == 2) {
					if(FirstRail == true || jumpedFromRail == true)
						lastShortcutType = 1;
				} else
					if(lastShortcutType == 1)
						break;
				break;

			// Was used to switch types for WMJ
			// case Jump:
			// if (OMNInfo.jumpedFromRail == true && player.input.holdFaceButtons & (AButton)) {
			//     if (OMNInfo.railJumpDelay < 20.0f) {
			//         OMNInfo.railJumpDelay += 1.0f;
			//     } else
			//     {
			//         OMNInfo.railJumpDelay = 0.0f;
			//         OMNInfo.lastShortcutType = 1;
			//     }
			// }
			// break;

			case PlayerState::QTE:
			case PlayerState::QTE2:
				if(player.previousState == PlayerState::Fly)
					qteEntered = true;
				if(player.previousState == PlayerState::Cruise && hasFlown == true)
					qteEntered = true;
				if(jumpedFromRail == true)
					lastShortcutType = 1;
				break;

			case PlayerState::Cruise:
				if(jumpedFromRail == true) {
					if(player.previousState == PlayerState::Jump || player.previousState == PlayerState::Fall) {
						if(qteEntered == true) {
							lastShortcutType = 2;
						} else
							lastShortcutType = 1;
					} else if(hasFlown == true) {
						lastShortcutType = 2;
					}
				} else
					if(qteEntered == true && hasFlown == true)
						lastShortcutType = 2;
				jumpedFromRail = false;
				hasFlown       = false;
			// OMNInfo.railJumpDelay = 0.0f;
				qteEntered = false;
				break;

			default: break;
		}
	}

}
