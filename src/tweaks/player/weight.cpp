#include "riders/player.hpp"
#include "riders/stage.hpp"

// WEIGHT STAT

ASMUsed f32 Weight_NaturalFall(Player *player, f32 f0, f32 f1) {
    f32 finalVertSpeed = f0;
    f32 differenceFromBase, weightcalc;
    
    if (*(u16 *) (&player->state) != 0x612) {
        if (player->state == PlayerState::Fall || player->state == PlayerState::Jump) {
            if (player->qteState != PlayerState::QTE) {
                if (CurrentStage == DigitalDimension && player->qteState == PlayerState::QTE2) {
	                return finalVertSpeed;
                }
                differenceFromBase = player->weight - 1.0f;
                weightcalc = f0 + f1;
                if (weightcalc < 0.0f) {
                    weightcalc *= differenceFromBase;
                    weightcalc /= 62.5f;
                    finalVertSpeed += weightcalc;
                }
            }
        }
    }
    return finalVertSpeed;
}

ASMUsed f32 Weight_UpDownHillSpeed(Player *player, f32 currentSpeed) {
    f32 finalSpeed = currentSpeed;
    f32 deltaSpeed;

    if (player->state != PlayerState::QTE) {
        deltaSpeed = currentSpeed - player->speed;
        if (deltaSpeed > 0.0f) {
            // downhill
            finalSpeed += (player->weight * 2.25f) * deltaSpeed;
        } else {
            // uphill
            finalSpeed -= deltaSpeed;
            deltaSpeed *= (player->weight / 1.3f);
            if (player->extremeGear == ExtremeGear::PowerfulGear) {
                deltaSpeed = -deltaSpeed / 4.0f;
            }
            finalSpeed += deltaSpeed;
        }
    }

    return finalSpeed;
}