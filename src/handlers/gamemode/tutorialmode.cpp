#include "tutorialmode.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"
#include "riders/stage.hpp"
#include "riders/player.hpp"
#include "riders/object.hpp"
#include "riders/gamemode.hpp"
#include "handlers/player/SetPlayerState.hpp"
#include <cstring>

ASMDefined ObjectNode *gpsCurrentTask;

/**
 * Attempts to initialize the tutorial mode upon pressing the button on the main menu for it.
 *
 * @param object The object node to the title sequence object.
 * @return true if tutorial mode was selected (whether it failed or not), false if otherwise.
 */
ASMUsed bool TryInitTutorialMode(ObjectNode &object) {
    auto *object1 = static_cast<TitleSequenceObject1 *>(object.object);
    if (object1->currentButtonIndex != 3) {
        return false;
    }

    Controller *firstPort = nullptr;

    // assign first port
    for (auto &controller: GameControllers) {
        if (controller.initStatus) {
            firstPort = &controller;
            break;
        }
    }

    // return early if no controller is plugged in (player input couldn't be initialized)
    if (firstPort == nullptr) {
        return true;
    }

    // set the settings
    auto *player = players.data();
    std::memset(player, 0, sizeof(Player));
    object.state = 0;
    object1->chosenGamemode = 10;
    CurrentStage = TestStage;
    InGamePlayerCount = 1;
    gu32ViewportNum = 1;
    player->input = firstPort;
    player->character = Character::Sonic;
    player->attributes = Character::Sonic;
    player->extremeGear = ExtremeGear::Default;

    return true;
}

/**
 * Specifically used on the ASM side to get the tutorial mode's gamemode enum.
 *
 * @return The gamemode enum.
 */
ASMUsed u32 GetTutorialGamemodeEnum() {
    return 750;
}

ASMDefined void InitGame();
ASMDefined void lbl_0000FF60();
ASMDefined void MovePauseMode(ObjectNode *objectNode, void *object1, u32 someId);
ASMDefined AllPlayerInputs AllPlayerInput;

ASMUsed void TutorialCtrl_Task() {
    lbl_0000FF60();

    auto *object = gpsCurrentTask;

    switch (object->state) {
        case 2:
            // init
            InitGame();

            geGame_ModeDetail = GetTutorialGamemodeEnum() + 2;

            object->state++;
            break;
        case 3: {
            auto *player = players.data();

            if (AllPlayerInput.toggleButtons.hasAny(Buttons::Start)) {
                MovePauseMode(object, object->object, 0xE);
            } else if (player->currentLap != 0) {
                geGame_ModeDetail = GetTutorialGamemodeEnum() + 3;
                object->state++;
            }
            break;
        }
        default:
            break;
    }
}