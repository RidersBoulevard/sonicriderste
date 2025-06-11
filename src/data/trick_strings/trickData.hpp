#pragma once

#include <locale>

template<std::size_t size>
class TrickString{
	std::array<char, size*2> buf{};

public:
	consteval explicit TrickString(const char (&str)[size]){ // NOLINT(modernize-avoid-c-arrays)
		if(size/2 > 0xFF){
			throw std::logic_error("Size of string is too large");
		}
		int spaces = 0;
		bool successfulParse = false;
		for (std::size_t i = 0; i < size; i++) {
			const std::size_t destIdx = (i - static_cast<size_t>(spaces)) * 2;
			char curChar = str[i];
			switch (curChar) {
				case 'A'...'Z':{
					constexpr auto diff = 'a' - 'A';
					curChar = static_cast<char>(curChar + diff);
				}
					[[fallthrough]];
				case 'a'...'z':
					buf[destIdx] = curChar - 'a';
					buf[destIdx + 1] = static_cast<char>(0xFC);
					continue;
				case ' ': // Got a space, go back and enable the space, then skip this character
					buf[destIdx - 1] = static_cast<char>(0x08);
					spaces++;
					continue;
				case '\0': // null terminator, reached end of string
					buf[destIdx] = static_cast<char>(0xFF);
					buf[destIdx + 1] = static_cast<char>(0x00);
					successfulParse = true;
					break;
				default: // Skip character if unknown how to handle
					continue;
			}
			break; // Should only be reached if we break out of the switch instead of continue
		}
		if(!successfulParse){
			[[unlikely]] throw std::logic_error("Reached end of string without parsing a null terminator");
		}
	}

	[[nodiscard]] consteval std::size_t length() const noexcept{
		std::size_t chars = 0;
		for (std::size_t i = 0; i < size; i += 2) {
			chars++;
			if (buf[i] == 0xFF) { break; }
		}
		return chars;
	}

	[[nodiscard]] constexpr auto data() const noexcept{
		return buf.data();
	}
};

#define _COMBINE_TOKEN(name, token) name##token
#define COMBINE_TOKEN(name, token) _COMBINE_TOKEN(name, token)

#define SETUP_TRICK_STRINGS(NAME, BOARD1, BOARD2, BOARD3, BOARD4, BIKE1, BIKE2, BIKE3, BIKE4, SKATE1, SKATE2, SKATE3, SKATE4) \
constexpr auto board1 	= TrickString(BOARD1); \
constexpr auto board2 	= TrickString(BOARD2); \
constexpr auto board3 	= TrickString(BOARD3); \
constexpr auto board4 	= TrickString(BOARD4); \
constexpr auto bike1 	= TrickString(BIKE1); \
constexpr auto bike2 	= TrickString(BIKE2); \
constexpr auto bike3 	= TrickString(BIKE3); \
constexpr auto bike4 	= TrickString(BIKE4); \
constexpr auto skate1 	= TrickString(SKATE1); \
constexpr auto skate2 	= TrickString(SKATE2); \
constexpr auto skate3 	= TrickString(SKATE3); \
constexpr auto skate4 	= TrickString(SKATE4); \
ASMUsed constexpr std::array<const char *, 12> COMBINE_TOKEN(NAME,_stringptrs) = { \
 skate1.data(), skate2.data(), skate3.data(), skate4.data(), \
 bike1.data(), bike2.data(), bike3.data(), bike4.data(), \
 board1.data(), board2.data(), board3.data(), board4.data() \
}; \
ASMUsed constexpr std::array<const char, 12> COMBINE_TOKEN(NAME,_lengths) = { \
 skate1.length(), skate2.length(), skate3.length(), skate4.length(), \
 bike1.length(), bike2.length(), bike3.length(), bike4.length(), \
 board1.length(), board2.length(), board3.length(), board4.length() \
}
