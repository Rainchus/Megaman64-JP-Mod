#include "megaman64.h"

typedef struct Unk8020CA80 {
    char unk_00[0xBD];
    u8 unk_BD;
} Unk8020CA80;

typedef struct UnkArg0 {
    u8 unk_00;
    s8 unk_01;
} UnkArg0;

void func_800290C4(s32);
void func_8002A41C(s32);
void func_8002C714(s32, s32, s32, s32);
void func_80035648(s32);
void func_80035794(s8);
void func_800362F0(s32, s32);
void func_80036838(void);
void func_80036AB0(s32, s32, s32);
void func_80036DF8(s32);
void func_8004B570(void);
void func_8004B5A8(s32);
void func_800571A4(Unk8020CA80*);
void func_800573C0(void);
s32 func_80057424(void);
void func_80061578(void);
void func_80073AF0(void);
void func_80089B70(void);
extern u8 D_801B5D88;
extern s8 D_801C4508;
extern s16 D_801C4824;
extern Unk8020CA80 D_8020CA80;

//draws a few different things on the screen when pausing
void func_8003CF4C_Hook(UnkArg0* arg0) {
    Unk8020CA80* temp = &D_8020CA80;

    switch (arg0->unk_01) {
    case 0:
        if (D_801B5D88 != 0) {
            D_801C4824 = 1;
            func_8004B570();
            func_8002A41C(0x10);
            func_8002A41C(0x16);
            func_8002A41C(0x17);
            func_80036DF8(0);
            arg0->unk_01++;
            func_80089B70();
            return;
        }
    default:
        return;
    case 1:
        func_8002C714(0x300, 0, 0x40, 0x100);
        func_800362F0(0x24, -2);
        D_801C4824 = 0;
        func_800573C0();
        func_80036AB0(0, 1, 0);
        arg0->unk_01++;
        func_80061578();
        func_8004B5A8(-1);
        return;
    case 2:
        arg0->unk_01 += func_80057424();
        return;
    case 3:
        D_801C4824 = 1;
        arg0->unk_01++;
        break;
    case 4:
        func_80035648(2);
        func_80073AF0();
        func_80035794(D_801C4508);
        func_80036838();
        D_801C4824 = 0;
        func_800571A4(temp);
        temp->unk_BD &= ~0x40;
        func_800290C4(5);
        arg0->unk_00 = 3;
        arg0->unk_01 = 1;
        break;
    }
}