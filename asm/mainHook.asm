.headersize 0x80025C00 - 0x1000
//.org 0x8003E1AC
	//J savestateCheckMainAsm
	//NOP

.org 0x8003E274
	J testTemp2
	NOP

.org 0x800922B8
	J rdpWaitOnSaveOrLoadAsm
	NOP

.org 0x80092174
	J rdpWaitOnSaveOrLoadAsm2
	NOP

.org 0x8008E8FC
	J WaitOnSaveOrLoadAsm3
	NOP

//.org 0x800925FC
	//J WaitOnSaveOrLoadAsm4
	//NOP

.org 0x800926C8
	J WaitOnSaveOrLoadAsm5
	NOP

//.org 0x80093A68 //probably a good hook?
	//J WaitOnSaveOrLoadAsm6
	//NOP

//.org 0x8002711C //dma related, dont hook for savestates
	//J WaitOnSaveOrLoadAsm7
	//NOP


//.org 0x8005D3D8
//.org 0x8005D34C
	//J tempTest
	//NOP

//2d drawing hook
.org 0x8009292C
	J func_8009292C_Hook
	NOP