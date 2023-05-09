/*
Code sections:
	.text0:	0x00000100	0x80003100	0x800055E0
	.text1:	0x000025E0	0x800055E0	0x800E2F20
Data sections:
	.data0:	0x000DFF20	0x800E2F20	0x800E2F40
	.data1:	0x000DFF40	0x800E2F40	0x800E2F60
	.data2:	0x000DFF60	0x800E2F60	0x800ED2E0
	.data3:	0x000EA2E0	0x800ED300	0x80125E00
	.data4:	0x00122DE0	0x8021B8C0	0x8021BB40
	.data5:	0x00123060	0x8021C340	0x8021CFA0
BSS section:
	.bss:	0x00000000	0x80125E00	0x8021CFA4
Entry Point: 0x80003154
*/
# PowerPC Register Constants
.set r0, 0
.set r1, 1
.set r2, 2
.set r3, 3
.set r4, 4
.set r5, 5
.set r6, 6
.set r7, 7
.set r8, 8
.set r9, 9
.set r10, 10
.set r11, 11
.set r12, 12
.set r13, 13
.set r14, 14
.set r15, 15
.set r16, 16
.set r17, 17
.set r18, 18
.set r19, 19
.set r20, 20
.set r21, 21
.set r22, 22
.set r23, 23
.set r24, 24
.set r25, 25
.set r26, 26
.set r27, 27
.set r28, 28
.set r29, 29
.set r30, 30
.set r31, 31
.set f0, 0
.set f1, 1
.set f2, 2
.set f3, 3
.set f4, 4
.set f5, 5
.set f6, 6
.set f7, 7
.set f8, 8
.set f9, 9
.set f10, 10
.set f11, 11
.set f12, 12
.set f13, 13
.set f14, 14
.set f15, 15
.set f16, 16
.set f17, 17
.set f18, 18
.set f19, 19
.set f20, 20
.set f21, 21
.set f22, 22
.set f23, 23
.set f24, 24
.set f25, 25
.set f26, 26
.set f27, 27
.set f28, 28
.set f29, 29
.set f30, 30
.set f31, 31
.set qr0, 0
.set qr1, 1
.set qr2, 2
.set qr3, 3
.set qr4, 4
.set qr5, 5
.set qr6, 6
.set qr7, 7

# AND with a bit and convert to bool macro
.macro andbool dgpr, gpr, bit
.if (\bit == 0x1)
rlwinm \dgpr, \gpr, 0, 31, 31
.endif

.if (\bit == 0x2)
rlwinm \dgpr, \gpr, 31, 31, 31
.endif

.if (\bit == 0x4)
rlwinm \dgpr, \gpr, 30, 31, 31
.endif

.if (\bit == 0x8)
rlwinm \dgpr, \gpr, 29, 31, 31
.endif

.if (\bit == 0x10)
rlwinm \dgpr, \gpr, 28, 31, 31
.endif

.if (\bit == 0x20)
rlwinm \dgpr, \gpr, 27, 31, 31
.endif

.if (\bit == 0x40)
rlwinm \dgpr, \gpr, 26, 31, 31
.endif

.if (\bit == 0x80)
rlwinm \dgpr, \gpr, 25, 31, 31
.endif

.if (\bit == 0x100)
rlwinm \dgpr, \gpr, 24, 31, 31
.endif

.if (\bit == 0x200)
rlwinm \dgpr, \gpr, 23, 31, 31
.endif

.if (\bit == 0x400)
rlwinm \dgpr, \gpr, 22, 31, 31
.endif

.if (\bit == 0x800)
rlwinm \dgpr, \gpr, 21, 31, 31
.endif

.if (\bit == 0x1000)
rlwinm \dgpr, \gpr, 20, 31, 31
.endif

.if (\bit == 0x2000)
rlwinm \dgpr, \gpr, 19, 31, 31
.endif

.if (\bit == 0x4000)
rlwinm \dgpr, \gpr, 18, 31, 31
.endif

.if (\bit == 0x8000)
rlwinm \dgpr, \gpr, 17, 31, 31
.endif

.if (\bit == 0x10000)
rlwinm \dgpr, \gpr, 16, 31, 31
.endif

.if (\bit == 0x20000)
rlwinm \dgpr, \gpr, 15, 31, 31
.endif

.if (\bit == 0x40000)
rlwinm \dgpr, \gpr, 14, 31, 31
.endif

.if (\bit == 0x80000)
rlwinm \dgpr, \gpr, 13, 31, 31
.endif

.if (\bit == 0x100000)
rlwinm \dgpr, \gpr, 12, 31, 31
.endif

.if (\bit == 0x200000)
rlwinm \dgpr, \gpr, 11, 31, 31
.endif

.if (\bit == 0x400000)
rlwinm \dgpr, \gpr, 10, 31, 31
.endif

.if (\bit == 0x800000)
rlwinm \dgpr, \gpr, 9, 31, 31
.endif

.if (\bit == 0x1000000)
rlwinm \dgpr, \gpr, 8, 31, 31
.endif

.if (\bit == 0x2000000)
rlwinm \dgpr, \gpr, 7, 31, 31
.endif

.if (\bit == 0x4000000)
rlwinm \dgpr, \gpr, 6, 31, 31
.endif

.if (\bit == 0x8000000)
rlwinm \dgpr, \gpr, 5, 31, 31
.endif

.if (\bit == 0x10000000)
rlwinm \dgpr, \gpr, 4, 31, 31
.endif

.if (\bit == 0x20000000)
rlwinm \dgpr, \gpr, 3, 31, 31
.endif

.if (\bit == 0x40000000)
rlwinm \dgpr, \gpr, 2, 31, 31
.endif

.if (\bit == 0x80000000)
rlwinm \dgpr, \gpr, 1, 31, 31
.endif
.endm

