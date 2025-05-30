#ifndef _MM64_H_
#define _MM64_H_

#include "ultra64.h"
#include "viint.h"

typedef struct Unk {
    /* 0x00 */ struct Unk* next;                           /* inferred */
    /* 0x04 */ char pad4[4];
    /* 0x08 */ s32 unk8;                            /* inferred */
    /* 0x0C */ s32 unkC;                            /* inferred */
    /* 0x10 */ char pad10[4];
    /* 0x14 */ u32 unk14;                           /* inferred */
    /* 0x18 */ char pad18[8];                       /* maybe part of unk14[3]? */
    /* 0x20 */ s32 unk20;                           /* inferred */
    /* 0x24 */ char pad24[4];
    /* 0x28 */ s32 unk28;                           /* inferred */
    /* 0x2C */ char pad2C[0xC];                     /* maybe part of unk28[4]? */
    /* 0x38 */ s32 unk38;                           /* inferred */
    /* 0x3C */ s32 unk3C;                           /* inferred */
    /* 0x40 */ Gfx* unk40;                          /* inferred */
    /* 0x44 */ s32 unk44;                           /* inferred */
    /* 0x48 */ char pad48[0xC];                     /* maybe part of unk44[4]? */
    /* 0x54 */ u16* unk54;                          /* inferred */
} Unk;                                              /* size = 0x58 */

typedef struct Unk2 {
    s32 unk_00;
    s32 unk_04;
} Unk2;

extern Gfx* gGfxMainPos;

#endif //_MM64_H_