savestateCheckMainAsm:
	JAL savestateCheckMain
	NOP
	LB v0, 0x0000 (s1)
	J 0x8003E1B4
	SLL v0, v0, 2

testTemp2: //8003E274
	JAL savestateCheckMain
	NOP
	JAL 0x800290C4
	ADDIU a0, r0, 1
	J 0x8003E27C
	NOP

rdpWaitOnSaveOrLoadAsm: //800922B8
	JAL rdpWaitOnSaveOrLoad
	NOP
	ADDU a0, s0, r0
	ADDIU a1, sp, 0x10
	JAL osRecvMesg
	ADDIU a2, r0, 1
	J 0x800922C4
	NOP

rdpWaitOnSaveOrLoadAsm2: //80092174
	JAL rdpWaitOnSaveOrLoad2
	NOP
	ADDU a0, s4, r0
	ADDIU a1, sp, 0x10
	JAL osRecvMesg
	ADDIU a2, r0, 1
	J 0x80092180
	NOP

WaitOnSaveOrLoadAsm3:
	JAL rdpWaitOnSaveOrLoad3
	NOP
	LUI v0, 0x800D
	LW v0, 0xF11C (v0)
	J 0x8008E904
	NOP

WaitOnSaveOrLoadAsm4:
	JAL rdpWaitOnSaveOrLoad4
	NOP
	LUI a0, 0x801E
	ADDIU a0, a0, 0x08C8
	ADDIU a1, sp, 0x0020
	JAL osRecvMesg
	ADDIU a2, r0, 1
	J 0x80092610
	NOP

WaitOnSaveOrLoadAsm5:
	JAL rdpWaitOnSaveOrLoad5
	NOP
	LUI a0, 0x8022
	ADDIU a0, a0, 0x5AD0
	ADDIU a1, sp, 0x0010
	JAL osRecvMesg
	ADDIU a2, r0, 1
	J 0x800926DC
	NOP

WaitOnSaveOrLoadAsm6:
	JAL rdpWaitOnSaveOrLoad6
	NOP
	LUI a0, 0x8020
	ADDIU a0, a0, 0x7440
	ADDIU a1, sp, 0x0040
	JAL osRecvMesg
	ADDIU a2, r0, 1
	J 0x80093A7C
	NOP

WaitOnSaveOrLoadAsm7:
	JAL rdpWaitOnSaveOrLoad7
	NOP
	LUI a0, 0x800D
	ADDIU a0, a0, 0x3BB0
	ADDIU a1, sp, 0x0020
	JAL osRecvMesg
	ADDIU a2, r0, 1
	J 0x80027130
	NOP

tempTest:
	JAL rdpWaitOnSaveOrLoad7
	NOP
	jal 0x8005CE18
	 nop
	J 0x8005D354
	 NOP

// MIPS assembly function for memory copy in chunks of 8 bytes
// using LD and SD instructions

// Arguments:
// $a0 = destination address
// $a1 = source address
// $a2 = number of bytes to copy

optimized_memcpy:
    addu $t0, $zero, $zero      // Counter for remaining bytes
    addu $t1, $zero, $a2        // Number of bytes to copy
    addu $t2, $a0, $t1          // End destination address
    addiu $t2, $t2, -1            // Last destination address
    
    // Loop to copy in chunks of 8 bytes
    ld_loop:
        addiu $t0, $t1, -8         // Calculate remaining bytes
        blez $t0, byte_copy      // Branch if less than 8 bytes remaining
        .word 0xDCAB0000    //ld $t3, 0($a1)           // Load 8-byte chunk from source
        .word 0xFC8B0000    //sd $t3, 0($a0)           // Store 8-byte chunk to destination
        
        addiu $a0, $a0, 8        // Increment destination address by 8
        addiu $a1, $a1, 8        // Increment source address by 8
        
        
        j ld_loop               // Jump to the beginning of the loop
            addiu $t1, $t1, -8         // Decrement remaining bytes by 8
    
    // Copy remaining bytes byte by byte
    byte_copy:
        addiu $t0, $t0, -1         // Decrement remaining bytes by 1
        blez $t0, end_memcpy    // Branch if no more bytes remaining
            lbu $t3, 0($a1)          // Load 1 byte from source
        sb $t3, 0($a0)           // Store 1 byte to destination
        addiu $a0, $a0, 1        // Increment destination address by 1
        
        
        j byte_copy             // Jump to the beginning of the loop
            addiu $a1, $a1, 1        // Increment source address by 1
    
    end_memcpy:
        jr $ra                   // Return
        nop
