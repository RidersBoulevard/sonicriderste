#include "macros.S"

.section .init, "ax"  # 0x80003100 - 0x800055E0

.balign 8

.global OSSetThreadPriority // LWP_SetThreadPriority
OSSetThreadPriority:
.global OSGetThreadPriority // LWP_GetThreadPriority
OSGetThreadPriority:
.global OSCreateThread // LWP_CreateThread
OSCreateThread:
.global OSResumeThread // LWP_ResumeThread
OSResumeThread:
.global OSCancelThread // LWP_CancelThread
OSCancelThread:
.global OSGetCurrentThread // LWP_GetSelf
OSGetCurrentThread:
.global OSSuspendThread // LWP_SuspendThread
OSSuspendThread: // note for this one, most likely won't behave properly

b throw_invalid_ogc_function