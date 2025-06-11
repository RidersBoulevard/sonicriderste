#include "fastfall.hpp"
#include "cosmetics/player/exloads.hpp"

std::array<f32, MaxPlayerCount> PlayerFastfallDebts;

void lbl_FastFall(Player &player){
	if(player.state < PlayerState::Jump || player.state > PlayerState::TurbulenceTrick2){
		f32* FastfallDebt = &PlayerFastfallDebts[player.index];
		*FastfallDebt = 0;
		return; 
	}
	if(!player.input->holdFaceButtons.hasAny(Buttons::B)){ return; }// B button
	if (player.characterArchetype == CharacterArchetype::Windcatcher) {
		player.verticalSpeed += -0.0231482F;
	} else {
		player.verticalSpeed += -0.0115741F;
	}

	if(player.specialFlags.hasAny(SpecialFlags::ringGear)){
		s32 maxAir = player.gearStats[0].maxAir;
		maxAir /= 100; // 1 ring
		f32 ringCost;
		ringCost = static_cast<f32>(maxAir) / 8;

		if(player.characterArchetype == CharacterArchetype::Trickster){
			ringCost *= 0.75f;
		}

		const s32 ringTest = player.currentAir - static_cast<s32>(ringCost);
		if(ringTest > 0){
			player.currentAir = ringTest;
		}
	}else{
		const bool tricksterAirReduction = player.characterArchetype == CharacterArchetype::Trickster;
		const bool legendAirReduction = player.extremeGear == ExtremeGear::Legend;
	    const bool darknessAirReduction = player.extremeGear == ExtremeGear::Darkness && player.gearExload().exLoadID != EXLoad::TheProfessional;
		// const u32 airReduction = (tricksterAirReduction ? 2u : 0u) + (legendAirReduction ? 1u : 0u);
	    const f32 airReduction = (tricksterAirReduction ? 2.0f : 0.0f) + (legendAirReduction ? 1.0f : 0.0f) + (darknessAirReduction ? 0.4f : 0.0f);

		if(player.state == PlayerState::Jump || player.state == PlayerState::Fall){
			f32 airCost = 700.0F * player.weight;
			airCost *= (1.0F - (static_cast<f32>(airReduction) * 0.25F));
			player.currentAir -= static_cast<s32>(airCost);
		} else {
			player.currentAir -= (650 - (650 / 4 * static_cast<s32>(airReduction)));
		}
	}

	if (player.currentAir < 0) {
		f32* FastfallDebt = &PlayerFastfallDebts[player.index];
		*FastfallDebt += player.currentAir;
		player.currentAir = 0;
	}
}

ASMUsed f32 Player_Fastfall_AirDebt(Player &player, f32 airGain) {
	auto &FastfallDebt = PlayerFastfallDebts[player.index];
	airGain += FastfallDebt;
	if (airGain < 0) {
		airGain = 0;
	}

	FastfallDebt = 0;
	return airGain;
}