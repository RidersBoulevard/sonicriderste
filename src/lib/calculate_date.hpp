#pragma once

#include "context.hpp"

global u32 OSGetTime(); // actually returns a 64 bit value, in r3 and r4
global u32 Divide64Bit(u32 dividendLow, u32 dividendHigh, u32 divisorLow, u32 divisorHigh); // actually returns a 64 bit value, in r3 and r4

struct Date {
	u8 day;
	u8 month;
	u16 year;

	Date();
};
bool CheckAprilFools();
