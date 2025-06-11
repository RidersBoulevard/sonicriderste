#include "skinsystem.hpp"
#include "containers/graphicalobject.hpp"
#include "handlers/files/packman_handlers.hpp"
#include "lib/stdlib.hpp"

/// Contains all data of currently selected skins for each player at runtime.
std::array<SkinSystemData, MaxPlayerCount> PlayerSkinSystemData;

/**
 * Checks if the player is currently pressing any of the buttons designated to interact with the skin system.
 *
 * @param player The player to check.
 * @return The type of control the player pressed. Returns ::NoneSkinSystemType if no designated button was pressed.
 */
SkinSystemTypes CheckSkinSystemControls(const Player &player) {
    if (player.input->toggleFaceButtons.hasAny(Buttons::Y)) {
        return SkinSystemTypeForwards;
    }
    if (player.input->toggleFaceButtons.hasAny(Buttons::X)) {
        return SkinSystemTypeBackwards;
    }
    return NoneSkinSystemType;
}

/**
 * Checks if the currently selected skin is still applicable for the player.
 *
 * @param player The player to check.
 * @return true if applicable, false otherwise
 */
bool IsSkinApplicable(const Player &player) {
    const u8 &controllerPort = player.input->port;
    return PlayerSkinSystemInfo[PlayerSkinSystemData[controllerPort].skinID].character == player.character;
}

/**
 * Takes care of cycling through all available skins for a player.
 *
 * @param player The player to cycle skins for.
 * @param type The type to use in cycling.
 */
void ApplyNextSkin(const Player &player, SkinSystemTypes type) {
    const u8 &controllerPort = player.input->port;

    u32 index = PlayerSkinSystemData[controllerPort].skinID;
    while (true) {
        // NoneSkinSystemType has already been filtered out in this function
        if (type == SkinSystemTypeForwards) {
            index += 1;
            if (index >= PlayerSkinSystemInfo.size()) {
                index = 0;
            }
        } else { // backwards
            index -= 1;
            if (static_cast<s32>(index) < 0) {
                index = SkinCount - 1;
            }
        }

        const SkinSystemInfo &info = PlayerSkinSystemInfo[index];

        if (info.character == static_cast<u16>(-1)) {
            break;
        }
        if (info.character == player.character) {
            break;
        }
    }

    PlayerSkinSystemData[controllerPort].skinID = index;
    PlayerSkinSystemData[controllerPort].delay = 15;
}

/**
 * Checks if the player intends to cycle through available skins.
 * Also takes care of applying delay between button presses.
 *
 * @param player The player to check.
 */
void CheckSkinSystemIntent(const Player &player) {
    const u8 &controllerPort = player.input->port;
    u8 &delayTime = PlayerSkinSystemData[controllerPort].delay;

    if (delayTime == 0) {
        const SkinSystemTypes type = CheckSkinSystemControls(player);
        if (type != NoneSkinSystemType) {
            // apply next skin
            ApplyNextSkin(player, type);
        }
    } else {
        delayTime--;
    }

    //delayTime = static_cast<u8>(clamp<s8>(static_cast<s8>(delayTime), 0));
    //PlayerSkinSystemData[controllerPort].delay = delayTime;
}

/**
 * Checks if any of the buttons regarding the skin system are pressed, and if they're applicable.
 *
 * @param player The player to check the controls from.
 * @return true if buttons are pressed and are applicable, otherwise false
 */
[[nodiscard]] bool IsSkinSystemControlPressed(const Player &player) {
    return PlayerSkinSystemData[player.input->port].delay == 0 && CheckSkinSystemControls(player) != NoneSkinSystemType;
}

/**
 * Updates the textures on the CSS to portray the correct texture of the selected skin for players, if applicable.
 */
ASMUsed void Character_UpdateGraphicalSkins(GraphicalObject *object) {
    if (!object->active) { return; }

    const Player &player = players[object->idStruct.idIndex];

    CheckSkinSystemIntent(player);

    if (!IsSkinApplicable(player)) { return; }

    const u8 &controllerPort = player.input->port;
    const SkinSystemInfo *info = &PlayerSkinSystemInfo[PlayerSkinSystemData[controllerPort].skinID];
    object->textureIDs[0].textureSlot1 = info->cssTextureID;
}

/**
 * Gets the player's currently selected skin's texture archive index.
 *
 * @return -1 if no skin is applied, otherwise the texture archive index (>= 0).
 */
ASMUsed u32 FetchCurrentTextureArchiveIndex(const Player &player) {
    // returns the -1 from the first (invalid) skin info if no skin is applied
    u8 controllerPort = player.input->port;
    return PlayerSkinSystemInfo[PlayerSkinSystemData[controllerPort].skinID].textureArchiveIndex;
}

/**
 * Gets a pointer to the texture list of the player's currently applied skin.
 *
 * @note Only use this function when you are certain a player has a skin applied.
 */
ASMUsed void* FetchCurrentSkinTextures(const Player &player) {
    // Only use this function when you are certain a player has a skin applied.

    const auto &controllerPort = player.input->port;
    const SkinSystemInfo *info = &PlayerSkinSystemInfo[PlayerSkinSystemData[controllerPort].skinID];
    return CharacterSkinTextures[player.character][info->textureArchiveIndex];
}

/**
 * Takes care of making sure PlayerSkinSystemData is properly set and non-applicable skins for players are filtered out.
 */
ASMUsed void UpdateSkinSystemData(const Player &player) {
    if (!IsSkinApplicable(player)) {
        PlayerSkinSystemData[player.input->port].skinID = 0;
    }
}

/**
 * Clears PlayerSkinSystemData whenever necessary.
 */
ASMUsed void ClearSkinSystemData() {
    //memset(PlayerSkinSystemData.data(), 0, sizeof(PlayerSkinSystemData));
    std::ranges::fill(PlayerSkinSystemData, SkinSystemData{});
}

ASMUsed void ClearSpecificSkinSystemData(Player &player) {
    PlayerSkinSystemData[player.input->port].skinID = 0;
}