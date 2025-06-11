#include "randomstage.hpp"
#include "lib/sound.hpp"
#include "lib/stdlib.hpp"

ASMUsed void RandomStageObj1Handler(StageSelectObject2* courseSelectObj1) {
    if (!gsActivePad.toggleButtons.hasAny(Buttons::R)) { return; }

    const u32 randomStage = lbl_RNG_Number(16);
    u32 selectedSquare;

    // calculate whether the random stage is a babylon stage or not (1 being a babylon stage)
    const u32 subStage = static_cast<u32>(randomStage / 8);

    if (randomStage >= 8) {
        // if it's a babylon stage, calculate selected stage square properly
        selectedSquare = randomStage % 8;
    } else {
        // otherwise, no need to calculate anything
        selectedSquare = randomStage;
    }

    courseSelectObj1->selectedStageSquare = selectedSquare;
    courseSelectObj1->selectedSubMenuStage = subStage;
	vu32& currentStage = const_cast<vu32&>(CurrentStage);
	currentStage = selectedSquare + 1;
    courseSelectObj1->subMenuControl = 2;
    courseSelectObj1->subMenuStatus = 1;

    u32 page;
    u32 cursorPosition;

    if (selectedSquare >= 4) {
        cursorPosition = 4;
        page = selectedSquare % 4;
    } else {
        cursorPosition = selectedSquare;
        page = 0;
    }

    courseSelectObj1->cursorPosition = cursorPosition;
    courseSelectObj1->currentPage = page;
}

ASMUsed void RandomStageObjHandler(ObjectNode & courseSelectObj) {
    if (!gsActivePad.toggleButtons.hasAny(Buttons::R)) { return; }

    PlayAudioFromDAT(Sound::VSFX::MenuConfirm);

    courseSelectObj.state = 5;
}