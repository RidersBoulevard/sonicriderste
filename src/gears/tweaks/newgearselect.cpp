#include "newgearselect.hpp"

// The amount of custom Gears we have for each Gear Type:
constexpr u8 customBoardAmount = 1;
constexpr u8 customBikeAmount = 1;
constexpr u8 customSkateAmount = 1;

constexpr std::array<u8, customBoardAmount> newBoardIDs = { // added boards
    ExtremeGear::GunGear,
    };

constexpr std::array<u8, customBikeAmount> newBikeIDs = { // added bikes
    0xFF,
    };

constexpr std::array<u8, customSkateAmount> newSkateIDs = { // added skates
    0xFF,
    };

/**
 * Essentially a replacement for lbl_001415D8. Determines the next Gear ID when the player presses RIGHT on Gear select.
 * Includes functionality for new Gears, putting them with their correct Gear Type, and freely shuffling of custom Gears.
 *
 * @param currentGearID The Gear ID the player is currently on. This is already stored in r3 in ASM.
 * @return The new Gear ID.
 */
ASMUsed u8 CustomGearSelect_Forward(u8 currentGearID) {
    u8 newGearID = 0;

    // If we are on the last custom Gear, go to the first vanilla Gear of the next Gear Type.

    if (currentGearID == newBoardIDs[customBoardAmount - 1]) {
        return ExtremeGear::ERider;
    }

    if (currentGearID == newBikeIDs[customBikeAmount - 1]) {
        return ExtremeGear::Darkness;
    }

    if (currentGearID == newSkateIDs[customSkateAmount - 1]) {
        return ExtremeGear::Default;
    }

    // If we are on a custom Gear, go to the next custom Gear instead.

    for (u8 i= 0; i < customBoardAmount; i++) {
        if (currentGearID == newBoardIDs[i]) {
            newGearID = newBoardIDs[i + 1];
            return newGearID;
        }
    }

    for (u8 i= 0; i < customBikeAmount; i++) {
        if (currentGearID == newBikeIDs[i]) {
            newGearID = newBikeIDs[i + 1];
            return newGearID;
        }
    }

    for (u8 i= 0; i < customSkateAmount; i++) {
        if (currentGearID == newSkateIDs[i]) {
            newGearID = newSkateIDs[i + 1];
            return newGearID;
        }
    }

    // Finally, handle vanilla Gears

    switch (currentGearID) {
        case ExtremeGear::Berserker: // Last OG board.
            newGearID = newBoardIDs[0];
            break;
        case ExtremeGear::SuperHangOn: // Last OG bike. 
            // newGearID = newBikeIDs[0];
            newGearID = ExtremeGear::Darkness; // SYB: Delete this and uncomment the line above once you add a custom bike!
            break;
        case ExtremeGear::Cannonball: // Last OG skate.
            // newGearID = newSkateIDs[0];
            newGearID = ExtremeGear::Default; // SYB: Delete this and uncomment the line above once you add a custom skate!
            break;
        default:
            newGearID = currentGearID + 1;
            break;
    }
    return newGearID;
}

/**
 * Essentially a replacement for lbl_001415EC. Determines the next Gear ID when the player presses LEFT on Gear select.
 * Includes functionality for new Gears, putting them with their correct Gear Type, and freely shuffling of custom Gears.
 *
 * @param currentGearID The Gear ID the player is currently on. This is already stored in r3 in ASM.
 * @return The new Gear ID.
 */
ASMUsed u8 CustomGearSelect_Backward(u8 currentGearID) {
    u8 newGearID = 0;

    // If we are on the first custom Gear, go to the last vanilla Gear of the previous Gear Type.

    if (currentGearID == newBoardIDs[0]) {
        return ExtremeGear::Berserker;
    }

    else if (currentGearID == newBikeIDs[0]) {
        return ExtremeGear::SuperHangOn;
    }

    else if (currentGearID == newSkateIDs[0]) {
        return ExtremeGear::Cannonball;
    }

    // If we are on a custom Gear, go to the previous custom Gear instead.

    for (u8 i= 0; i < customBoardAmount; i++) {
        if (currentGearID == newBoardIDs[i]) {
            newGearID = newBoardIDs[i - 1];
            return newGearID;
        }
    }

    for (u8 i= 0; i < customBikeAmount; i++) {
        if (currentGearID == newBikeIDs[i]) {
            newGearID = newBikeIDs[i - 1];
            return newGearID;
        }
    }

    for (u8 i= 0; i < customSkateAmount; i++) {
        if (currentGearID == newSkateIDs[i]) {
            newGearID = newSkateIDs[i - 1];
            return newGearID;
        }
    }

    // Finally, handle vanilla Gears

    switch (currentGearID) {
        case ExtremeGear::Darkness: // First OG skate.
            // newGearID = newBikeIDs[customBikeAmount - 1];
            newGearID = ExtremeGear::SuperHangOn; // SYB: Delete this and uncomment the line above once you add a custom bike!
            break;
        case ExtremeGear::ERider: // First OG bike.
            newGearID = newBoardIDs[customBoardAmount - 1];
            break;
        case ExtremeGear::Default: // First OG board.
            // newGearID = newSkateIDs[customSkateAmount - 1];
            newGearID = ExtremeGear::Cannonball; // SYB: Delete this and uncomment the line above once you add a custom bike!
            break;
        default:
            newGearID = currentGearID - 1;
            break;
    }
    return newGearID;
}