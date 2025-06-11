#include "faster.hpp"
#include "gears/blastGaugeGears.hpp"
#include "lib/stdlib.hpp"
#include "lib/sound.hpp"
#include "compare_many.hpp"
#include "cosmetics/player/supersonicaura.hpp"
#include <handlers/player/initgeardata.hpp>

std::array<AcceleratorInfo, MaxPlayerCount> PlayerAcceleratorInfo;

void Player_Faster(Player &player) {
	// if (player.character == Character::E10G && player.extremeGear == ExtremeGear::AdvantageP) {
	// 	player.trickAirGainMultiplier = 0.5;
	// }

	// if(player->extremeGear != ExtremeGear::Faster) { return; }
	if (!DebugMenu_CheckOption(DebugMenuOptions::PTRMode)) return;
	if(player.extremeGear != ExtremeGear::Accelerator) { return; }
    if (player.gearExload().exLoadID != EXLoad::HyperHangOn) { return; }

	// if (player.currentLap <= 1) player.specialFlags |= SpecialFlags::noBoostSpeedBoostChain;
	// else player.specialFlags &= ~SpecialFlags::noBoostSpeedBoostChain;
	//
	// if (player.state == PlayerState::Cruise && player.movementFlags.hasAny(MovementFlags::drifting)) {
	// 	player.specialFlags |= SpecialFlags::alwaysOnIce;
	// 	player.unk9A8 = 0.04f + (static_cast<f32>(player.input->leftStickVertical) / 4000.0f); // adjust drift angle based on stick vertical value
	// 	player.unk9AC = 1.4f; // 1.3 // Momentum in drift increased
	// } else {
	// 	player.specialFlags &= ~SpecialFlags::alwaysOnIce;
	// 	player.unk9A8 = 0.0261799022555351f;
	// 	player.unk9AC = 1.0472000837326f;
	// }

	// if (player->speed > player->gearStats[player->level].boostSpeed) {
	// 	player->specialFlags |= SpecialFlags::alwaysOnIce;
	// } else player->specialFlags &= ~SpecialFlags::alwaysOnIce;


    BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player.index];
    AcceleratorInfo &aclInfo = PlayerAcceleratorInfo[player.index];

    if (player.state == PlayerState::StartLine) return;

    // aclInfo.isOverheated ? (player.gearStats[0].maxAir * 2) / 100 :
    player.gearStats[0].airDrain = aclInfo.isOverheated ? 0 : 0x9;
    player.gearStats[0].boostCost = aclInfo.isOverheated ? (player.gearStats[0].maxAir * 5) / 100 : (player.gearStats[0].maxAir * 7) / 100;
    player.gearStats[player.level].driftCost = (player.characterArchetype == CharacterArchetype::Drift) ? 20 : 22;

    // Kickdash handler
    f32 boostArchBonus = player.characterArchetype == CharacterArchetype::Boost ? BoostArchetypeBoostSpeeds[player.level] : 0.0f;
    if (aclInfo.isOverheated && !player.movementFlags.hasAny(MovementFlags::boosting)) {
        f32 kickdashSpeed = player.characterArchetype == CharacterArchetype::Boost ? pSpeed(90.0f) : pSpeed(80.0f);
        kickdashSpeed = clamp(player.speed + kickdashSpeed, pSpeed(0.0f), pSpeed(330.0f));
        player.gearStats[player.level].boostSpeed = kickdashSpeed; // Kickdash calc here
    } else if (!aclInfo.isOverheated && !player.movementFlags.hasAny(MovementFlags::boosting)) {
        player.gearStats[player.level].boostSpeed = pSpeed(240.0f) + boostArchBonus;
    }

    // Meter drain handler
    if (if_any(player.state, std::is_eq, PlayerState::Cruise, PlayerState::Jump, PlayerState::Fall)) {
        s32 drainAmount = (!player.movementFlags.hasAny(MovementFlags::boosting)) ? 83 : 0;
        s32 overheatBonus = aclInfo.isOverheated ? 5 : 1; // Overheat doubles drain bonus
        // drain 2.5% faster when drifting
        drainAmount += player.movementFlags.hasAny(MovementFlags::drifting) && player.state == PlayerState::Cruise ? 30 * overheatBonus : 0;
        // drain 2.5% faster when charging jump
        drainAmount += (player.jumpCharge > 0 && player.input->holdFaceButtons.hasAny(Buttons::A) && player.state == PlayerState::Cruise) ? 30 * overheatBonus : 0;
        drainAmount = bgInfo->currentGauge - drainAmount;
        bgInfo->currentGauge = clamp(drainAmount, 0, 100000); // clamp values
    }

    // REV-CHAIN: Boost chain out of boosts (only when not overheated)
    s32 gaugeGain = 0;
    if (!aclInfo.isOverheated
        && player.state == PlayerState::Cruise
        && player.input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X)
        && player.currentAir >= player.gearStats[player.level].boostCost
        && player.unkB90 <= 0
        && player.unkB08 & 0x2400
        && player.movementFlags.hasAny(MovementFlags::boosting)) {

        gaugeGain = bgInfo->currentGauge + 37500;
        bgInfo->currentGauge = clamp(gaugeGain, 0, 100000); // 25% meter increase.

        if (bgInfo->currentGauge < 100000) lbl_Player_BoostEndFunction(player);
        else player.speed += pSpeed(50.0f);

    }

    // Overheat handler
    if (bgInfo->currentGauge >= 100000) {
        if (!aclInfo.isOverheated) PlayAudioFromDAT(Sound::SFX::AdvPTOP3);
        aclInfo.isOverheated = true; // Overheated if meter filled out
        player.specialFlags |= SpecialFlags::noBoostChain; // Kickdash
        bgInfo->currentGauge = 100000;
    } else if (bgInfo->currentGauge <= 0) {
        if (aclInfo.isOverheated) PlayAudioFromDAT(Sound::SFX::AdvPTOP1);
        aclInfo.isOverheated = false; // end overheat (recovery) if meter fully depleted.
        player.specialFlags &= ~SpecialFlags::noBoostChain; // Can boost chain
        bgInfo->currentGauge = 0;
    }

    if (aclInfo.isOverheated) SuperSonicAuraCXX(player); // Overheat visual

}