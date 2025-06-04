#include "megaman64.h"

//game thread 1: func_80089CFC --
//game thread 17: func_80092298 --
//game thread 18: func_8009213C
//game thread 19: func_80089CFC --
//game thread 200: func_8003E168
//game thread 3: func_80026930
//game thread 3(2): func_8008E8A8 //audio related
//game thread 4: func_800925B8
//game thread 5: func_80092690
//game thread 5(2): func_80092690
//game thread 6: func_80093A30
//game thread 7: func_800270F8

extern int __osSpDeviceBusy(void);
extern int __osDpDeviceBusy(void);
int __osSiDeviceBusy(void);

volatile s32 isSaveOrLoadActive = 0;

int __osPiDeviceBusy() {
    register u32 stat = IO_READ(PI_STATUS_REG);
    if (stat & (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY))
        return 1;
    return 0;
}

void wait_on_hardware(void) {
    //wait on rsp
    while (__osSpDeviceBusy() == 1) {}

    //wait on rdp
    while ( __osDpDeviceBusy() == 1) {}

    //wait on SI
    while (__osSiDeviceBusy() == 1) {}

    //wait on PI
    while (__osPiDeviceBusy() == 1) {}

    //invalidate caches
    osInvalICache((void*)0x80000000, 0x2000);
	osInvalDCache((void*)0x80000000, 0x2000);
}

#define ramStartAddr (u32)0x800A4430
#define ramEndAddr (u32)0x80400000
#define ramAddrSavestateDataSlot1 (void*)0x80480000

//these might be important?
// dlabel D_800AC500
//     /* 87900 800AC500 8003E2A0 */ .word func_8003E2A0
//     /* 87904 800AC504 8003BE70 */ .word func_8003BE70
//     /* 87908 800AC508 8003C01C */ .word func_8003C01C
//     /* 8790C 800AC50C 8003C1B8 */ .word func_8003C1B8
//     /* 87910 800AC510 8003CF4C */ .word func_8003CF4C
//     /* 87914 800AC514 8003D0D8 */ .word func_8003D0D8
//     /* 87918 800AC518 8003E394 */ .word func_8003E394
//     /* 8791C 800AC51C 8003E4F0 */ .word func_8003E4F0
//     /* 87920 800AC520 8003D5BC */ .word func_8003D5BC
//     /* 87924 800AC524 8003D85C */ .word func_8003D85C
//     /* 87928 800AC528 8003D85C */ .word func_8003D85C
//     /* 8792C 800AC52C 00000000 */ .word 0x00000000
// .size D_800AC500, . - D_800AC500

// dlabel D_800AC530
//     /* 87930 800AC530 80043DC4 */ .word func_80043DC4
//     /* 87934 800AC534 80043DC4 */ .word func_80043DC4
//     /* 87938 800AC538 80043F40 */ .word func_80043F40
//     /* 8793C 800AC53C 800441E4 */ .word func_800441E4
//     /* 87940 800AC540 80044488 */ .word func_80044488
//     /* 87944 800AC544 8004472C */ .word func_8004472C
//     /* 87948 800AC548 800449D0 */ .word func_800449D0
//     /* 8794C 800AC54C 80044B8C */ .word func_80044B8C
// .size D_800AC530, . - D_800AC530

//start + c-up to load
//start + c-right to save

void loadstateMain(void) {
    u32 saveMask;
    isSaveOrLoadActive = 1;

    if (*(u32*)ramAddrSavestateDataSlot1 != 0x4A00002C) { //if savedata isn't here already, dont attempt to load it
        isSaveOrLoadActive = 0; //allow thread to continue
        return;
    }

    wait_on_hardware();

    saveMask = __osDisableInt();
    optimized_memcpy((void*)ramStartAddr,  ramAddrSavestateDataSlot1, ramEndAddr - ramStartAddr);
    __osRestoreInt(saveMask);
    isSaveOrLoadActive = 0; //allow thread to continue
}

void savestateMain(void) {
    u32 saveMask;
    isSaveOrLoadActive = 1;
    
    wait_on_hardware();
    
    saveMask = __osDisableInt();
    optimized_memcpy(ramAddrSavestateDataSlot1, (void*)ramStartAddr, ramEndAddr - ramStartAddr);
    __osRestoreInt(saveMask);
    isSaveOrLoadActive = 0; //allow thread to continue
}