#include "context.hpp"
#include "lib/calculate_date.hpp"

// Functions for some April Fools shenanigans

ASMUsed void MotionBlur();	// _Main/text.s

ASMUsed void AprilFools_EnableMotionBlur() {
    if (CheckAprilFools()) {
        if (!(RuleSettings & 0x80)) {
            SetTask(&MotionBlur, 0xE678, 0);
        }
    }
}