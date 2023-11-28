#include "load_deduplicated.hpp"
#include "riders/stage.hpp"
#include "riders/gamemode.hpp"
#include <cstdio>

extern const vu32 gu32ViewportNum;

inline void LoadPackManFileGeneral(const char *fileName) {
    SetArchiveBinary(DumpPackManFile(fileName, 0), 0, 0);
}

ASMUsed void LoadDeduplicatedStage() {
    char fileName[8];

    // load common files for every stage
    LoadPackManFileGeneral("CMN");

    if (CurrentGameMode == MissionMode) {
        LoadPackManFileGeneral("MCMN");
    } else if (CurrentGameMode == TagMode) {
        LoadPackManFileGeneral("TCMN");
    }

    // load specific assets depending on player count ("CMN1", "CMN2", "CMN4")
    auto playerCount = gu32ViewportNum;
    if (playerCount >= 3) { playerCount = 4; }

    sprintf(fileName, "CMN%d", playerCount);
    LoadPackManFileGeneral(fileName);

    // load specific stage's common file, like "1CMN"
    sprintf(fileName, "%dCMN", CurrentStage);
    LoadPackManFileGeneral(fileName);
}