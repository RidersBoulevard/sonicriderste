/** \file os.cpp
 * Contains wrappers around OS functions from Dolphin SDK for backwards compatibility.
 */

#include "os.hpp"

#include <cstdlib>
#include <cstring>
#include <lib/stdlib.hpp>
#include <stdexcept>

#include "globalDefs.hpp"
#include "types.hpp"

#include <ogc/mutex.h>
#include <ogc/semaphore.h>
#include <ogc/system.h>
#include <ogc/lwp_watchdog.h>

#include "gx.hpp"

constexpr auto LWP_WORKSPACE_SIZE = 0x100000;
ASMDefined u32 gu32HeapInitialized;

ASMUsed void OS_LockMutex(const mutex_t *mutex) {
    LWP_MutexLock(*mutex);
}

ASMUsed void OS_UnlockMutex(const mutex_t *mutex) {
    LWP_MutexUnlock(*mutex);
}

ASMUsed void OS_InitSemaphore(sem_t *semaphore, const u32 start) {
    LWP_SemInit(semaphore, start, 0xFFFFFFFF);
}

ASMUsed void OS_SignalSemaphore(const sem_t *semaphore) {
    LWP_SemPost(*semaphore);
}

ASMUsed void OS_WaitSemaphore(const sem_t *semaphore) {
    LWP_SemWait(*semaphore);
}

ASMUsed u64 OS_GetTime() {
    return gettime();
}

ASMUsed void OS_TicksToCalendarTime(const u64 ticks, OSCalendarTime *td) {
    __OSTicksToCalendarTime(ticks, td);
}

ASMUsed void* OS_AllocFromHeap(const void *unused, u32 size) {
    (void)unused;
    return malloc(size);
}

ASMUsed void OS_FreeToHeap(const void *unused, void *ptr) {
    (void)unused;
    free(ptr);
}

ASMUsed u32 OS_CheckHeap() {
    return SYS_GetArena1Size();
}

ASMUsed void __SYS_SetAlarm(const syswd_t *alarm, u32 unused, u64 ticks, alarmcallback cb) {
    (void)unused;

    timespec time{};
    time.tv_sec = 0;
    time.tv_nsec = ticks_to_nanosecs(ticks);

    SYS_SetAlarm(*alarm, &time, cb, nullptr);
}

// Memory allocation functions are heavily unstable for some reason, so we're using the game's heap system instead.

ASMUsed void* __wrap_malloc(const size_t size) {
    if (gu32HeapInitialized == 0) { gNp_InitHeap(SYS_GetArenaLo(), SYS_GetArenaHi()); }

    return aligned_malloc(4, size);
}

ASMUsed void* __wrap_calloc(const size_t nmemb, const size_t size) {
    if (gu32HeapInitialized == 0) { gNp_InitHeap(SYS_GetArenaLo(), SYS_GetArenaHi()); }

    const size_t finalSize = nmemb * size;
    void *ptr = aligned_malloc(4, finalSize);
    memset(ptr, 0, finalSize);
    return ptr;
}

ASMUsed void __wrap_free(void* ptr) {
    nnFree(ptr);
}

/*
ASMUsed void __SYS_PreInit() {
    // need to account for the lwp workspace size because uhhhhh we will overwrite everything important
    gNp_InitHeap(SYS_GetArenaLo() + LWP_WORKSPACE_SIZE, SYS_GetArenaHi());
}
 */

ASMUsed void throw_invalid_ogc_function() {
    throw std::runtime_error("Invalid function called! This is not the libogc equivalent!");
}