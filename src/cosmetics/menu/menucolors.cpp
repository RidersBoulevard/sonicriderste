#include "menucolors.hpp"
#include "lib/stdlib.hpp"

constexpr std::array<RGBA, 11> CustomMenuColors = {{
    {0x7f, 0xff, 0xd4},	// Aquamarineblue
    {0x41, 0x69, 0xe1},	// Royalblue
    {0x99, 0x00, 0x00},	// CrimsonRed
    {0xFF, 0x00, 0xCB},	// Magenta
    {0xFF, 0xC0, 0xCB},	// Pink
    {0x8f, 0x00, 0xFF},	// Violet
    {0x22, 0x8B, 0x22},	// ForestGreen
    {0xFF, 0x70, 0x00},	// Orange
    {0xFF, 0xD7, 0x00},	// gold
    {0x5d, 0x43, 0x2c},	// WalnutBrown
    {0x00, 0x00, 0x00}	// black
}};

/**
 * Handles setting a custom main menu color.
 *
 * @param graphicalObject The main menu's graphical object.
 * @param object The main menu task's object data.
 */
ASMUsed void RandomizeCustomMenuColors(GraphicalObject *graphicalObject, MainMenuObject1 *object) {
    // originally coded by joe a tron as gecko code, ported to C++
	constexpr auto vanillaColorCount = 6;
	constexpr auto customColorCount = CustomMenuColors.size();
    const u32 random = lbl_RNG_Number(customColorCount + vanillaColorCount); // +6 to account for vanilla colors
    if (random >= vanillaColorCount) {
        object->colorPreset = 0x3; // set it to gray to tint it better
        graphicalObject->tint = CustomMenuColors[random - vanillaColorCount];
    }
}