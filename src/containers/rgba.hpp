#pragma once

#include "types.hpp"

struct RGBA {
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    u8 a = 0;

    constexpr RGBA(const u32 &rgba) {    // NOLINT(google-explicit-constructor)
        *this = std::bit_cast<RGBA>(rgba);
    }

    consteval RGBA(const int &rgba) : RGBA(
            static_cast<u32>(rgba)) {} // This takes care of the ambiguity from constructing with literals

    constexpr RGBA(u8 red = 0x00, u8 green = 0x00, u8 blue = 0x00, u8 alpha = 0xFF) : r(red), g(green), b(blue),
                                                                                      a(alpha) {}

    constexpr operator u32() const { // NOLINT(google-explicit-constructor)
        return std::bit_cast<u32>(*this);
    }

    constexpr operator bool() const { // NOLINT(google-explicit-constructor)
        return static_cast<u32>(*this) != 0;
    }

    // Boilerplate
    constexpr RGBA() = default;

    constexpr RGBA(const RGBA &) = default;

    constexpr RGBA(RGBA &&) = default;

    constexpr ~RGBA() = default;

    constexpr RGBA &operator=(const RGBA &) = default;

    constexpr RGBA &operator=(RGBA &&) = default;
};

consteval RGBA operator ""_rgba(u64 rgba) {
    return {static_cast<u32>(rgba)};
}

struct RGBANormalized {
    f32 r = 0;
    f32 g = 0;
    f32 b = 0;
    f32 a = 0;

    constexpr RGBANormalized(u8 red, u8 green, u8 blue, u8 alpha = 0xFF) : // NOLINT(google-explicit-constructor)
            r(static_cast<f32>(red) / 255.0F),
            g(static_cast<f32>(green) / 255.0F),
            b(static_cast<f32>(blue) / 255.0F),
            a(static_cast<f32>(alpha) / 255.0F) {}

    constexpr RGBANormalized(bool negative = false) { // NOLINT(google-explicit-constructor)
        const f32 setValue = (negative) ? -1.0F : 0.0F;
        this->r = setValue;
        this->g = setValue;
        this->b = setValue;
        this->a = setValue;
    }

    constexpr operator bool() const { // NOLINT(google-explicit-constructor)
        return this->a != -1.0f && this->a != 0.0f;
    }

    constexpr static RGBANormalized fromNormalized(f32 red, f32 green, f32 blue, f32 alpha = 1.0F) {
        auto rgba = RGBANormalized(false);
        rgba.r = red;
        rgba.g = green;
        rgba.b = blue;
        rgba.a = alpha;
        return rgba;
    }
};