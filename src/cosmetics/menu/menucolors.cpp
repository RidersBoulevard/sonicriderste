#include "menucolors.hpp"

constexpr RGBA CustomMenuColors[11] = {
    {0x7fffd4ff},	// Aquamarineblue
    {0x4169e1FF},	// Royalblue
    {0x990000FF},	// CrimsonRed
    {0xFF00CBFF},	// Magenta
    {0xFFC0CBFF},	// Pink
    {0x8f00FFFF},	// Violet
    {0x228B22FF},	// ForestGreen
    {0xFF7000FF},	// Orange
    {0xFFD700FF},	// gold
    {0x5d432cFF},	// WalnutBrown
    {0x000000FF}	// black
};

USED void RandomizeCustomMenuColors(GraphicalObject *graphicalObject, MainMenuObject1 *object) {
    // originally coded by joe a tron as gecko code, ported to C++
    u32 random = lbl_RNG_Number(11 + 6); // +6 to account for vanilla colors
    if (random >= 6) {
        object->colorPreset = 0x3; // set it to gray to tint it better
        graphicalObject->tint = CustomMenuColors[random - 6];
    }
}