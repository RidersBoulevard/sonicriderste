#include "context.hpp"
#include "lib/lib.hpp"

struct StageIconObject1 {
    char filler[0x8];
    u32 state;
};

global void* lbl_10087ECC[2];

ASMUsed void StageIconInCSS(struct Object *object, struct StageIconObject1 *object1) {
    u32 textureID = 0x1, stage = CurrentStage, row, column;
    HUDStruct hud{};

    if (object->state == 0 && object1->state == 0x1) {
        hud.scale = 0.0078125f; // texture scale (1 / texture width [128 in this case])
        hud.screenX = 530;
        hud.screenY = 406;
        hud.textureXLength = 0x2A;
        hud.textureYLength = 0x2A;

        if (CurrentStage >= SkyRoad) {
            textureID = 0x1C;
            stage -= DarkDesert;
        } else if (CurrentStage >= NightChase) {
            // babylon stages
            textureID = 0x2;
            stage -= SegaCarnival;
        }

        stage -= 1;
        row = stage / 3;
        column = stage - (row * 3);

        hud.textureX = column * 0x2A;
        hud.textureY = row * 0x2A;

        Custom_CreateHUDElement(lbl_10087ECC[1], textureID, ~0U, &hud);
    }
}