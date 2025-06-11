#include "airtank.hpp"

std::array<AirTankInfo, MaxPlayerCount> PlayerAirTankInfo;

struct ballAndChainInfo
{
	Player *player;
	u32 timer;
	f32 maxSpeed;
};


void BallAndChain_Task() {
	ObjectNode * object = gpsCurrentTask;
    auto* ballInfo = static_cast<ballAndChainInfo *>(object->object);

	auto * ballPlayer = ballInfo->player;
    switch (object->state) {
        case 2:
            // lbl_000AC470
			if ((ballPlayer->state >= PlayerState::Cruise && ballPlayer->state <= PlayerState::Fall)
                || (ballPlayer->extremeGear == ExtremeGear::AdvantageS && ballPlayer->state == PlayerState::Run)) {
				if (ballPlayer->speed > pSpeed(150.0f)) {
					if(ballPlayer->speed < ballPlayer->gearStats[ballPlayer->level].baseTopSpeed) {
						ballPlayer->speed -= ((ballPlayer->gearStats[ballPlayer->level].tier1Accel * SPEED_DIVISOR) * pSpeed(5.0f));
					} else if(ballPlayer->speed < ballPlayer->gearStats[ballPlayer->level].baseAccel) {
						ballPlayer->speed -= ((ballPlayer->gearStats[ballPlayer->level].tier2Accel * SPEED_DIVISOR) * pSpeed(5.0f));
					} else if(ballPlayer->speed < ballPlayer->speedCap) {
						ballPlayer->speed -= ((ballPlayer->gearStats[ballPlayer->level].tier3Accel * SPEED_DIVISOR) * pSpeed(5.0f));
					} else {
						ballPlayer->speed -= ((ballPlayer->gearStats[ballPlayer->level].tier3Accel * SPEED_DIVISOR * 2.0f) * pSpeed(5.0f));
					}
				}
                ballPlayer->speed = clamp(ballPlayer->speed, pSpeed(150.0f));
			}
            
			ballInfo->timer -= 1;
            if (ballInfo->timer <= 0) object->state += 1;
            break;
        case 3:
            gNp_DeadTask();
            break;
        default:
            break;
    }
}

void Player_TrapGear(Player *player) {
    if (player->extremeGear != ExtremeGear::TrapGear) return;

    if (player->input->toggleFaceButtons.hasAny(Buttons::B) && player->rings >= 10 && player->state == PlayerState::Cruise) {
			// bomb
			lbl_0008CBD4();
            player->rings -= 10;
		}
		if (player->input->toggleFaceButtons.hasAny(Buttons::X) && player->rings >= 30) {
            auto * objectNode = SetTask(&BallAndChain_Task, ObjectGroups::Object_BallAndChain, Object1Sizes::x20);
			auto* ballInfo = static_cast<ballAndChainInfo *>(objectNode->object);
			// ball and chain
            player->rings -= 30;
            if (InGamePlayerCount == 2) { // 2P
                // give ball and chain to opposite player
                int otherPlayer = player->index ^ 1; // other player
                ballInfo->player = &players[otherPlayer];
            } else if (InGamePlayerCount > 2) { // 3 or more players
                // give ball and chain to random player
                ballInfo->player = &players[lbl_RNG_Number(InGamePlayerCount)]; // russian roulette including yourself
            } else ballInfo->player = player; // single player applies it to itself because why not ig.

			lbl_0008CB64(ballInfo->player, 240, pSpeed(150.0f)); // ball and chain func.

			// ballInfo->player = player;
			ballInfo->timer = 240;
			ballInfo->maxSpeed = pSpeed(150.0f);

			ballInfo->player->unkBCC = 0; // we need it fr
			lbl_0008CB1C(ballInfo->player->index); // makes the model ahaha
		}

    // if (player->extremeGear != AirTank) return;

    // AirTankInfo *ATInfo = &PlayerAirTankInfo[player->index];

    // if (player->state == StartLine)
    // {
    //     ATInfo->stoSize = 0;
    //     u32 itemStorage[] = {-1, -1, -1};
    //     ATInfo->itemUsed = 0;
    // }

    // if (player->input->toggleFaceButtons & XButton && player->state != StartLine)
    // {
    //     for (int i = 0; i < 3; i++)
    //     {
    //         if (ATInfo->itemStorage[i] == -1) {
    //             break;
    //         } else { // if (itemStorage[i] == 0)
    //             ATInfo->itemUsed = 1;
    //             AntiItemCampHandler(player, ATInfo->itemStorage[i]);
    //             // player->current_itemID = ATInfo->itemStorage[i];
    //             // player->last_itemID = ATInfo->itemStorage[i];
    //             ATInfo->itemStorage[i] = -1;
    //         } 
    //     }
    // }
}

void Player_AirTank(Player &player) {
	if (player.extremeGear != ExtremeGear::AirTank) return;
	if (player.state == PlayerState::StartLine) return;
	SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player.index];
	const f32 currAirPercent = static_cast<f32>(player.currentAir) / static_cast<f32>(player.gearStats[player.level].maxAir);
	const s32 driftChargeReduction = static_cast<s32>(15.0f * currAirPercent);
	player.requiredDriftDashFrames = 60 - driftChargeReduction;

	f32 topSpeedIncrease = pSpeed(30.0f) *  (1.0f - currAirPercent);
	player.gearStats[player.level].topSpeed = spfInfo->lvlTopSpeed[player.level] + topSpeedIncrease;
	// if (!player.movementFlags.hasAny(MovementFlags::boosting) && player.state == PlayerState::Cruise) player.speedCap = player.gearStats[player.level].topSpeed;

}