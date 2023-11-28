#include "calculate_date.hpp"

Date Date::GetCurrentDate() {
	Date date;
	auto& [day, month, year] = date;
	u32 timePassedInSeconds = ConvertGetTimeToSeconds();

	static constexpr auto NONLEAPYEAR_DAYS_IN_SEC = 0x1E13380;
	static constexpr auto LEAPYEAR_DAYS_IN_SEC = 0x1E28500;
	static constexpr auto THIRTYONE_DAYS_IN_SEC = 0x28DE80;
	static constexpr auto THIRTY_DAYS_IN_SEC = 0x278D00;
	static constexpr auto TWENTYNINE_DAYS_IN_SEC = 0x263B80;
	static constexpr auto TWENTYEIGHT_DAYS_IN_SEC = 0x24EA00;
	static constexpr auto ONE_DAY_IN_SEC = 86400;

	// calculate year
	while (true) {
		if (year % 4 == 0) {
			// leap year
			timePassedInSeconds -= LEAPYEAR_DAYS_IN_SEC;
			year++;
			if (timePassedInSeconds <= NONLEAPYEAR_DAYS_IN_SEC) { break; }
		} else {
			// non leap year
			timePassedInSeconds -= NONLEAPYEAR_DAYS_IN_SEC;
			year++;
			if (((year + 1) % 4) == 0) {
				// if next year is leap year
				if (timePassedInSeconds <= LEAPYEAR_DAYS_IN_SEC) { break; }
			} else {
				// if next year is not leap year
				if (timePassedInSeconds <= NONLEAPYEAR_DAYS_IN_SEC) { break; }
			}
		}
	}

	// calculate month
	while (true) {
		bool isThirtyOneDayMonth;
		if (month > 7) {
			// switch algorithm after july
			isThirtyOneDayMonth = month % 2 == 0;
		} else {
			isThirtyOneDayMonth = month % 2 != 0;
		}

		if (isThirtyOneDayMonth) {
			if (timePassedInSeconds <= THIRTYONE_DAYS_IN_SEC) { break; }
			timePassedInSeconds -= THIRTYONE_DAYS_IN_SEC;
			month++;
		} else {
			if (month == 2) {
				if (year % 4 == 0) {
					// leap year
					if (timePassedInSeconds <= TWENTYNINE_DAYS_IN_SEC) { break; }
					timePassedInSeconds -= TWENTYNINE_DAYS_IN_SEC;
					month++;
				} else {
					// non leap year
					if (timePassedInSeconds <= TWENTYEIGHT_DAYS_IN_SEC) { break; }
					timePassedInSeconds -= TWENTYEIGHT_DAYS_IN_SEC;
					month++;
				}
			} else {
				if (timePassedInSeconds <= THIRTY_DAYS_IN_SEC) { break; }
				timePassedInSeconds -= THIRTY_DAYS_IN_SEC;
				month++;
			}
		}
	}

	// calculate day
	while (true) {
		if (timePassedInSeconds <= ONE_DAY_IN_SEC) { break; }
		timePassedInSeconds -= ONE_DAY_IN_SEC;
		day++;
	}
	return date;
}
