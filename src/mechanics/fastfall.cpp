#include "fastfall.hpp"
#include "cosmetics/player/exloads.hpp"
// #include "gears/eggsterminator.hpp"

void lbl_FastFall(Player *player){
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	// EggInfo *EggInfo = &PlayerEggsterminator[player->index];

	if(player->state < Jump || player->state > TurbulenceTrick2){ return; }
	if(!player->input->holdFaceButtons.hasAny(BButton)){ return; }// B button
	if (player->characterArchetype == Windcatcher) {
		player->verticalSpeed += -0.0231482F;
	} else {
		player->verticalSpeed += -0.0115741F;
	}

	if(player->specialFlags.hasAny(ringGear)){
		s32 maxAir = player->gearStats[0].maxAir;
		maxAir /= 100; // 1 ring
		f32 ringCost;
		if(player->extremeGear == ExtremeGear::Accelerator){
			ringCost = static_cast<f32>(maxAir) / 7.5F;
		}else{
			ringCost = static_cast<f32>(maxAir) / 8;
		}

		if(player->characterArchetype == TricksterArchetype){
			ringCost *= 0.75f;
		}

		const s32 ringTest = player->currentAir - static_cast<s32>(ringCost);
		if(ringTest > 0){
			player->currentAir = ringTest;
		}
	}else{
		const bool tricksterAirReduction = player->characterArchetype == TricksterArchetype;
		const bool legendAirReduction = player->extremeGear == ExtremeGear::Legend;
		const u32 airReduction = (tricksterAirReduction ? 2u : 0u) + (legendAirReduction ? 1u : 0u);

		if(player->state == Jump || player->state == Fall){
			f32 airCost = 700.0F * player->weight;
			airCost *= (1.0F - (static_cast<f32>(airReduction) * 0.25F));
			// if (exLoads.gearExLoadID == E99EXLoad && EggInfo->emeraldsInserted == 5) {
			//     player->currentAir -= 0;
			// } else
			player->currentAir -= static_cast<s32>(airCost);
		}else{
			player->currentAir -= (650 - (650 / 4 * static_cast<s32>(airReduction)));
		}
	}
}