;***********************************************************
.ORIG x4000
LOAD_ALIGNED        AND   R2, R1, #1
                    BRp   UNALIGNED
					LDW   R0, R1, #0
					BR LA_EXIT
UNALIGNED			LDB   R0, R1, #0
					LDB   R2, R1, #1
					LSHF  R2, R2, #8
					NOT   R0, R0
					NOT   R2, R2
					AND   R0, R0, R2
					NOT   R0, R0
LA_EXIT             RET
