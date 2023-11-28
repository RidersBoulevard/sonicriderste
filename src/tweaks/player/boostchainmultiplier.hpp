#pragma once

#include "globalDefs.hpp"

ASMDefined const f32 lbl_001C7400; // base boost chain multiplier // _Main/rodata.s

constexpr std::array<f32, 3> BCM_GeneralBoostSpeeds = {
		pSpeed(200),
		pSpeed(230),
		pSpeed(250)
};
