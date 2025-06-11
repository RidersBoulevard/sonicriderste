#include "boostchainmultiplier.hpp"
#include "riders/player.hpp"
#include "gears/turbostar.hpp"
#include "riders/gear.hpp"
#include "gears/hypersonic.hpp"
#include "tweaks/player/archetype/boostarchetypejcbc.hpp"


ASMUsed void Player_BoostChainMultiplier(Player *player) {
	const f32 currentBoostSpeed = player->gearStats[player->level].boostSpeed;
    player->speedCap = currentBoostSpeed;

	if (player->specialFlags.hasAny(SpecialFlags::noBoostChain)) {
		player->speed = currentBoostSpeed;
		return;
	}
    
    // add 0.1% bcm per 1 boost speed above average boost speeds
	f32 additiveBcm = (currentBoostSpeed - BCM_GeneralBoostSpeeds[player->level]) * (0.001f / pSpeed(1));
    if (player->specialFlags.hasAny(SpecialFlags::ringGear) || player->gearExload().exLoadID == EXLoad::HyperSonic
    	|| (isSuperCharacter(*player, Character::MetalSonic) && player->gearExload().exLoadID == EXLoad::StardustSpeeder)) { additiveBcm *= (1.0f/3.0f); }
	f32 bcm = lbl_001C7400;
    if (additiveBcm > 0) { bcm += additiveBcm; }

    switch (player->characterArchetype) {
        case CharacterArchetype::Combat:
            bcm += 0.03f;
            break;
        case CharacterArchetype::NoType:
            bcm += 0.04f;
            break;

        case CharacterArchetype::AllRounder:
            bcm += 0.02f;
            break;
    }

    switch (player->extremeGear) {
		using namespace ExtremeGear;
        case BlueStar2:
            bcm += 0.10f;
            break;

        case HangOn:
            if (player->hangOn_infiniteRingTimer > 0) bcm += 0.11f;
        //     bcm += 0.11f;
            break;

        case AdvantageF:
            if (player->gearExload().exLoadID == EXLoad::TheProfessional){
                if (player->input->holdFaceButtons.hasAny(Buttons::A) && player->state == PlayerState::Cruise) {
					bcm += 0.35f;
				} else {
					bcm += 0.25f;
				}
            } else {
				bcm += 0.05f;
			}
            break;

    	case AirTank:
    		// More air, more bcm
    		bcm += 0.03f * (static_cast<f32>(player->currentAir) / static_cast<f32>(player->gearStats[player->level].maxAir));
    		break;

    	case Accelerator:
			if (player->currentLap == 2) {
				bcm += 0.11f;
			} else if (player->currentLap >= 3) bcm += 0.06f;
    		break;

    	case Darkness:
    		if (player->level == 2 && player->rings >= 60) {
    			f32 jcbcCheck = (player->input->holdFaceButtons.hasAny(Buttons::A) && player->state == PlayerState::Cruise) ? 0.10f : 0.05f;
    			bcm += jcbcCheck * (static_cast<f32>(player->rings - 60) / 40.0f);
    		}
        
		default:
			break;
    }

    if (player->specialFlags.hasAny(SpecialFlags::lowBoost)) {
        bcm = 1.05f;
    }
	player->speed *= bcm;
}