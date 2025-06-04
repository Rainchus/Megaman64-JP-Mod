#include "megaman64.h"
#include "viint.h"

s32 menuActive = 0;
s32 cursorPos = 0;
Gfx customGfx[8092] ALIGNED(16) = {0};

//80032E38 gets player input for use in game

extern s32 gCamUnk; //some unknown camera parameter, related to position somehow?
//80033120: JAL memcpy //updates camera to player position
//8004DC84: JAL 0x80030A50 //somehow related to collision checking?
//8004DA84: JAL 0x8004DBEC //related to moving player

extern volatile s32 isSaveOrLoadActive;
extern u16 p1Inputs; //801C4450
extern u16 p1InputsPressed; //801C4456
extern u32 gTime;

extern Unk* D_80195300;
extern Unk2* D_801E0EF0;
extern volatile s32 D_80207C80;
extern s32 D_80218B20;
extern s32* D_80218CC0;
extern u32 D_800CF420;
extern u32 D_800CF424;
extern s32 D_800CF428;
extern u16 D_800CF434;
extern u16 D_80195304;
extern u16 D_80195306;
extern u32 D_801B7544;
extern OSMesgQueue D_8021D28C;
extern gfx_font *kfont;
void BuildCustomDL(void);
u32 _strlen(char*);

s32 init = 0;

typedef struct CustomThread {
    /* 0x000 */ OSThread thread;
    /* 0x1B0 */ char stack[0xC000];
    /* 0x9B0 */ OSMesgQueue queue;
    /* 0x9C8 */ OSMesg mesg;
    /* 0x9CC */ u16* frameBuf;
    /* 0x9D0 */ u16 width;
    /* 0x9D2 */ u16 height;
} CustomThread; // size = 0x9D4

CustomThread gCustomThread ALIGNED(16) = {0};

Gfx* gfx_printf_color(Gfx* gfx, u16 left, u16 top, u32 color, const char *format, ...);
void savestateMain(void);
void loadstateMain(void);
void gfx_init(void);
void crash_screen_init(void);

u32 osDpGetStatus() {
    return IO_READ(DPC_STATUS_REG);
}

void osDpSetStatus(u32 data) {
    IO_WRITE(DPC_STATUS_REG, data);
}

void savestateCheckMain(void) {
    if (p1Inputs == 0x0100) {
        //isSaveOrLoadActive = 1;
        osCreateThread(&gCustomThread.thread, 255, (void*)savestateMain, NULL,
                gCustomThread.stack + sizeof(gCustomThread.stack), 255);
        osStartThread(&gCustomThread.thread);

    } else if (p1Inputs == 0x0200) {
        //isSaveOrLoadActive = 1;
        osCreateThread(&gCustomThread.thread, 255, (void*)loadstateMain, NULL,
                gCustomThread.stack + sizeof(gCustomThread.stack), 255);
        osStartThread(&gCustomThread.thread);
    }

    while (isSaveOrLoadActive == 1) {

    }
}

void rdpWaitOnSaveOrLoad(void) {
    while (isSaveOrLoadActive) {

    }
}

void rdpWaitOnSaveOrLoad2(void) {
    while (isSaveOrLoadActive) {

    }
}

void rdpWaitOnSaveOrLoad3(void) {
    while (isSaveOrLoadActive) {

    }
}

//seems to make it worse?
void rdpWaitOnSaveOrLoad4(void) {
    while (isSaveOrLoadActive == 1) {

    }
}

//seems to help?
void rdpWaitOnSaveOrLoad5(void) {
    while (isSaveOrLoadActive == 1) {

    }
}

//seems to make it worse?
void rdpWaitOnSaveOrLoad6(void) {
    while (isSaveOrLoadActive) {
        //osYieldThread();
    }
}

void rdpWaitOnSaveOrLoad7(void) {
    while (isSaveOrLoadActive) {
        //osYieldThread();
    }
}

// u8 ciImage[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x22, 0x21, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x22, 0x22, 0x33, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x22, 0x22, 0x33, 0x21, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x33, 0x33, 0x33, 0x24, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x13, 0x33, 0x33, 0x24, 0x41, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x12, 0x22, 0x22, 0x24, 0x41, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x44, 0x44, 0x22, 0x41, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x14, 0x44, 0x42, 0x21, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x11, 0x14, 0x42, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x11, 0x11, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x11, 0x55, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// u16 palette[] = {0x1886, 0x20C9, 0x8BEB, 0x6AE3, 0xC5F7, 0x2883, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

void func_80092560(void);
void func_800927A4(void);
void func_8009292C(Gfx*, s32, s32, s32);
void func_80092B10(void*);
void func_80092B70(void);
void func_80092C10(void*, s32);
extern void func_80092B50(void);
extern void func_80092D60(void*, s32);

extern Unk2 D_800A6970;
extern s32 D_801E6CB0;
extern Gfx D_800A6980[];
extern u16* gFrameBufferPtrs[2];
extern u8* gDepthBufferPtr;

// void func_80025C60_Hook(void) {
//     Gfx sp10[257];
//     s32 i = 0;

//     func_80092B70();
//     func_80092C10(gFrameBufferPtrs, 2);
//     gDepthBufferPtr = (u8* )0x80000400;
//     func_80092B10(func_80092B50);
//     func_80092D60(&D_801E6CB0, 0x20000);
//     D_801E0EF0 = &D_800A6970;
//     func_800927A4();
    
//     gSPDisplayList(&sp10[i++], OS_K0_TO_PHYSICAL(D_800A6980));
//     gDPFullSync(&sp10[i++]);
//     gSPEndDisplayList(&sp10[i++]);
    
//     func_8009292C(sp10, sizeof(Gfx) * i, 0, 0x40000);
//     func_80092B70();
// }

// void func_80025D14_Hook(void) {
//     Gfx sp10[256];
//     s32 i = 0;

//     func_80092560();
//     func_80092B70();
//     func_80092C10(gFrameBufferPtrs, 2);
//     gDepthBufferPtr = (void*)0x80000400; //this cant be a symbol, is hardcoded to this address
//     func_80092B10(&func_80092B50);
//     func_80092D60(&D_801E6CB0, 0x20000);
//     D_801E0EF0 = &D_800A6970;
//     func_800927A4();

//     gSPDisplayList(&sp10[i++], OS_K0_TO_PHYSICAL(D_800A6980));
//     gDPFullSync(&sp10[i++]);
//     gSPEndDisplayList(&sp10[i++]);
    
//     func_8009292C(sp10, sizeof(Gfx) * i, 0, 0x40000);
//     func_80092B70();
// }

void format_time_30fps(int frame_count, char* out_str) {
    // Convert frames to total milliseconds (1000 ms per second, 30 frames per second)
    int total_ms = (frame_count * 1000) / 30;

    int hours = total_ms / (60 * 60 * 1000);
    total_ms %= (60 * 60 * 1000);

    int minutes = total_ms / (60 * 1000);
    total_ms %= (60 * 1000);

    int seconds = total_ms / 1000;
    int milliseconds = total_ms % 1000;

    // Format: HR:MIN:SEC.MS
    _sprintf(out_str, "%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds);
}

//draw game
void func_8009292C_Hook(Gfx* gfxData, s32 gfxDataSize, s32 arg2, u32 arg3) {
    u32 mask;

    u8* curDLPos = (u8*)gfxData;
    Gfx* endDLAddr;

    //parse DL commands to find full sync
    while (*curDLPos != 0xE9) {
        curDLPos += 8;
    }

    
    BuildCustomDL(); //insert new DL data
    endDLAddr = (Gfx*)curDLPos;
    gSPDisplayList(endDLAddr++, customGfx);
    gDPFullSync(endDLAddr++); //full sync
    gSPEndDisplayList(endDLAddr++); //end DL
    gGfxMainPos = endDLAddr;

    gfxDataSize = (u8*)endDLAddr - (u8*)gfxData; //calculate new size with end - beginning

    D_80195300->unk40 = gfxData; //gfxData points to the beginning of the current display list to parse
    D_80195300->unk44 = gfxDataSize;
    D_80195300->unk14 = (arg3 >> 0x10);
    D_80195300->unk20 = D_801E0EF0[arg2].unk_00;
    D_80195300->unk28 = D_801E0EF0[arg2].unk_04;
    D_80195300->unk38 = D_800CF428;
    D_80195300->unk3C = D_800CF428 + ((D_800CF424 >> 3) * 8);
    D_80195300->unkC = D_80218B20;
    D_80195300->unk8 = (u16)arg3;
    
    if (D_800CF434 & 4) {
        D_800CF434 ^= 4;
        D_80195300->unk14 |= 2;
    }
    
    D_800CF434 = arg3;
    
    if (arg3 & 1) {
        D_80195300->unk54 = &D_80195306;
        D_801B7544 = (D_801B7544 + 1) % D_800CF420;
        D_80218B20 = D_80218CC0[D_801B7544];
        
    } else {
        D_80195300->unk54 = &D_80195304;
    }
    
    mask = osSetIntMask(1);
    D_80207C80++;
    osSetIntMask(mask);
    osWritebackDCacheAll();
    osSendMesg(&D_8021D28C, D_80195300, 1);
    D_80195300 = D_80195300->next;
}

extern s16 gCurHp; //8020CAEE
extern s16 gMaxHP; //8020CB30
Gfx* gfx_begin(Gfx*);
Gfx* gfx_draw_rectangle(Gfx* gfx, int x, int y, int width, int height, u32 color);

extern Player gPlayer;

enum Toggles {
    POS_DISPLAY = 1,
    INF_HP = 2,
    HP_DISPLAY = 3,
    IGT_DISPLAY = 4,
    INPUT_DISPLAY = 5,
};

s8 toggles[] = {
    2,  // NO_TOGGLE
    //page 0
    1,  // POS_DISPLAY
    0,  // INF_HP
    0,  // HP_DISPLAY
    0,  // IGT_DISPLAY
    0,  // Option 5
    0,  // Option 6
};

typedef struct Menu {
    char optionStr[32];
    void (*func)(void);
    s32 flagID;
} Menu;


// void InfHealth(void) {
//     gPlayer.curHp = gPlayer.maxHP;
// }

void dummy(void) {

}

void togglePosDisplay(void) {

}



Menu Page0[] = {
    {"Pos Display\n", dummy, POS_DISPLAY},
    {"Inf Health\n", dummy, INF_HP},
    {"HP Display\n", dummy, HP_DISPLAY},
    {"IGT Display\n", dummy, IGT_DISPLAY},
    {"Option 5\n", dummy, 5},
    {"Option 6\n", dummy, 6},
};

Gfx* BuildMenuDLMain(Gfx* gfxMain, s32 yRoot, Menu* curPage, s32 pageEntries) {
    u32 colorWhite = 0xFFFFFFFF;
    u32 colorRed = 0xFF0000FF;
    u32 colorGreen = 0x00FF00FF;
    u32 colorCyan = 0x00FFFFFF;
    u32 colorPurple = 0x800080FF;

    char menuBuffer[256];

    for (int i = 0; i < pageEntries; i++) {
        _sprintf(menuBuffer, "%s", curPage[i].optionStr);
        if (i == cursorPos) {
            //
            if (toggles[curPage[cursorPos].flagID] == 1) {
                //draw as cyan if current position on the page is on
                gfxMain = gfx_printf_color(gfxMain, 7, yRoot + (cursorPos * kfont->c_height), colorPurple, "%s", curPage[cursorPos].optionStr); //redraw current option
            } else {
                gfxMain = gfx_printf_color(gfxMain, 7, yRoot + (cursorPos * kfont->c_height), colorRed, "%s", curPage[cursorPos].optionStr); //redraw current option
            }
        } else {
            if (toggles[curPage[i].flagID] == 1) {
                gfxMain = gfx_printf_color(gfxMain, 7, yRoot + (i * kfont->c_height), colorGreen, "%s", menuBuffer); //draw main menu
            } else {
                gfxMain = gfx_printf_color(gfxMain, 7, yRoot + (i * kfont->c_height), colorWhite, "%s", menuBuffer); //draw main menu
            }
        }
    }

    // gfxMain = gfx_printf_color(gfxMain, 7, yRoot, colorWhite, "%s", menuBuffer); //draw main menu
    
    return gfxMain;
}

s32 menuCheck(void) {
    if (p1InputsPressed & 8) { //if c-down is pressed, toggle menu
        menuActive ^= 1;
        return 1;
    }

    return menuActive;
}
// //menuCheck
// void ControlMenuToggle(void) {
//     if (p1InputsPressed & CONT_D) { //if c-down is pressed, toggle menu
//         menuActive ^= 1;
//     }
// }

void ControlMenuMain(Menu* curPage, s32 wrapperValue) {
    switch (p1InputsPressed) {
    case CONT_A:
        if (toggles[curPage[cursorPos].flagID] == 0 || toggles[curPage[cursorPos].flagID] == 1) {
            toggles[curPage[cursorPos].flagID] ^= 1; //flip flag
        } else {
            //TODO: implement toggle value 2 that runs function directly, no flag
            curPage[cursorPos].func();
        }
        break;
    case CONT_RIGHT:
        //increment page
        break;
    case CONT_LEFT:
        //decrement page
        break;
    case CONT_DOWN:
        if (cursorPos == wrapperValue - 1) {
            cursorPos = 0;
        } else {
            if ((cursorPos + 1) < wrapperValue) {
                cursorPos++;
            }
        }
        break;
    case CONT_UP:
        if (cursorPos == 0) {
            cursorPos = wrapperValue - 1;
        } else {
            if ((cursorPos - 1) >= 0) {
                cursorPos--;
            }
        }
        break;
    }
}

void func(void) {
    

}

Gfx* DrawActiveToggles(Gfx* gfxMain, s32 yRoot) {
    if (toggles[POS_DISPLAY] == 1) {
        gfxMain = gfx_printf_color(gfxMain, 7,102 + yRoot, 0xFFFFFFFF, "X: %d", gPlayer.xPos);
        gfxMain = gfx_printf_color(gfxMain, 7,112 + yRoot, 0xFFFFFFFF, "Y: %d", gPlayer.yPos);
        gfxMain = gfx_printf_color(gfxMain, 7,122 + yRoot, 0xFFFFFFFF, "Z: %d", gPlayer.zPos);
    }

    if (toggles[INF_HP] == 1) {
        gPlayer.curHp = gPlayer.maxHP;
    }
    
    if (toggles[HP_DISPLAY] == 1) {
        gfxMain = gfx_printf_color(gfxMain, 7,92 + yRoot, 0xFFFFFFFF, "HP: %d", gPlayer.curHp);
    }

    if (toggles[IGT_DISPLAY] == 1) {
        char buffer[64];
        format_time_30fps(gTime, buffer);
        gfxMain = gfx_printf_color(gfxMain, 7, 226 + yRoot, 0xFFFFFFFF, "%s", buffer); //draw current igt
    }

    return gfxMain;
}

Gfx* DrawMenu(Gfx* gfxMain, s32 yRoot) {
    gfxMain = BuildMenuDLMain(gfxMain, yRoot, Page0, ARRAY_COUNT(Page0));

    return gfxMain;
}

static Gfx kzgfx[] = {
    gsDPPipeSync(),

    //gsSPLoadGeometryMode(0), //TODO: figure out why this causes a crash
    gsDPSetScissor(G_SC_NON_INTERLACE,
              0, 0, 320, 240),

    gsDPSetOtherMode(G_AD_DISABLE | G_CD_DISABLE | CVG_DST_FULL |
        G_CK_NONE | G_TC_FILT |
        G_TD_CLAMP | G_TP_NONE |
        G_TL_TILE | G_TT_NONE |
        G_PM_NPRIMITIVE | G_CYC_1CYCLE |
        G_TF_BILERP, // HI
        G_AC_NONE | G_ZS_PRIM |
        G_RM_XLU_SURF | G_RM_XLU_SURF2), // LO
    
    gsSPEndDisplayList()
};

//main 2d drawing function, builds custom DL that func_8009292C_Hook reads
void BuildCustomDL(void) {
    Gfx* curGfxPos = customGfx;
    s32 yRoot = 3;

    if (init == 0) {
        init = 1;
        gfx_init();
        crash_screen_init();
    }

    //ControlMenuToggle();

    gSPDisplayList(curGfxPos++,&kzgfx);

    if (menuActive == 1) {
        //this function call needs to be moved elsewhere. Inputs dont work well currently
        ControlMenuMain(Page0, ARRAY_COUNT(Page0)); 

        curGfxPos = DrawMenu(curGfxPos, yRoot); //build DL
    }
    curGfxPos = DrawActiveToggles(curGfxPos, yRoot);
    gSPEndDisplayList(curGfxPos++); //end custom DL
}