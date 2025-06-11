/*
 * This header file can be used for when you want to call a function written in C++ from the game's assembly code.
 * This is most useful in cases where you need to call a C++ function, but you need to do it in an unsafe area in assembly.
 *
 * When using this header file, you have to #define all the required and optional defines BEFORE you include it in your file.
 * If applied correctly, this header file will generate a #define called ASMHELPER_FUNCTION.
 * You can use that macro in your source file anywhere you see fit.
 * If you get a declaration syntax error (ASMHELPER_FUNCTION define wasn't generated), double-check your required defines.
 *
 *
 *
 * These are the required defines:
 *
 * ASMHELPER_MAINFUNCNAME
 * - The name of the function you can call from ASM that will call your C++ function.
 * - Examples:
 * #define ASMHELPER_MAINFUNCNAME CallMyFuncFromASM
 *
 * ASMHELPER_FUNCARGUMENTSETUP
 * - List of assembly instructions that set up the arguments to be passed into your function, if necessary.
 * - Examples:
 * - #define ASMHELPER_FUNCARGUMENTSETUP
 * - #define ASMHELPER_FUNCARGUMENTSETUP \
 * "mr r3, r28\n"\
 * "mr r4, r29\n"\
 * "mr r5, r31\n"
 *
 * ASMHELPER_FUNCTOCALL
 * - The name of the function the ASM helper function will call after performing stack frame setup and function argument setup.
 * - Examples:
 * - #define ASMHELPER_FUNCTOCALL MyFunc
 *
 *
 *
 * Here are the optional defines:
 *
 * ASMHELPER_RETURNVALUEFROMFUNC
 * - If your C++ function returns a value in r3, define this keyword to have the ASM helper function account for that.
 * - You don't have to give this define a value.
 * - #define ASMHELPER_RETURNVALUEFROMFUNC
 *
 * ASMHELPER_PRESTACKSETUP
 * - List of assembly instructions that are performed very first (even before stack frame setup) in the ASM helper function.
 * - This is mostly used for if you need to preserve an original instruction in the ASM helper function.
 * - Examples:
 * - #define ASMHELPER_PRESTACKSETUP \
 * "stw r3, 0x108 (r9)\n"
 *
 */

// ReSharper disable once CppMissingIncludeGuard
#include "macros.h"

#define ASMHELPER_ALLOCSTACKFRAME \
"stwu r1, -0x90 (r1)\n"\
"stw r0, 0x8 (r1)\n"\
"mflr r0\n"\
"stw r0, 0x94 (r1)\n"\
"lwz r0, 0x8 (r1)\n"\
"stmw r3, 0xC (r1)\n"

#define ASMHELPER_FREESTACKFRAME \
"lwz r0, 0x94 (r1)\n"\
"mtlr r0\n"\
"lwz r0, 0x8 (r1)\n"\
"lmw r3, 0xC (r1)\n"\
"addi r1, r1, 0x90\n"\
"blr\n"

#define ASMHELPER_RETURNVALUEFROMFUNCASM \
"stw r3, 0xC (r1)\n"

#if defined (ASMHELPER_MAINFUNCNAME) && defined (ASMHELPER_FUNCTOCALL) && defined (ASMHELPER_FUNCARGUMENTSETUP)

    #if defined (ASMHELPER_PRESTACKSETUP) && defined (ASMHELPER_RETURNVALUEFROMFUNC)

        #define ASMHELPER_FUNCTION \
        ASMUsed void ASMHELPER_MAINFUNCNAME() { \
            asm(                       \
            ASMHELPER_PRESTACKSETUP\
            ASMHELPER_ALLOCSTACKFRAME\
            ASMHELPER_FUNCARGUMENTSETUP\
            "bl %[ASMFunction]\n"\
            ASMHELPER_RETURNVALUEFROMFUNCASM\
            ASMHELPER_FREESTACKFRAME           \
            :                      \
            : [ASMFunction] "i"(ASMHELPER_FUNCTOCALL));                           \
        }

    #elif defined (ASMHELPER_PRESTACKSETUP)

        #define ASMHELPER_FUNCTION \
        ASMUsed void ASMHELPER_MAINFUNCNAME() { \
            asm(                       \
            ASMHELPER_PRESTACKSETUP\
            ASMHELPER_ALLOCSTACKFRAME\
            ASMHELPER_FUNCARGUMENTSETUP\
            "bl %[ASMFunction]\n"\
            ASMHELPER_FREESTACKFRAME\
            :                      \
            : [ASMFunction] "i"(ASMHELPER_FUNCTOCALL));                           \
        }

    #elif defined (ASMHELPER_RETURNVALUEFROMFUNC)

        #define ASMHELPER_FUNCTION \
        ASMUsed void ASMHELPER_MAINFUNCNAME() { \
            asm(                       \
            ASMHELPER_ALLOCSTACKFRAME\
            ASMHELPER_FUNCARGUMENTSETUP\
            "bl %[ASMFunction]\n"\
            ASMHELPER_RETURNVALUEFROMFUNCASM\
            ASMHELPER_FREESTACKFRAME\
            :                      \
            : [ASMFunction] "i"(ASMHELPER_FUNCTOCALL));                           \
        }

    #else

        #define ASMHELPER_FUNCTION \
        ASMUsed void ASMHELPER_MAINFUNCNAME() { \
            asm(                       \
            ASMHELPER_ALLOCSTACKFRAME\
            ASMHELPER_FUNCARGUMENTSETUP\
            "bl %[ASMFunction]\n"\
            ASMHELPER_FREESTACKFRAME\
            :                      \
            : [ASMFunction] "i"(ASMHELPER_FUNCTOCALL));                           \
        }

    #endif

#endif