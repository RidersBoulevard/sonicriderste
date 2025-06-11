#include "files.hpp"

#include <handlers/music/custom_music.hpp>

#include "globalDefs.hpp"
#include "types.hpp"
#include "stdlib.hpp"
#include <ogc/dvd.h>
#include <ogc/system.h>
#include <ogc/video.h>
#include "alignment.hpp"

ASMUsed void* DumpFile(const char* path, const bool unknownAlwaysTrue) {
    dvdfileinfo fileInfo;
    if (!DVD_Open(path, &fileInfo)) {
        return nullptr;
    }

    const u32 len = align_val<u32>(fileInfo.len, 32);
    void* buf = aligned_malloc(32, len);

    DVD_ReadAbsAsyncPrio(&fileInfo.block, buf, len, fileInfo.addr, nullptr, 2);

    while (true) {
        if (const s32 cmdblkStatus = DVD_GetCmdBlockStatus(&fileInfo.block);
            cmdblkStatus == DVD_STATE_END) {
            break;
        }

        if (!unknownAlwaysTrue) {
            continue;
        }

        const s32 status = DVD_GetDriveStatus();
        u32 currState = 0;

        switch (status) {
            case DVD_STATE_FATAL_ERROR:
                currState = 1;
                break;
            case DVD_STATE_NO_DISK:
                currState = 2;
                break;
            case DVD_STATE_COVER_OPEN:
                currState = 3;
                lbl_8021BB98 = 480.0f;
                break;
            case DVD_STATE_WRONG_DISK:
                currState = 4;
                break;
            case DVD_STATE_IGNORED:
                currState = 5;
                break;
            case DVD_STATE_RETRY:
                currState = 6;
                break;
            default:
                break;
        }

        func_800090D8(currState, 0, 0);

        if (currState == 1) {
            continue;
        }

        const bool oldResetButton = lbl_8021BB9C;
        const bool newResetButton = static_cast<bool>(SYS_ResetButtonDown());
        lbl_8021BB9C = newResetButton;
        if (!oldResetButton || newResetButton) {
            continue;
        }

        if (lbl_8021BB61) {
            SNDF_SendSystemControl(0, 0xA0000100, 0);
            SNDFGC_Se3dSendSystemControl(0, 0xA0000500, 0);
        }

        if (lbl_8021BB62) {
            ADXT_Stop(gpasAdxtHandle_Bgm);
            ADXT_Stop(lbl_8021BC0C);
            ADXT_Stop(lbl_8021BC08);
        }

        VIDEO_Flush();
        VIDEO_SetBlack(true);
        VIDEO_Flush();
        VIDEO_WaitVSync();
        SYS_ResetSystem(0, 0, 0);
    }

    return buf;
}
