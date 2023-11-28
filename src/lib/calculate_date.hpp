#pragma once

#include "types.hpp"
#include "globalDefs.hpp"

ASMDefined u64 gettime(); // ogc function

struct Date {
	u8 day = 1;
	u8 month = 1;
	u16 year = 2000;

	static Date GetCurrentDate();

	[[nodiscard]] inline bool CheckAprilFools() const{
		return (day == 1 && month == 4);
	}

	// Tested to be functionally equivalent to the old implementation
	static inline u32 ConvertGetTimeToSeconds() {
		// gets time passed in seconds from 1st Jan 2000 until current time
		static constexpr auto timeBaseFrequency = 40500000;
		return gettime() / timeBaseFrequency;
	}
};
