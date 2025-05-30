.headersize 0x80025C00 - 0x1000
.org 0x8003E1AC
	J savestateCheckMainAsm
	NOP

.org 0x800922B8
	J rdpWaitOnSaveOrLoadAsm
	NOP

//.org 0x80092174
	//J rdpWaitOnSaveOrLoadAsm2
	//NOP

//.org 0x8008E8FC
	//J WaitOnSaveOrLoadAsm3
	//NOP

//.org 0x800925FC
	//J WaitOnSaveOrLoadAsm4
	//NOP

.org 0x800926C8
	J WaitOnSaveOrLoadAsm5
	NOP

//.org 0x80093A68
	//J WaitOnSaveOrLoadAsm6
	//NOP

//.org 0x8002711C
	//J WaitOnSaveOrLoadAsm7
	//NOP

.org 0x8003CF4C
	J func_8003CF4C_Hook
	NOP

.org 0x8009292C
	J func_8009292C_Hook
	NOP

//.org 0x800280A8
	//JAL drawCustomMainAsm
	//NOP

//.org 0x800280C8
	//NOP //SW r0, 0x0004 (a1) //writes 0x00000000 of 0xDF000000 00000000 command

//.org 0x80025C60
	//J func_80025C60_Hook
	//NOP

//.org 0x80025D14
	//J func_80025D14_Hook
	//NOP