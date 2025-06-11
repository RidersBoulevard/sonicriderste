#include "customcss.hpp"
#include "cosmetics/player/exloads.hpp"
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