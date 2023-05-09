#include "calculate_date.hpp"
#include "macros.hpp"

#define NONLEAPYEAR_DAYS_IN_SEC 0x1E13380
#define LEAPYEAR_DAYS_IN_SEC 0x1E28500
#define THIRTYONE_DAYS_IN_SEC 0x28DE80
#define THIRTY_DAYS_IN_SEC 0x278D00
#define TWENTYNINE_DAYS_IN_SEC 0x263B80
#define TWENTYEIGHT_DAYS_IN_SEC 0x24EA00
#define ONE_DAY_IN_SEC 86400

// Maybe replace this with ogc's time functions?
u32 ConvertGetTimeToSeconds() {
	// gets time passed in seconds from 1st Jan 2000 until current time
	register u32 ret asm("r3");
	asm volatile(
		"stwu r1, -0x10 (r1)\n"
		"mflr r0\n"
		"stw r0, 0x14 (r1)\n"
		"bl %[OSGetTime]\n"
		"li r5, 0\n"
		"lis r6, 0x26A\n"
		"addi r6, r6, -0x4E0\n" // time base frequency
		"bl %[Divide64Bit]\n"
		"mr r3, r4\n"
		"lwz r0, 0x14 (r1)\n"
		"mtlr r0\n"
		"addi r1, r1, 0x10\n"
		: [ret] "=r"(ret)
		: [OSGetTime] "i"(OSGetTime), [Divide64Bit] "i"(Divide64Bit)
		: "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12");
	return ret;
}

bool CheckAprilFools() {
	Date currentDate;
	return (currentDate.day == 1 && currentDate.month == 4);
}

Date::Date() {
	u32 isThirtyOneDayMonth;
	u32 timePassedInSeconds = ConvertGetTimeToSeconds();

	year = 2000;
	month = 1;
	day = 1;

	// calculate year
	do {
		if (year % 4 == 0) {
			// leap year
			timePassedInSeconds -= LEAPYEAR_DAYS_IN_SEC;
			year++;
			if (timePassedInSeconds <= NONLEAPYEAR_DAYS_IN_SEC) break;
		} else {
			// non leap year
			timePassedInSeconds -= NONLEAPYEAR_DAYS_IN_SEC;
			year++;
			if (((year + 1) % 4) == 0) {
				// if next year is leap year
				if (timePassedInSeconds <= LEAPYEAR_DAYS_IN_SEC) break;
			} else {
				// if next year is not leap year
				if (timePassedInSeconds <= NONLEAPYEAR_DAYS_IN_SEC) break;
			}
		}
	} while (true);

	// calculate month
	do {
		if (month > 7) {
			// switch algorithm after july
			isThirtyOneDayMonth = (month % 2 == 0) ? TRUE : FALSE;
		} else {
			isThirtyOneDayMonth = (month % 2 != 0) ? TRUE : FALSE;
		}

		if (isThirtyOneDayMonth) {
			if (timePassedInSeconds <= THIRTYONE_DAYS_IN_SEC) break;
			timePassedInSeconds -= THIRTYONE_DAYS_IN_SEC;
			month++;
		} else {
			if (month == 2) {
				if (year % 4 == 0) {
					// leap year
					if (timePassedInSeconds <= TWENTYNINE_DAYS_IN_SEC) break;
					timePassedInSeconds -= TWENTYNINE_DAYS_IN_SEC;
					month++;
				} else {
					// non leap year
					if (timePassedInSeconds <= TWENTYEIGHT_DAYS_IN_SEC) break;
					timePassedInSeconds -= TWENTYEIGHT_DAYS_IN_SEC;
					month++;
				}
			} else {
				if (timePassedInSeconds <= THIRTY_DAYS_IN_SEC) break;
				timePassedInSeconds -= THIRTY_DAYS_IN_SEC;
				month++;
			}
		}
	} while (true);

	// calculate day
	do {
		if (timePassedInSeconds <= ONE_DAY_IN_SEC) break;
		timePassedInSeconds -= ONE_DAY_IN_SEC;
		day++;
	} while (true);
}
