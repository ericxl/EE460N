#include "stdafx.h"
#include "CppUnitTest.h"

#include "../lc3bsim2.c"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(InstructionTests)
	{
	public:
		TEST_METHOD(TestBR)
		{
			//test imm positive
			//BRp 3
			loadProgram(0x3000, new int{
				0x0203
			});
			CURRENT_LATCHES.P = 1;
			CURRENT_LATCHES.Z = 0;
			CURRENT_LATCHES.N = 0;
			cycle();
			Assert::AreEqual(0x6008, NEXT_LATCHES.PC);

			//test imm neg 
			//BRp -2
			loadProgram(0x3000, new int{
				0x03FE
			});
			CURRENT_LATCHES.P = 1;
			CURRENT_LATCHES.Z = 0;
			CURRENT_LATCHES.N = 0;
			cycle();
			Assert::AreEqual(0x6000 - 2, NEXT_LATCHES.PC);

			//test brn branched 
			//BRn 3
			loadProgram(0x3000, new int{
				0x0803
			});
			CURRENT_LATCHES.P = 0;
			CURRENT_LATCHES.Z = 0;
			CURRENT_LATCHES.N = 1;
			cycle();
			Assert::AreEqual(0x6008, NEXT_LATCHES.PC);

			//test brn not branched 
			//BRn 3
			loadProgram(0x3000, new int{
				0x0803
			});
			CURRENT_LATCHES.P = 1;
			CURRENT_LATCHES.Z = 1;
			CURRENT_LATCHES.N = 0;
			cycle();
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);

			//test brz branched 
			//BRn 3
			loadProgram(0x3000, new int{
				0x0403
			});
			CURRENT_LATCHES.P = 0;
			CURRENT_LATCHES.Z = 1;
			CURRENT_LATCHES.N = 1;
			cycle();
			Assert::AreEqual(0x6008, NEXT_LATCHES.PC);

			//test brz not branched 
			//BRn 3
			loadProgram(0x3000, new int{
				0x0403
			});
			CURRENT_LATCHES.P = 1;
			CURRENT_LATCHES.Z = 0;
			CURRENT_LATCHES.N = 1;
			cycle();
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);

			//test brp branched 
			//BRn 3
			loadProgram(0x3000, new int{
				0x0203
			});
			CURRENT_LATCHES.P = 1;
			CURRENT_LATCHES.Z = 0;
			CURRENT_LATCHES.N = 1;
			cycle();
			Assert::AreEqual(0x6008, NEXT_LATCHES.PC);

			//test brp not branched 
			//BRn 3
			loadProgram(0x3000, new int{
				0x0203
			});
			CURRENT_LATCHES.P = 0;
			CURRENT_LATCHES.Z = 0;
			CURRENT_LATCHES.N = 1;
			cycle();
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);

			//test br branched 
			//BRn 3
			loadProgram(0x3000, new int{
				0x0003
			});
			CURRENT_LATCHES.P = 1;
			CURRENT_LATCHES.Z = 0;
			CURRENT_LATCHES.N = 1;
			cycle();
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);

			//test brnzp branched 
			//BRn 3
			loadProgram(0x3000, new int{
				0x0E03
			});
			CURRENT_LATCHES.P = 1;
			CURRENT_LATCHES.Z = 0;
			CURRENT_LATCHES.N = 1;
			cycle();
			Assert::AreEqual(0x6008, NEXT_LATCHES.PC);

		}

		TEST_METHOD(TestADD)
		{
			//immediate positive
			//ADD R1, R1, 15
			loadProgram(0x3000, new int{
				0x126F
			});
			CURRENT_LATCHES.REGS[1] = 1;
			cycle();
			Assert::AreEqual(16, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(1, NEXT_LATCHES.P);

			//immediate negative
			//ADD R1, R1, -1
			loadProgram(0x3000, new int {
				0x127F
			});
			CURRENT_LATCHES.REGS[1] = 0;
			cycle();
			Assert::AreEqual(0xFFFF, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(1, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//immediate positive overflowed
			//ADD R1, R1, 1
			loadProgram(0x3000, new int{
				0x1261
			});
			CURRENT_LATCHES.REGS[1] = 65535;
			cycle();
			Assert::AreEqual(0, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(1, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//immediate negative overflowed
			//ADD R1, R1, -1
			loadProgram(0x3000, new int{
				0x127F
			});
			CURRENT_LATCHES.REGS[1] = 32768;
			cycle();
			Assert::AreEqual(0x7FFF, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(1, NEXT_LATCHES.P);


			//reg positive
			// ADD R1, R1, R2
			loadProgram(0x3000, new int{
				0x1242
			});
			CURRENT_LATCHES.REGS[1] = 1;
			CURRENT_LATCHES.REGS[2] = 59;
			cycle();
			Assert::AreEqual(60, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(1, NEXT_LATCHES.P);

			//reg negative
			// ADD R1, R1, R2
			loadProgram(0x3000, new int{
				0x1242
			});
			CURRENT_LATCHES.REGS[1] = 0xFFFF;
			CURRENT_LATCHES.REGS[2] = 0xFFFE;
			cycle();
			Assert::AreEqual(0xFFFD, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(1, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//reg positive overflowed
			// ADD R1, R1, R2
			loadProgram(0x3000, new int{
				0x1242
			});
			CURRENT_LATCHES.REGS[1] = 65535;
			CURRENT_LATCHES.REGS[2] = 1;
			cycle();
			Assert::AreEqual(0, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(1, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//reg negative overflowed
			// ADD R1, R1, R2
			loadProgram(0x3000, new int{
				0x1242
			});
			CURRENT_LATCHES.REGS[1] = 32768;
			CURRENT_LATCHES.REGS[2] = 0xFFFF;
			cycle();
			Assert::AreEqual(0x7FFF, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(1, NEXT_LATCHES.P);

			//cc always set for positive
			// ADD R1, R1, 0
			loadProgram(0x3000, new int{
				0x1260
			});
			CURRENT_LATCHES.REGS[1] = 16;
			cycle();
			Assert::AreEqual(16, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(1, NEXT_LATCHES.P);

			//cc always set for positive
			// ADD R1, R1, 0
			loadProgram(0x3000, new int{
				0x1260
			});
			CURRENT_LATCHES.REGS[1] = 0;
			cycle();
			Assert::AreEqual(0, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(1, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//memory overflowed
			// ADD R1, R1, R2
			loadProgram(0x3000, new int{
				0x1242
			});
			CURRENT_LATCHES.REGS[1] = 0x80000000;
			CURRENT_LATCHES.REGS[2] = 0x80000000;
			cycle();
			Assert::AreEqual(0x0000, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(1, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//Test pc increment
			//ADD R1, R1, 15
			loadProgram(0x3000, new int{
				0x126F
			});
			cycle();
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);
		}

		TEST_METHOD(TestLDB)
		{
			//LDB R0, R1, #0
			loadProgram(0x3000, new int{
				0x2040
			});
			MEMORY[0x3010][0] = 0x77;
			CURRENT_LATCHES.REGS[1] = 0x6020;
			cycle();
			Assert::AreEqual(0x77, NEXT_LATCHES.REGS[0]);

			//LDB R0, R1, #1
			loadProgram(0x3000, new int{
				0x2040
			});
			MEMORY[0x3010][1] = 0x77;
			CURRENT_LATCHES.REGS[1] = 0x6021;
			cycle();
			Assert::AreEqual(0x77, NEXT_LATCHES.REGS[0]);

			//LDB R0, R1, #-1
			loadProgram(0x3000, new int{
				0x207F
			});
			MEMORY[0x3003][0] = 0x77;
			CURRENT_LATCHES.REGS[1] = 0x6007;
			cycle();
			Assert::AreEqual(0x77, NEXT_LATCHES.REGS[0]);

			//LDB R0, R1, #-1
			loadProgram(0x3000, new int{
				0x207F
			});
			MEMORY[0x3003][1] = 0x77;
			CURRENT_LATCHES.REGS[1] = 0x6008;
			cycle();
			Assert::AreEqual(0x77, NEXT_LATCHES.REGS[0]);
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);
		}

		TEST_METHOD(TestSTB)
		{
			//STB R0, R1, #0
			loadProgram(0x3000, new int{
				0x3040
			});
			CURRENT_LATCHES.REGS[0] = 0x66DD;
			CURRENT_LATCHES.REGS[1] = 0x6020;
			cycle();
			Assert::AreEqual(0xDD, MEMORY[0x3010][0]);

			//STB R0, R1, #1
			loadProgram(0x3000, new int{
				0x3040
			});
			CURRENT_LATCHES.REGS[0] = 0x6655;
			CURRENT_LATCHES.REGS[1] = 0x6021;
			cycle();
			Assert::AreEqual(0x55, MEMORY[0x3010][1]);

			//STB R0, R1, #-1
			loadProgram(0x3000, new int{
				0x307F
			});
			CURRENT_LATCHES.REGS[0] = 0x6699;
			CURRENT_LATCHES.REGS[1] = 0x6044;
			cycle();
			Assert::AreEqual(0x99, MEMORY[0x3021][1]);

			//STB R0, R1, #-1
			loadProgram(0x3000, new int{
				0x307F
			});
			CURRENT_LATCHES.REGS[0] = 0x6688;
			CURRENT_LATCHES.REGS[1] = 0x6043;
			cycle();
			Assert::AreEqual(0x88, MEMORY[0x3021][0]);
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);
		}

		TEST_METHOD(TestJSR)
		{
			//JSR #0
			loadProgram(0x3000, new int{
				0x4800
			});
			cycle();
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);
			Assert::AreEqual(0x6002, NEXT_LATCHES.REGS[7]);

			//JSR #1
			loadProgram(0x3000, new int{
				0x4801
			});
			cycle();
			Assert::AreEqual(0x6004, NEXT_LATCHES.PC);
			Assert::AreEqual(0x6002, NEXT_LATCHES.REGS[7]);

			//JSR #-2
			loadProgram(0x3004, new int{
				0x4FFE
			});
			cycle();
			Assert::AreEqual(0x6006, NEXT_LATCHES.PC);
			Assert::AreEqual(0x600A, NEXT_LATCHES.REGS[7]);

			//JSRR R0
			loadProgram(0x3000, new int{
				0x4000
			});
			CURRENT_LATCHES.REGS[0] = 0x6022;
			cycle();
			Assert::AreEqual(0x6022, NEXT_LATCHES.PC);
			Assert::AreEqual(0x6002, NEXT_LATCHES.REGS[7]);
		}

		TEST_METHOD(TestAND)
		{
			//immediate
			//AND R1, R1, 15
			loadProgram(0x3000, new int{
				0x526F
			});
			CURRENT_LATCHES.REGS[1] = 0xFF78;
			cycle();
			Assert::AreEqual(0x0008, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(1, NEXT_LATCHES.P);

			//immediate neg
			//AND R1, R1, -1
			loadProgram(0x3000, new int{
				0x527F
			});
			CURRENT_LATCHES.REGS[1] = 0x3953;
			cycle();
			Assert::AreEqual(0x3953, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(1, NEXT_LATCHES.P);

			//reg neg
			//AND R1, R1, R2
			loadProgram(0x3000, new int{
				0x5242
			});
			CURRENT_LATCHES.REGS[1] = 0xAAAA;
			CURRENT_LATCHES.REGS[2] = 0x5555;
			cycle();
			Assert::AreEqual(0, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(1, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//cc always set for positive
			//AND R1, R1, 0
			loadProgram(0x3000, new int{
				0x5260
			});
			CURRENT_LATCHES.REGS[1] = 0;
			cycle();
			Assert::AreEqual(0, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(1, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//memory overflowed
			//AND R1, R1, R2
			loadProgram(0x3000, new int{
				0x5242
			});
			CURRENT_LATCHES.REGS[1] = 0x80000000;
			CURRENT_LATCHES.REGS[2] = 0x80000000;
			cycle();
			Assert::AreEqual(0x0000, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(1, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//Test pc increment
			loadProgram(0x3000, new int{
				0x526F
			});
			cycle();
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);
		}

		TEST_METHOD(TestLDW)
		{
			//LDW R0, R1, #0
			loadProgram(0x3000, new int{
				0x6040
			});
			MEMORY[0x3010][0] = 0x77;
			MEMORY[0x3010][1] = 0x33;
			CURRENT_LATCHES.REGS[1] = 0x6020;
			cycle();
			Assert::AreEqual(0x3377, NEXT_LATCHES.REGS[0]);

			//LDW R0, R1, #3
			loadProgram(0x3000, new int{
				0x6043
			});
			MEMORY[0x3013][0] = 0x77;
			MEMORY[0x3013][1] = 0x33;
			CURRENT_LATCHES.REGS[1] = 0x6020;
			cycle();
			Assert::AreEqual(0x3377, NEXT_LATCHES.REGS[0]);

			//LDW R0, R1, #-2
			loadProgram(0x3000, new int{
				0x607E
			});
			MEMORY[0x300E][0] = 0x77;
			MEMORY[0x300E][1] = 0x33;
			CURRENT_LATCHES.REGS[1] = 0x6020;
			cycle();
			Assert::AreEqual(0x3377, NEXT_LATCHES.REGS[0]);
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);
		}

		TEST_METHOD(TestSTW)
		{
			//STW R0, R1, #0
			loadProgram(0x3000, new int{
				0x7040
			});
			CURRENT_LATCHES.REGS[0] = 0x3377;
			CURRENT_LATCHES.REGS[1] = 0x6020;
			cycle();
			Assert::AreEqual(0x77, MEMORY[0x3010][0]);
			Assert::AreEqual(0x33, MEMORY[0x3010][1]);

			//STW R0, R1, #3
			loadProgram(0x3000, new int{
				0x7043
			});
			CURRENT_LATCHES.REGS[0] = 0x13377;
			CURRENT_LATCHES.REGS[1] = 0x66020;
			cycle();
			Assert::AreEqual(0x77, MEMORY[0x3013][0]);
			Assert::AreEqual(0x33, MEMORY[0x3013][1]);

			//STW R0, R1, #-2
			loadProgram(0x3000, new int{
				0x707E
			});
			CURRENT_LATCHES.REGS[0] = 0x5588;
			CURRENT_LATCHES.REGS[1] = 0x6020;
			cycle();
			Assert::AreEqual(0x88, MEMORY[0x300E][0]);
			Assert::AreEqual(0x55, MEMORY[0x300E][1]);
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);
		}

		TEST_METHOD(TestXOR)
		{
			//immediate
			//XOR R1, R1, 15
			loadProgram(0x3000, new int{
				0x926F
			});
			CURRENT_LATCHES.REGS[1] = 0xFF78;
			cycle();
			Assert::AreEqual(0xFF77, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(1, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//immediate neg
			//XOR R1, R1, -1
			//NOT R1, R1
			loadProgram(0x3000, new int{
				0x927F
			});
			CURRENT_LATCHES.REGS[1] = 0x3953 ;
			cycle();
			Assert::AreEqual(0xC6AC, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(1, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//reg neg
			//XOR R1, R1, R2
			loadProgram(0x3000, new int{
				0x9242
			});
			CURRENT_LATCHES.REGS[1] = 0xAAAA;
			CURRENT_LATCHES.REGS[2] = 0x5555;
			cycle();
			Assert::AreEqual(0xFFFF, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(1, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//cc always set for positive
			//XOR R1, R1, 0
			loadProgram(0x3000, new int{
				0x9260
			});
			CURRENT_LATCHES.REGS[1] = 0;
			cycle();
			Assert::AreEqual(0, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(0, NEXT_LATCHES.N);
			Assert::AreEqual(1, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//memory overflowed
			//XOR R1, R1, R2
			loadProgram(0x3000, new int{
				0x9242
			});
			CURRENT_LATCHES.REGS[1] = 0x39533953;
			CURRENT_LATCHES.REGS[2] = 0xFFFFFFFF;
			cycle();
			Assert::AreEqual(0xC6AC, NEXT_LATCHES.REGS[1]);
			Assert::AreEqual(1, NEXT_LATCHES.N);
			Assert::AreEqual(0, NEXT_LATCHES.Z);
			Assert::AreEqual(0, NEXT_LATCHES.P);

			//Test pc increment
			loadProgram(0x3000, new int{
				0x926F
			});
			cycle();
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);
		}

		TEST_METHOD(TestJMP_RET)
		{
			//JMP R1
			loadProgram(0x3000, new int{
				0xC040
			});
			CURRENT_LATCHES.REGS[1] = 0xFF78;
			cycle();
			Assert::AreEqual(0xFF78, NEXT_LATCHES.PC);

			//RET
			loadProgram(0x3000, new int{
				0xC1C0
			});
			CURRENT_LATCHES.REGS[7] = 0xFF78;
			cycle();
			Assert::AreEqual(0xFF78, NEXT_LATCHES.PC);
		}

		TEST_METHOD(TestSHF)
		{
			//LSHF R0,R0,0
			loadProgram(0x3000, new int{
				0xD000
			});
			CURRENT_LATCHES.REGS[0] = 0xFF78;
			cycle();
			Assert::AreEqual(0xFF78, NEXT_LATCHES.REGS[0]);

			//LSHF R0,R0,1
			loadProgram(0x3000, new int{
				0xD001
			});
			CURRENT_LATCHES.REGS[0] = 0xFF78;
			cycle();
			Assert::AreEqual(0xFEF0, NEXT_LATCHES.REGS[0]);

			//LSHF R0,R0,15
			loadProgram(0x3000, new int{
				0xD00F
			});
			CURRENT_LATCHES.REGS[0] = 0xFF77;
			cycle();
			Assert::AreEqual(0x8000, NEXT_LATCHES.REGS[0]);

			//RSHFL R0,R0,3
			loadProgram(0x3000, new int{
				0xD013
			});
			CURRENT_LATCHES.REGS[0] = 0xFF78;
			cycle();
			Assert::AreEqual(0x1FEF, NEXT_LATCHES.REGS[0]);

			//RSHFL R0,R0,3
			loadProgram(0x3000, new int{
				0xD013
			});
			CURRENT_LATCHES.REGS[0] = 0x7F78;
			cycle();
			Assert::AreEqual(0xFEF, NEXT_LATCHES.REGS[0]);

			//RSHFA R0,R0,3
			loadProgram(0x3000, new int{
				0xD033
			});
			CURRENT_LATCHES.REGS[0] = 0xFF78;
			cycle();
			Assert::AreEqual(0xFFEF, NEXT_LATCHES.REGS[0]);

			//RSHFA R0,R0,0
			loadProgram(0x3000, new int{
				0xD030
			});
			CURRENT_LATCHES.REGS[0] = 0xFF78;
			cycle();
			Assert::AreEqual(0xFF78, NEXT_LATCHES.REGS[0]);

			//RSHFA R0,R0,15
			loadProgram(0x3000, new int{
				0xD03F
			});
			CURRENT_LATCHES.REGS[0] = 0x8000;
			cycle();
			Assert::AreEqual(0xFFFF, NEXT_LATCHES.REGS[0]);

			//RSHFA R0,R0,3
			loadProgram(0x3000, new int{
				0xD033
			});
			CURRENT_LATCHES.REGS[0] = 0x7F78;
			cycle();
			Assert::AreEqual(0x0FEF, NEXT_LATCHES.REGS[0]);
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);
		}

		TEST_METHOD(TestLEA)
		{
			//LEA R0, #0
			loadProgram(0x3000, new int{
				0xE000
			});
			cycle();
			Assert::AreEqual(0x6002, NEXT_LATCHES.REGS[0]);

			//LEA R0, #3
			loadProgram(0x3000, new int{
				0xE003
			});
			cycle();
			Assert::AreEqual(0x6008, NEXT_LATCHES.REGS[0]);


			//LEA R0, #-2
			loadProgram(0x3000, new int{
				0xE1FE
			});
			cycle();
			Assert::AreEqual(0x5FFE, NEXT_LATCHES.REGS[0]);
			Assert::AreEqual(0x6002, NEXT_LATCHES.PC);
		}

		TEST_METHOD(TestTRAP)
		{

		}
	private:
		void loadProgram(int programBase, int* program)
		{
			init_memory();
			for (int i = 0; i < sizeof(program); i++)
			{
				int word = program[i];
				MEMORY[programBase + i][0] = word & 0x00FF;
				MEMORY[programBase + i][1] = (word >> 8) & 0x00FF;
			}
			CURRENT_LATCHES.PC = (programBase << 1);
			CURRENT_LATCHES.Z = 1;
			NEXT_LATCHES = CURRENT_LATCHES;
		}
	};

	TEST_CLASS(UnitTests)
	{
	public:

		TEST_METHOD(TestSEXT16)
		{
			Assert::AreEqual(0xFFFF, SEXT16(1, 1));
			Assert::AreEqual(1, SEXT16(1, 2));
			Assert::AreEqual(13, SEXT16(13, 9));
			Assert::AreEqual(65521, SEXT16(17, 5));
			Assert::AreEqual(21, SEXT16(213, 6));
			Assert::AreEqual(0, SEXT16(0, 6));
			Assert::AreEqual(1, SEXT16(1, 2));
			Assert::AreEqual(65525, SEXT16(245, 5));
		}

	};
}