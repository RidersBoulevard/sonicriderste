#pragma once

#include "globalDefs.hpp"
#include "containers/vector3.hpp"

using TransformData = std::pair<Vector3F, Euler>;

struct StageChanges{
	u32 timer;
	struct TornadoStat{ // Possibly Vector3?
		f32 unk1;
		f32 unk2;
		f32 unk3;
	};
	std::array<TornadoStat, 3> tornadoStats;
	f32 slingShotSpeedBoost;
	std::array<TransformData, 3> whiteCaveDashPanels;
	std::array<f32, 3> eggFactoryDashPanelZPos;
	f32 eggFactoryDashPanelXPos;
	std::array<TransformData, 2> eggFactoryFlyRings;
	std::array<std::pair<f32, f32>, 4> redCanyonDashPanelXZPositions;
	f32 redCanyonDashPanelYPos;
	f32 digitalDimensionFirstRowZPos;
	std::array<Vector3F, 4> digitalDimensionItemBoxPositions;
};
static_assert(sizeof(StageChanges) == 0x10C); // NOLINT(readability-magic-numbers)

ASMDefined StageChanges data_stageChanges;

struct Object1 {
	/* 0x0	*/ Vector3F pos;
	/* 0xC	*/fillerData<0xC> unknown;
	/* 0x18	*/ Euler rotation;
	/* 0x24	*/fillerData<0x24> filler;
	/* 0x48	*/u32 item_icon;
};