
	.ORIG x3000   	
	LEA R0, TEN		;This instruction will be loaded into memory location x3000
START	ADD R1, R1, #-1
	NOP
	NOP
	BRZ DONE
DONE	BR START
	RET
	RTI
	ADD R0, R0, R0
TEN	.FILL #-2
	XOR r0, r0, r0
	TRAP x25
	HALT
	.END