#include "megaman64.h"

//game thread 1: func_80089CFC
//game thread 17: func_80092298
//game thread 18: func_8009213C
//game thread 19: func_80089CFC
//game thread 200: func_8003E168
//game thread 3: func_80026930
//game thread 3(2): func_8008E8A8
//game thread 4: func_800925B8
//game thread 5: func_80092690
//game thread 5(2): func_80092690
//game thread 6: func_80093A30
//game thread 7: func_800270F8

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
#define ramEndAddr (u32)0x80324120
#define ramAddrSavestateDataSlot1 (void*)0x804C0000

//start + c-up to load
//start + c-right to save

void loadstateMain(void) {
    u32 saveMask;

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
    
    wait_on_hardware();
    
    saveMask = __osDisableInt();
    optimized_memcpy(ramAddrSavestateDataSlot1, (void*)ramStartAddr, ramEndAddr - ramStartAddr);
    __osRestoreInt(saveMask);
    isSaveOrLoadActive = 0; //allow thread to continue
}