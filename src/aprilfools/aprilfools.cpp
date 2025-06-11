#include "containers/graphicalobject.hpp"
#include "riders/object.hpp"
#include "lib/calculate_date.hpp"
#include "riders/gamemode.hpp"

// Functions for some April Fools shenanigans

ASMUsed void MotionBlur();	// _Main/text.s
extern Sys_2dObjectTableData lbl_10087EC4;

ASMUsed void AprilFools_EnableMotionBlur() {
	if(!Date::GetCurrentDate().CheckAprilFools()) { return; }
	if (!ruleSettings.unk7) {
		SetTask(&MotionBlur, ObjectGroups::MotionBlur, Object1Sizes::None);
	}
}

ASMUsed void AprilFools_EnablePoopyCSSGradient() {
	if(!Date::GetCurrentDate().CheckAprilFools()) { return; }
    void *cssBackground = lbl_10087EC4.graphicalData->objects[0x15]->objects[0x8];
	RGBA32 *gradient = static_cast<RGBA32*>(cssBackground) + 0x20 / 4;
    gradient[0] = 0x77FF00FF;
    gradient[1] = 0x77FF00FF;
    gradient[2] = 0x784000FF;
    gradient[3] = 0x784000FF;
}
