#include "macros.S"

.section .sbss  # 0x80125E00 - 0x8021CFA4

.balign 8

.global __sbss_rom_start__
__sbss_rom_start__:

.global lbl_8021BB40
lbl_8021BB40:
	.skip 0x4
.global lbl_8021BB44
lbl_8021BB44:
	.skip 0xC
.global gsActivePad
gsActivePad:
	.skip 0x4
.global lbl_8021BB54
lbl_8021BB54:
	.skip 0xC
.global lbl_8021BB60
lbl_8021BB60:
	.skip 0x1
.global lbl_8021BB61
lbl_8021BB61:
	.skip 0x1
.global lbl_8021BB62
lbl_8021BB62:
	.skip 0x1
.global lbl_8021BB63
lbl_8021BB63:
	.skip 0x1
.global gu32GameCnt
gu32GameCnt:
	.skip 0x4
.global lbl_8021BB68
lbl_8021BB68:
	.skip 0x4
.global lbl_8021BB6C
lbl_8021BB6C:
	.skip 0x4
.global lbl_8021BB70
lbl_8021BB70:
	.skip 0x4
.global gu32MultiFlag
gu32MultiFlag:
	.skip 0x4
.global lbl_8021BB78
lbl_8021BB78:
	.skip 0x4
.global gu32_EnableSystem30Frame
gu32_EnableSystem30Frame:
	.skip 0x4
.global gu32_30MoveOnlyFrame
gu32_30MoveOnlyFrame:
	.skip 0x4
.global lbl_8021BB84
lbl_8021BB84:
	.skip 0x4
.global gu32Game_PauseFlag
gu32Game_PauseFlag:
	.skip 0x4
.global lbl_8021BB8C
lbl_8021BB8C:
	.skip 0x4
.global gpsTexList_Residence
gpsTexList_Residence:
	.skip 0x4
.global gvpDrawFunc_ReadAyyncManager
gvpDrawFunc_ReadAyyncManager:
	.skip 0x4
.global lbl_8021BB98
lbl_8021BB98:
	.skip 0x4
.global lbl_8021BB9C
lbl_8021BB9C:
	.skip 0x1
.global lbl_8021BB9D
lbl_8021BB9D:
	.skip 0x3
.global lbl_8021BBA0
lbl_8021BBA0:
	.skip 0x4
.global lbl_8021BBA4
lbl_8021BBA4:
	.skip 0x4
.global lbl_8021BBA8
lbl_8021BBA8:
	.skip 0x18
.global gf32Motion_1Frame_SystemHz
gf32Motion_1Frame_SystemHz:
	.skip 0x4
.global gf32MapMotion_1Frame
gf32MapMotion_1Frame:
	.skip 0x4
.global gf32Motion_1Frame
gf32Motion_1Frame:
	.skip 0x4
.global lbl_8021BBCC
lbl_8021BBCC:
	.skip 0x20
.global gf32Game_TimerAdd1
gf32Game_TimerAdd1:
	.skip 0x4
.global gf32Game_PauseAdd1
gf32Game_PauseAdd1:
	.skip 0x4
.global gf32_SystemFrameRate_Add1
gf32_SystemFrameRate_Add1:
	.skip 0x4
.global gsNnMtxStack
gsNnMtxStack:
	.skip 0x10
.global lbl_8021BC08
lbl_8021BC08:
	.skip 0x4
.global lbl_8021BC0C
lbl_8021BC0C:
	.skip 0x4
.global gpasAdxtHandle_Bgm
gpasAdxtHandle_Bgm:
	.skip 0x4
.global lbl_8021BC14
lbl_8021BC14:
	.skip 0x1C
.global lbl_8021BC30
lbl_8021BC30:
	.skip 0x8
.global OS_BootInfo
OS_BootInfo:
	.skip 0x4
.global BI2DebugFlag
BI2DebugFlag:
	.skip 0x4
.global BI2DebugFlagHolder
BI2DebugFlagHolder:
	.skip 0x4
.global __OSIsGcam
__OSIsGcam:
	.skip 0x4
.global ZeroF
ZeroF:
	.skip 0x8
.global ZeroPS
ZeroPS:
	.skip 0x8
.global AreWeInitialized
AreWeInitialized:
	.skip 0x4
.global OSExceptionTable
OSExceptionTable:
	.skip 0x4
.global __OSInIPL
__OSInIPL:
	.skip 0x8
.global __OSStartTime
__OSStartTime:
	.skip 0x4
.global __OSStartTimeplus0x4
__OSStartTimeplus0x4:
	.skip 0x4
.global AlarmQueue
AlarmQueue:
	.skip 0x8
.global HeapArray
HeapArray:
	.skip 0x4
.global NumHeaps
NumHeaps:
	.skip 0x4
.global ArenaStart
ArenaStart:
	.skip 0x4
.global ArenaEnd
ArenaEnd:
	.skip 0x4
.global __OSArenaHi
__OSArenaHi:
	.skip 0x8
.global DOLPHINSDK_Prepared
DOLPHINSDK_Prepared:
	.skip 0x4
.global apploaderPosition
apploaderPosition:
	.skip 0x4
.global FontDataAnsi
FontDataAnsi:
	.skip 0x4
.global FontDataSjis
FontDataSjis:
	.skip 0x4
.global ParseString
ParseString:
	.skip 0x8
.global InterruptHandlerTable
InterruptHandlerTable:
	.skip 0x4
.global __OSLastInterruptSrr0
__OSLastInterruptSrr0:
	.skip 0x4
.global __OSLastInterrupt
__OSLastInterrupt:
	.skip 0x8
.global __OSLastInterruptTime
__OSLastInterruptTime:
	.skip 0x4
.global __OSLastInterruptTimeplus0x4
__OSLastInterruptTimeplus0x4:
	.skip 0x4
.global SaveStart
SaveStart:
	.skip 0x4
.global SaveEnd
SaveEnd:
	.skip 0x4
.global ResetFunctionQueue
ResetFunctionQueue:
	.skip 0x8
.global bootThisDol
bootThisDol:
	.skip 0x8
.global ResetCallback
ResetCallback:
	.skip 0x4
.global __Down
__Down:
	.skip 0x4
.global __LastState
__LastState:
	.skip 0x8
.global lbl_8021BCE8
lbl_8021BCE8:
	.skip 0x4
.global lbl_8021BCEC
lbl_8021BCEC:
	.skip 0x4
.global __HoldDown
__HoldDown:
	.skip 0x4
.global __HoldDownplus0x4
__HoldDownplus0x4:
	.skip 0x4
.global RunQueueBits
RunQueueBits:
	.skip 0x4
.global RunQueueHint
RunQueueHint:
	.skip 0x4
.global DOLPHINSDK_Reschedule
DOLPHINSDK_Reschedule:
	.skip 0x8
.global lbl_8021BD08
lbl_8021BD08:
	.skip 0x8
.global __DBInterface
__DBInterface:
	.skip 0x4
.global DBVerbose
DBVerbose:
	.skip 0x4
.global DVD_StopAtNextInt
DVD_StopAtNextInt:
	.skip 0x4
.global DVD_LastLength
DVD_LastLength:
	.skip 0x4
.global DVD_Callback
DVD_Callback:
	.skip 0x4
.global DVD_ResetCoverCallback
DVD_ResetCoverCallback:
	.skip 0x4
.global DVD_LastResetEnd
DVD_LastResetEnd:
	.skip 0x4
.global DVD_LastResetEndplus0x4
DVD_LastResetEndplus0x4:
	.skip 0x4
.global DVD_ResetOccurred
DVD_ResetOccurred:
	.skip 0x4
.global DVD_WaitingCoverClose
DVD_WaitingCoverClose:
	.skip 0x4
.global DVD_Breaking
DVD_Breaking:
	.skip 0x4
.global DVD_WorkAroundType
DVD_WorkAroundType:
	.skip 0x4
.global DVD_WorkAroundSeekLocation
DVD_WorkAroundSeekLocation:
	.skip 0x8
.global DVD_LastReadFinished
DVD_LastReadFinished:
	.skip 0x4
.global DVD_LastReadFinishedplus0x4
DVD_LastReadFinishedplus0x4:
	.skip 0x4
.global DVD_LastReadIssued
DVD_LastReadIssued:
	.skip 0x4
.global DVD_LastReadIssuedplus0x4
DVD_LastReadIssuedplus0x4:
	.skip 0x4
.global DVD_LastCommandWasRead
DVD_LastCommandWasRead:
	.skip 0x4
.global DVD_NextCommandNumber
DVD_NextCommandNumber:
	.skip 0x4
.global DVD_BootInfo
DVD_BootInfo:
	.skip 0x4
.global DVD_FstStart
DVD_FstStart:
	.skip 0x4
.global DVD_FstStringStart
DVD_FstStringStart:
	.skip 0x4
.global DVD_MaxEntryNum
DVD_MaxEntryNum:
	.skip 0x4
.global DVD_currentDirectory
DVD_currentDirectory:
	.skip 0x4
.global __DVDLongFileNameFlag
__DVDLongFileNameFlag:
	.skip 0x4
.global __DVDThreadQueue
__DVDThreadQueue:
	.skip 0x8
.global DVD_executing
DVD_executing:
	.skip 0x4
.global DVD_IDShouldBe
DVD_IDShouldBe:
	.skip 0x4
.global DVD_bootInfo
DVD_bootInfo:
	.skip 0x4
.global lbl_8021BD8C
lbl_8021BD8C:
	.skip 0x4
.global lbl_8021BD90
lbl_8021BD90:
	.skip 0x4
.global lbl_8021BD94
lbl_8021BD94:
	.skip 0x4
.global DVD_FatalErrorFlag
DVD_FatalErrorFlag:
	.skip 0x4
.global lbl_8021BD9C
lbl_8021BD9C:
	.skip 0x4
.global lbl_8021BDA0
lbl_8021BDA0:
	.skip 0x4
.global lbl_8021BDA4
lbl_8021BDA4:
	.skip 0x4
.global lbl_8021BDA8
lbl_8021BDA8:
	.skip 0x4
.global lbl_8021BDAC
lbl_8021BDAC:
	.skip 0x4
.global DVD_LastError
DVD_LastError:
	.skip 0x4
.global DVD_NumInternalRetry
DVD_NumInternalRetry:
	.skip 0x4
.global lbl_8021BDB8
lbl_8021BDB8:
	.skip 0x4
.global DVD_FirstTimeInBootrom
DVD_FirstTimeInBootrom:
	.skip 0x4
.global DVD_MotorState
DVD_MotorState:
	.skip 0x4
.global DVDInitialized
DVDInitialized:
	.skip 0x4
.global DVD_LastState
DVD_LastState:
	.skip 0x8
.global lbl_8021BDD0
lbl_8021BDD0:
	.skip 0x8
.global lbl_8021BDD8
lbl_8021BDD8:
	.skip 0x4
.global lbl_8021BDDC
lbl_8021BDDC:
	.skip 0x4
.global lbl_8021BDE0
lbl_8021BDE0:
	.skip 0x8
.global lbl_8021BDE8
lbl_8021BDE8:
	.skip 0x4
.global lbl_8021BDEC
lbl_8021BDEC:
	.skip 0x4
.global lbl_8021BDF0
lbl_8021BDF0:
	.skip 0x4
.global lbl_8021BDF4
lbl_8021BDF4:
	.skip 0x8
.global VI_PreCB
VI_PreCB:
	.skip 0x4
.global VI_PostCB
VI_PostCB:
	.skip 0x4
.global lbl_8021BE04
lbl_8021BE04:
	.skip 0x4
.global lbl_8021BE08
lbl_8021BE08:
	.skip 0x4
.global lbl_8021BE0C
lbl_8021BE0C:
	.skip 0x2
.global lbl_8021BE0E
lbl_8021BE0E:
	.skip 0x2
.global lbl_8021BE10
lbl_8021BE10:
	.skip 0x8
.global lbl_8021BE18
lbl_8021BE18:
	.skip 0x4
.global lbl_8021BE1C
lbl_8021BE1C:
	.skip 0x4
.global lbl_8021BE20
lbl_8021BE20:
	.skip 0x8
.global lbl_8021BE28
lbl_8021BE28:
	.skip 0x4
.global lbl_8021BE2C
lbl_8021BE2C:
	.skip 0x4
.global lbl_8021BE30
lbl_8021BE30:
	.skip 0x4
.global lbl_8021BE34
lbl_8021BE34:
	.skip 0x4
.global lbl_8021BE38
lbl_8021BE38:
	.skip 0x4
.global lbl_8021BE3C
lbl_8021BE3C:
	.skip 0x4
.global lbl_8021BE40
lbl_8021BE40:
	.skip 0x4
.global lbl_8021BE44
lbl_8021BE44:
	.skip 0x4
.global lbl_8021BE48
lbl_8021BE48:
	.skip 0x8
.global lbl_8021BE50
lbl_8021BE50:
	.skip 0x4
.global lbl_8021BE54
lbl_8021BE54:
	.skip 0x4
.global lbl_8021BE58
lbl_8021BE58:
	.skip 0x4
.global lbl_8021BE5C
lbl_8021BE5C:
	.skip 0x4
.global lbl_8021BE60
lbl_8021BE60:
	.skip 0x4
.global lbl_8021BE64
lbl_8021BE64:
	.skip 0x4
.global lbl_8021BE68
lbl_8021BE68:
	.skip 0x4
.global lbl_8021BE6C
lbl_8021BE6C:
	.skip 0x4
.global lbl_8021BE70
lbl_8021BE70:
	.skip 0x4
.global lbl_8021BE74
lbl_8021BE74:
	.skip 0x4
.global __PADSpec
__PADSpec:
	.skip 0x8
.global lbl_8021BE80
lbl_8021BE80:
	.skip 0x4
.global lbl_8021BE84
lbl_8021BE84:
	.skip 0x4
.global lbl_8021BE88
lbl_8021BE88:
	.skip 0x4
.global lbl_8021BE8C
lbl_8021BE8C:
	.skip 0x4
.global lbl_8021BE90
lbl_8021BE90:
	.skip 0x4
.global lbl_8021BE94
lbl_8021BE94:
	.skip 0x4
.global lbl_8021BE98
lbl_8021BE98:
	.skip 0x4
.global lbl_8021BE9C
lbl_8021BE9C:
	.skip 0x4
.global lbl_8021BEA0
lbl_8021BEA0:
	.skip 0x4
.global lbl_8021BEA4
lbl_8021BEA4:
	.skip 0x4
.global lbl_8021BEA8
lbl_8021BEA8:
	.skip 0x4
.global lbl_8021BEAC
lbl_8021BEAC:
	.skip 0x4
.global lbl_8021BEB0
lbl_8021BEB0:
	.skip 0x4
.global lbl_8021BEB4
lbl_8021BEB4:
	.skip 0x4
.global lbl_8021BEB8
lbl_8021BEB8:
	.skip 0x4
.global lbl_8021BEBC
lbl_8021BEBC:
	.skip 0x4
.global lbl_8021BEC0
lbl_8021BEC0:
	.skip 0x4
.global lbl_8021BEC4
lbl_8021BEC4:
	.skip 0x4
.global lbl_8021BEC8
lbl_8021BEC8:
	.skip 0x4
.global lbl_8021BECC
lbl_8021BECC:
	.skip 0x4
.global lbl_8021BED0
lbl_8021BED0:
	.skip 0x4
.global lbl_8021BED4
lbl_8021BED4:
	.skip 0x4
.global lbl_8021BED8
lbl_8021BED8:
	.skip 0x4
.global lbl_8021BEDC
lbl_8021BEDC:
	.skip 0x4
.global lbl_8021BEE0
lbl_8021BEE0:
	.skip 0x4
.global lbl_8021BEE4
lbl_8021BEE4:
	.skip 0x4
.global lbl_8021BEE8
lbl_8021BEE8:
	.skip 0x4
.global lbl_8021BEEC
lbl_8021BEEC:
	.skip 0x4
.global lbl_8021BEF0
lbl_8021BEF0:
	.skip 0x4
.global lbl_8021BEF4
lbl_8021BEF4:
	.skip 0x4
.global lbl_8021BEF8
lbl_8021BEF8:
	.skip 0x4
.global lbl_8021BEFC
lbl_8021BEFC:
	.skip 0x4
.global lbl_8021BF00
lbl_8021BF00:
	.skip 0x4
.global lbl_8021BF04
lbl_8021BF04:
	.skip 0x4
.global lbl_8021BF08
lbl_8021BF08:
	.skip 0x8
.global lbl_8021BF10
lbl_8021BF10:
	.skip 0x4
.global lbl_8021BF14
lbl_8021BF14:
	.skip 0x4
.global lbl_8021BF18
lbl_8021BF18:
	.skip 0x4
.global lbl_8021BF1C
lbl_8021BF1C:
	.skip 0x4
.global lbl_8021BF20
lbl_8021BF20:
	.skip 0x4
.global lbl_8021BF24
lbl_8021BF24:
	.skip 0x4
.global lbl_8021BF28
lbl_8021BF28:
	.skip 0x4
.global lbl_8021BF2C
lbl_8021BF2C:
	.skip 0x4
.global lbl_8021BF30
lbl_8021BF30:
	.skip 0x4
.global lbl_8021BF34
lbl_8021BF34:
	.skip 0x4
.global lbl_8021BF38
lbl_8021BF38:
	.skip 0x4
.global lbl_8021BF3C
lbl_8021BF3C:
	.skip 0x4
.global lbl_8021BF40
lbl_8021BF40:
	.skip 0x8
.global lbl_8021BF48
lbl_8021BF48:
	.skip 0x4
.global lbl_8021BF4C
lbl_8021BF4C:
	.skip 0x4
.global lbl_8021BF50
lbl_8021BF50:
	.skip 0x4
.global lbl_8021BF54
lbl_8021BF54:
	.skip 0x4
.global lbl_8021BF58
lbl_8021BF58:
	.skip 0x8
.global lbl_8021BF60
lbl_8021BF60:
	.skip 0x4
.global lbl_8021BF64
lbl_8021BF64:
	.skip 0x4
.global lbl_8021BF68
lbl_8021BF68:
	.skip 0x8
.global lbl_8021BF70
lbl_8021BF70:
	.skip 0x4
.global lbl_8021BF74
lbl_8021BF74:
	.skip 0x4
.global lbl_8021BF78
lbl_8021BF78:
	.skip 0x4
.global lbl_8021BF7C
lbl_8021BF7C:
	.skip 0x4
.global lbl_8021BF80
lbl_8021BF80:
	.skip 0x4
.global lbl_8021BF84
lbl_8021BF84:
	.skip 0x4
.global lbl_8021BF88
lbl_8021BF88:
	.skip 0x8
.global lbl_8021BF90
lbl_8021BF90:
	.skip 0x8
.global lbl_8021BF98
lbl_8021BF98:
	.skip 0x4
.global lbl_8021BF9C
lbl_8021BF9C:
	.skip 0x4
.global lbl_8021BFA0
lbl_8021BFA0:
	.skip 0x4
.global lbl_8021BFA4
lbl_8021BFA4:
	.skip 0x4
.global lbl_8021BFA8
lbl_8021BFA8:
	.skip 0x4
.global lbl_8021BFAC
lbl_8021BFAC:
	.skip 0x4
.global lbl_8021BFB0
lbl_8021BFB0:
	.skip 0x4
.global lbl_8021BFB4
lbl_8021BFB4:
	.skip 0x4
.global lbl_8021BFB8
lbl_8021BFB8:
	.skip 0x8
.global lbl_8021BFC0
lbl_8021BFC0:
	.skip 0x4
.global lbl_8021BFC4
lbl_8021BFC4:
	.skip 0x4
.global lbl_8021BFC8
lbl_8021BFC8:
	.skip 0x8
.global lbl_8021BFD0
lbl_8021BFD0:
	.skip 0x4
.global lbl_8021BFD4
lbl_8021BFD4:
	.skip 0x4
.global lbl_8021BFD8
lbl_8021BFD8:
	.skip 0x4
.global lbl_8021BFDC
lbl_8021BFDC:
	.skip 0x4
.global lbl_8021BFE0
lbl_8021BFE0:
	.skip 0x4
.global lbl_8021BFE4
lbl_8021BFE4:
	.skip 0x4
.global lbl_8021BFE8
lbl_8021BFE8:
	.skip 0x8
.global lbl_8021BFF0
lbl_8021BFF0:
	.skip 0x8
.global lbl_8021BFF8
lbl_8021BFF8:
	.skip 0x4
.global lbl_8021BFFC
lbl_8021BFFC:
	.skip 0x4
.global lbl_8021C000
lbl_8021C000:
	.skip 0x8
.global lbl_8021C008
lbl_8021C008:
	.skip 0x4
.global lbl_8021C00C
lbl_8021C00C:
	.skip 0x4
.global lbl_8021C010
lbl_8021C010:
	.skip 0x4
.global lbl_8021C014
lbl_8021C014:
	.skip 0x4
.global lbl_8021C018
lbl_8021C018:
	.skip 0x4
.global lbl_8021C01C
lbl_8021C01C:
	.skip 0x4
.global lbl_8021C020
lbl_8021C020:
	.skip 0x2
.global lbl_8021C022
lbl_8021C022:
	.skip 0x6
.global lbl_8021C028
lbl_8021C028:
	.skip 0x4
.global lbl_8021C02C
lbl_8021C02C:
	.skip 0x4
.global lbl_8021C030
lbl_8021C030:
	.skip 0x4
.global lbl_8021C034
lbl_8021C034:
	.skip 0x4
.global lbl_8021C038
lbl_8021C038:
	.skip 0x8
.global lbl_8021C040
lbl_8021C040:
	.skip 0x4
.global lbl_8021C044
lbl_8021C044:
	.skip 0x4
.global lbl_8021C048
lbl_8021C048:
	.skip 0x4
.global lbl_8021C04C
lbl_8021C04C:
	.skip 0x4
.global lbl_8021C050
lbl_8021C050:
	.skip 0x4
.global lbl_8021C054
lbl_8021C054:
	.skip 0x4
.global lbl_8021C058
lbl_8021C058:
	.skip 0x4
.global lbl_8021C05C
lbl_8021C05C:
	.skip 0x4
.global lbl_8021C060
lbl_8021C060:
	.skip 0x4
.global lbl_8021C064
lbl_8021C064:
	.skip 0x4
.global lbl_8021C068
lbl_8021C068:
	.skip 0x8
.global lbl_8021C070
lbl_8021C070:
	.skip 0x4
.global lbl_8021C074
lbl_8021C074:
	.skip 0x4
.global lbl_8021C078
lbl_8021C078:
	.skip 0x4
.global lbl_8021C07C
lbl_8021C07C:
	.skip 0xC
.global lbl_8021C088
lbl_8021C088:
	.skip 0x8
.global lbl_8021C090
lbl_8021C090:
	.skip 0x4
.global lbl_8021C094
lbl_8021C094:
	.skip 0x4
.global lbl_8021C098
lbl_8021C098:
	.skip 0x4
.global lbl_8021C09C
lbl_8021C09C:
	.skip 0x4
.global lbl_8021C0A0
lbl_8021C0A0:
	.skip 0x8
.global lbl_8021C0A8
lbl_8021C0A8:
	.skip 0x8
.global lbl_8021C0B0
lbl_8021C0B0:
	.skip 0x8
.global lbl_8021C0B8
lbl_8021C0B8:
	.skip 0x8
.global lbl_8021C0C0
lbl_8021C0C0:
	.skip 0x8
.global lbl_8021C0C8
lbl_8021C0C8:
	.skip 0x8
.global lbl_8021C0D0
lbl_8021C0D0:
	.skip 0x4
.global lbl_8021C0D4
lbl_8021C0D4:
	.skip 0x4
.global lbl_8021C0D8
lbl_8021C0D8:
	.skip 0x4
.global lbl_8021C0DC
lbl_8021C0DC:
	.skip 0x4
.global lbl_8021C0E0
lbl_8021C0E0:
	.skip 0x4
.global lbl_8021C0E4
lbl_8021C0E4:
	.skip 0x4
.global lbl_8021C0E8
lbl_8021C0E8:
	.skip 0x8
.global lbl_8021C0F0
lbl_8021C0F0:
	.skip 0x8
.global lbl_8021C0F8
lbl_8021C0F8:
	.skip 0x8
.global lbl_8021C100
lbl_8021C100:
	.skip 0x8
.global lbl_8021C108
lbl_8021C108:
	.skip 0x8
.global lbl_8021C110
lbl_8021C110:
	.skip 0x4
.global lbl_8021C114
lbl_8021C114:
	.skip 0x4
.global lbl_8021C118
lbl_8021C118:
	.skip 0x4
.global lbl_8021C11C
lbl_8021C11C:
	.skip 0x4
.global lbl_8021C120
lbl_8021C120:
	.skip 0x4
.global lbl_8021C124
lbl_8021C124:
	.skip 0x4
.global lbl_8021C128
lbl_8021C128:
	.skip 0x4
.global lbl_8021C12C
lbl_8021C12C:
	.skip 0x4
.global lbl_8021C130
lbl_8021C130:
	.skip 0x4
.global lbl_8021C134
lbl_8021C134:
	.skip 0x4
.global lbl_8021C138
lbl_8021C138:
	.skip 0x4
.global lbl_8021C13C
lbl_8021C13C:
	.skip 0x4
.global lbl_8021C140
lbl_8021C140:
	.skip 0x4
.global lbl_8021C144
lbl_8021C144:
	.skip 0x4
.global lbl_8021C148
lbl_8021C148:
	.skip 0x4
.global lbl_8021C14C
lbl_8021C14C:
	.skip 0x4
.global lbl_8021C150
lbl_8021C150:
	.skip 0x4
.global lbl_8021C154
lbl_8021C154:
	.skip 0x4
.global lbl_8021C158
lbl_8021C158:
	.skip 0x8
.global lbl_8021C160
lbl_8021C160:
	.skip 0x4
.global lbl_8021C164
lbl_8021C164:
	.skip 0x4
.global lbl_8021C168
lbl_8021C168:
	.skip 0x4
.global lbl_8021C16C
lbl_8021C16C:
	.skip 0x4
.global lbl_8021C170
lbl_8021C170:
	.skip 0x4
.global lbl_8021C174
lbl_8021C174:
	.skip 0x4
.global lbl_8021C178
lbl_8021C178:
	.skip 0x4
.global lbl_8021C17C
lbl_8021C17C:
	.skip 0x4
.global lbl_8021C180
lbl_8021C180:
	.skip 0x4
.global lbl_8021C184
lbl_8021C184:
	.skip 0x4
.global lbl_8021C188
lbl_8021C188:
	.skip 0x8
.global lbl_8021C190
lbl_8021C190:
	.skip 0x4
.global lbl_8021C194
lbl_8021C194:
	.skip 0x4
.global lbl_8021C198
lbl_8021C198:
	.skip 0x4
.global lbl_8021C19C
lbl_8021C19C:
	.skip 0x4
.global lbl_8021C1A0
lbl_8021C1A0:
	.skip 0x4
.global lbl_8021C1A4
lbl_8021C1A4:
	.skip 0x4
.global lbl_8021C1A8
lbl_8021C1A8:
	.skip 0x4
.global lbl_8021C1AC
lbl_8021C1AC:
	.skip 0x4
.global lbl_8021C1B0
lbl_8021C1B0:
	.skip 0x4
.global lbl_8021C1B4
lbl_8021C1B4:
	.skip 0x4
.global lbl_8021C1B8
lbl_8021C1B8:
	.skip 0x4
.global lbl_8021C1BC
lbl_8021C1BC:
	.skip 0x4
.global lbl_8021C1C0
lbl_8021C1C0:
	.skip 0x4
.global lbl_8021C1C4
lbl_8021C1C4:
	.skip 0x4
.global lbl_8021C1C8
lbl_8021C1C8:
	.skip 0x8
.global lbl_8021C1D0
lbl_8021C1D0:
	.skip 0x8
.global lbl_8021C1D8
lbl_8021C1D8:
	.skip 0x4
.global lbl_8021C1DC
lbl_8021C1DC:
	.skip 0x4
.global lbl_8021C1E0
lbl_8021C1E0:
	.skip 0x8
.global lbl_8021C1E8
lbl_8021C1E8:
	.skip 0x4
.global lbl_8021C1EC
lbl_8021C1EC:
	.skip 0x4
.global lbl_8021C1F0
lbl_8021C1F0:
	.skip 0x4
.global lbl_8021C1F4
lbl_8021C1F4:
	.skip 0x4
.global lbl_8021C1F8
lbl_8021C1F8:
	.skip 0x4
.global lbl_8021C1FC
lbl_8021C1FC:
	.skip 0x4
.global lbl_8021C200
lbl_8021C200:
	.skip 0x4
.global lbl_8021C204
lbl_8021C204:
	.skip 0x4
.global lbl_8021C208
lbl_8021C208:
	.skip 0x4
.global lbl_8021C20C
lbl_8021C20C:
	.skip 0x4
.global lbl_8021C210
lbl_8021C210:
	.skip 0x4
.global lbl_8021C214
lbl_8021C214:
	.skip 0x4
.global lbl_8021C218
lbl_8021C218:
	.skip 0x4
.global lbl_8021C21C
lbl_8021C21C:
	.skip 0x4
.global lbl_8021C220
lbl_8021C220:
	.skip 0x4
.global lbl_8021C224
lbl_8021C224:
	.skip 0x4
.global lbl_8021C228
lbl_8021C228:
	.skip 0x4
.global lbl_8021C22C
lbl_8021C22C:
	.skip 0x4
.global lbl_8021C230
lbl_8021C230:
	.skip 0x4
.global lbl_8021C234
lbl_8021C234:
	.skip 0x4
.global lbl_8021C238
lbl_8021C238:
	.skip 0x4
.global lbl_8021C23C
lbl_8021C23C:
	.skip 0x4
.global lbl_8021C240
lbl_8021C240:
	.skip 0x4
.global lbl_8021C244
lbl_8021C244:
	.skip 0x4
.global lbl_8021C248
lbl_8021C248:
	.skip 0x4
.global lbl_8021C24C
lbl_8021C24C:
	.skip 0x4
.global lbl_8021C250
lbl_8021C250:
	.skip 0x4
.global lbl_8021C254
lbl_8021C254:
	.skip 0x4
.global lbl_8021C258
lbl_8021C258:
	.skip 0x4
.global lbl_8021C25C
lbl_8021C25C:
	.skip 0x4
.global lbl_8021C260
lbl_8021C260:
	.skip 0x4
.global lbl_8021C264
lbl_8021C264:
	.skip 0x4
.global lbl_8021C268
lbl_8021C268:
	.skip 0x4
.global lbl_8021C26C
lbl_8021C26C:
	.skip 0x4
.global lbl_8021C270
lbl_8021C270:
	.skip 0x4
.global lbl_8021C274
lbl_8021C274:
	.skip 0x4
.global lbl_8021C278
lbl_8021C278:
	.skip 0x4
.global lbl_8021C27C
lbl_8021C27C:
	.skip 0x4
.global lbl_8021C280
lbl_8021C280:
	.skip 0x4
.global lbl_8021C284
lbl_8021C284:
	.skip 0x4
.global lbl_8021C288
lbl_8021C288:
	.skip 0x8
.global lbl_8021C290
lbl_8021C290:
	.skip 0x8
.global lbl_8021C298
lbl_8021C298:
	.skip 0x4
.global lbl_8021C29C
lbl_8021C29C:
	.skip 0x4
.global lbl_8021C2A0
lbl_8021C2A0:
	.skip 0x4
.global lbl_8021C2A4
lbl_8021C2A4:
	.skip 0x4
.global lbl_8021C2A8
lbl_8021C2A8:
	.skip 0x4
.global lbl_8021C2AC
lbl_8021C2AC:
	.skip 0x4
.global lbl_8021C2B0
lbl_8021C2B0:
	.skip 0x4
.global lbl_8021C2B4
lbl_8021C2B4:
	.skip 0x4
.global lbl_8021C2B8
lbl_8021C2B8:
	.skip 0x4
.global lbl_8021C2BC
lbl_8021C2BC:
	.skip 0x4
.global lbl_8021C2C0
lbl_8021C2C0:
	.skip 0x8
.global lbl_8021C2C8
lbl_8021C2C8:
	.skip 0x4
.global lbl_8021C2CC
lbl_8021C2CC:
	.skip 0x4
.global lbl_8021C2D0
lbl_8021C2D0:
	.skip 0x4
.global lbl_8021C2D4
lbl_8021C2D4:
	.skip 0x4
.global lbl_8021C2D8
lbl_8021C2D8:
	.skip 0x8
.global lbl_8021C2E0
lbl_8021C2E0:
	.skip 0x8
.global lbl_8021C2E8
lbl_8021C2E8:
	.skip 0x4
.global lbl_8021C2EC
lbl_8021C2EC:
	.skip 0x4
.global MaxObjectAmount
MaxObjectAmount:
	.skip 0x4
.global lbl_8021C2F4
lbl_8021C2F4:
	.skip 0x2
.global gu16MaxTaskExBNum
gu16MaxTaskExBNum:
	.skip 0x2
.global gu32TaskNum
gu32TaskNum:
	.skip 0x4
.global lbl_8021C2FC
lbl_8021C2FC:
	.skip 0x4
.global gu32TaskExBNum
gu32TaskExBNum:
	.skip 0x4
.global lbl_8021C304
lbl_8021C304:
	.skip 0x4
.global lbl_8021C308
lbl_8021C308:
	.skip 0x4
.global StartOfObjectsPointer
StartOfObjectsPointer:
	.skip 0x4
.global CurrentObjectAmount
CurrentObjectAmount:
	.skip 0x4
.global gpsCurrentTask
gpsCurrentTask:
	.skip 0x4
.global gsHeapInfo
gsHeapInfo:
	.skip 0x18
.global gu32HeapInitialized
gu32HeapInitialized:
	.skip 0x4

.global __sbss_rom_end__
__sbss_rom_end__: