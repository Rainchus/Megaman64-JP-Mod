.headersize 0x80025C00 - 0x1000
.org 0x80025C00
	LUI t0, 0x800D
	ADDIU t0, t0, 0x37A0
	LUI a0, 0x0015
	ADDIU a0, a0, 0x23D0
	bssClearLoop:
	SD r0, 0x0000 (t0)
	ADDI a0, a0, 0xFFF8
	BNEZ a0, bssClearLoop
	ADDI t0, t0, 0x0008

	//if *all* of the `LI` instructions used for the osPiRawStartDma call here expand -
	//into 2 instructions, it will leak into the next function, `80025C60`
	LUI sp, 0x801E //first half of stack pointer init
	LI a1, PAYLOAD_ROM //0x1FE0780 rom addr
	LI a3, PAYLOAD_END_RAM - PAYLOAD_VRAM //size
	LI a2, PAYLOAD_START_RAM //0x80400000 ram addr
	JAL osPiRawStartDma
	ADDIU sp, sp, 0x5F30 //second half of stack pointer init
	
	//wait on our dma to finish before continuing
	dmaBusyLoop:
	LUI t0, 0xA460
	LW t1, 0x0010 (t0)
	ANDI t1, t1, 0x0001
	BNEZ t1, dmaBusyLoop
	LUI t2, 0x8009
	
	ADDIU t2, t2, 0x9CA0
	JR t2
	NOP