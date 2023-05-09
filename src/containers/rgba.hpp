#pragma once

#include "main.hpp"

struct RGBA {
	union {
		u32 rgba;
		struct {
			u8 r;
			u8 g;
			u8 b;
			u8 a;
		} colors;
	};
	constexpr RGBA() = default;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
	constexpr RGBA(const u32 &rgba) : rgba(rgba){} // NOLINT(google-explicit-constructor)
#pragma GCC diagnostic pop

	constexpr operator u32() const { // NOLINT(google-explicit-constructor)
		return rgba;
	}
};