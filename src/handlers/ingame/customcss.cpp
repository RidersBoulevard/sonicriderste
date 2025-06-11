#include "customcss.hpp"
#include "cosmetics/player/exloads.hpp"
#include "cosmetics/player/skinsystem.hpp"
#include "riders/controller.hpp"
#include "lib/sound.hpp"

std::bitset<4> PlayerCharacterInformationBox;

ASMUsed void SetPlayerInformationBox(u32 index, const CSSObject& object) {
    if (object.cssSelectionState[index] != 1) { return; }

    const Controller &input = GameControllers[index];
    if (input.toggleFaceButtons.hasAny(Buttons::Z)) {
        const bool isBoxOpen = !PlayerCharacterInformationBox[index];
        PlayerCharacterInformationBox[index] = isBoxOpen;
        if (isBoxOpen) {
            PlayAudioFromDAT(Sound::VSFX::MenuConfirm);
        } else {
            PlayAudioFromDAT(Sound::VSFX::MenuBack);
        }
    }
}

ASMUsed bool CheckPlayerInformationBox(const u32 index, const CSSObject& object) {
    return object.cssSelectionState[index] == 1 && PlayerCharacterInformationBox[index];
}

ASMUsed void ClearPlayerInformationBox() {
	PlayerCharacterInformationBox.reset();
    //memset(&PlayerCharacterInformationBox, 0, sizeof(PlayerCharacterInformationBox));
}

inline void ApplyVisual(CSSObject &object) {
    object.visualTrigger[0] = TRUE; // object that is passed in is already aligned to the player index, no need to apply index in here
    object.visualTriggerTime = 1;
    lbl_0013FA84(0xA9000000);
    lbl_0013FA84(0xA9000800);
}

/**
 * Applies the fading effect and the sound effects that go along with it on the CSS that happen when you scroll.
 * It applies them based on custom conditions (like the EX load system).
 *
 * @param player The player to apply the effect for.
 * @param object The associated CSS object. This needs to be already aligned to the player index!
 */
ASMUsed void CSSApplyCustomVisualTrigger(const Player &player, CSSObject &object) {
    const auto exLoadControls = IsEXLoadControlPressed(player, object);
    const auto skinSystemControls = IsSkinSystemControlPressed(player);

    if (exLoadControls || skinSystemControls) {
        ApplyVisual(object);
    }
}