/*
    Remove all unnecessary lines (including this one) 
    in this comment.
    REFER TO THE SUBMISSION INSTRUCTION FOR DETAILS

    Name 1: Xiaoyong Liang
    Name 2: Kaisheng Song
    UTEID 1: XL5432
    UTEID 2: KS38873
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
*/

#define WORDS_IN_MEM    0x08000 
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */


typedef struct System_Latches_Struct{

  int PC,		/* program counter */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P;		/* p condition bit */
  int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {                                                    
  printf("----------------LC-3b ISIM Help-----------------------\n");
  printf("go               -  run program to completion         \n");
  printf("run n            -  execute program for n instructions\n");
  printf("mdump low high   -  dump memory from low to high      \n");
  printf("rdump            -  dump the register & bus values    \n");
  printf("?                -  display this help menu            \n");
  printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  process_instruction();
  CURRENT_LATCHES = NEXT_LATCHES;
  INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {                                      
  int i;

  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++) {
    if (CURRENT_LATCHES.PC == 0x0000) {
	    RUN_BIT = FALSE;
	    printf("Simulator halted\n\n");
	    break;
    }
    cycle();
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go() {                                                     
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating...\n\n");
  while (CURRENT_LATCHES.PC != 0x0000)
    cycle();
  RUN_BIT = FALSE;
  printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {          
  int address; /* this is a byte address */

  printf("\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    printf("  0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    fprintf(dumpsim_file, " 0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */   
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {                               
  int k; 

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  printf("Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    fprintf(dumpsim_file, "%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */  
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {                         
  char buffer[20];
  int start, stop, cycles;

  printf("LC-3b-SIM> ");

  scanf("%s", buffer);
  printf("\n");

  switch(buffer[0]) {
  case 'G':
  case 'g':
    go();
    break;

  case 'M':
  case 'm':
    scanf("%i %i", &start, &stop);
    mdump(dumpsim_file, start, stop);
    break;

  case '?':
    help();
    break;
  case 'Q':
  case 'q':
    printf("Bye.\n");
    exit(0);

  case 'R':
  case 'r':
    if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
    else {
	    scanf("%d", &cycles);
	    run(cycles);
    }
    break;

  default:
    printf("Invalid Command\n");
    break;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {                                           
  int i;

  for (i=0; i < WORDS_IN_MEM; i++) {
    MEMORY[i][0] = 0;
    MEMORY[i][1] = 0;
  }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {                   
  FILE * prog;
  int ii, word, program_base;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL) {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  /* Read in the program. */
  if (fscanf(prog, "%x\n", &word) != EOF)
    program_base = word >> 1;
  else {
    printf("Error: Program file is empty\n");
    exit(-1);
  }

  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF) {
    /* Make sure it fits. */
    if (program_base + ii >= WORDS_IN_MEM) {
	    printf("Error: Program file %s is too long to fit in memory. %x\n",
             program_filename, ii);
	    exit(-1);
    }

    /* Write the word to memory array. */
    MEMORY[program_base + ii][0] = word & 0x00FF;
    MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
    ii++;
  }

  if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

  printf("Read %d words from program into memory.\n\n", ii);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */ 
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files) { 
  int i;

  init_memory();
  for ( i = 0; i < num_prog_files; i++ ) {
    load_program(program_filename);
    while(*program_filename++ != '\0');
  }
  CURRENT_LATCHES.Z = 1;  
  NEXT_LATCHES = CURRENT_LATCHES;
    
  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {                              
  FILE * dumpsim_file;

  /* Error Checking */
  if (argc < 2) {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("LC-3b Simulator\n\n");

  initialize(argv[1], argc - 1);

  if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  while (1)
    get_command(dumpsim_file);
    
}

/***************************************************************/
/* Do not modify the above code.
   You are allowed to use the following global variables in your
   code. These are defined above.

   MEMORY

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */

/***************************************************************/

enum {
	b0 = 1 ,
	b1 = 2,
	b2 = 4,
	b3 = 8,
	b4 = 16,
	b5 = 32,
	b6 = 64,
	b7 = 128,
	b8 = 256,
	b9 = 512,
	b10 = 1024,
	b11 = 2048,
	b12 = 4096,
	b13 = 8192,
	b14 = 16384,
	b15 = 32768,
};

int load16Memory(int rawAddress){
	int loadedAddress = rawAddress / 2;
	return Low16bits((MEMORY[loadedAddress][0] & 0x00FF) | ((MEMORY[loadedAddress][1] << 8) & 0xFF00));
}

int load8Memory(int rawAddress) {
	int loadedAddress = rawAddress / 2;
	if(rawAddress % 2 == 0){
		return MEMORY[loadedAddress][0];
	}
	else{
		return MEMORY[loadedAddress][1];
	}
}

void store16Memory(int rawAddress, int word){
	int storeAddress = rawAddress / 2;
	MEMORY[storeAddress][0] = (word & 0x00FF);
	MEMORY[storeAddress][1] = (word & 0xFF00) >> 8;
}

void store8Memory(int rawAddress, int word) {
	int loadedAddress = rawAddress / 2;
	if (rawAddress % 2 == 0) {
		MEMORY[loadedAddress][0] = word;
	}
	else {
		MEMORY[loadedAddress][1] = word;
	}
}

int SEXT16(int word, int bits){
	int result = word;
	int mibMask = 1 << (bits - 1);
	int mib = (word & mibMask) >> (bits - 1);
	int bit = bits;
	while (bit < 16){
		result = result & ~(1 << bit);
		result = result | mib << bit;
		bit++;
	}
	return Low16bits(result);
}

void setCondition(int word){
	NEXT_LATCHES.N = 0;
	NEXT_LATCHES.Z = 0;
	NEXT_LATCHES.P = 0;
	int value = Low16bits(word);
	if (value == 0){
		NEXT_LATCHES.Z = 1;
	}
	else if ((value & b15) > 0){
		NEXT_LATCHES.N = 1;
	}
	else{
		NEXT_LATCHES.P = 1;
	}
}

void process_instruction() {
	/*  function: process_instruction
	*
	*    Process one instruction at a time
	*       -Fetch one instruction
	*       -Decode
	*       -Execute
	*       -Update NEXT_LATCHES
	*/
	int ir = load16Memory(CURRENT_LATCHES.PC);
	int opcode = (ir >> 12) & (b0 | b1 | b2 | b3);

	switch (opcode)
	{
		/* BR */
		case 0x00:{
			NEXT_LATCHES.PC += 2;
			int cc_n = (ir >> 11) & b0;
			int cc_z = (ir >> 10) & b0;
			int cc_p = (ir >> 9) & b0;
			if ((cc_n && CURRENT_LATCHES.N) || (cc_z && CURRENT_LATCHES.Z) || (cc_p && CURRENT_LATCHES.P)){
				int bit9 = SEXT16((ir & (b0 | b1 | b2 | b3 | b4 | b5 | b6 | b7 | b8)), 9);
				int data = Low16bits(bit9 << 1);
				NEXT_LATCHES.PC = Low16bits(NEXT_LATCHES.PC + data);
			}
			break;
		}
		/* ADD */
		case 0x01:{
			NEXT_LATCHES.PC += 2;
			int bit5 = (ir >> 5) & b0;
			int dr = (ir >> 9) & (b0 | b1 | b2);
			int sr1 = (ir >> 6) & (b0 | b1 | b2);
			int data;
			if(bit5 == 0){
				int sr2 = ir & (b0 | b1 | b2);
				data = Low16bits(CURRENT_LATCHES.REGS[sr1]) + Low16bits(CURRENT_LATCHES.REGS[sr2]);
			}
			else{
				int imm = ir & (b0 | b1 | b2 | b3 | b4);
				data = Low16bits(CURRENT_LATCHES.REGS[sr1]) + SEXT16(imm, 5);
			}
			NEXT_LATCHES.REGS[dr] = Low16bits(data);
			setCondition(NEXT_LATCHES.REGS[dr]);
			break;
		}
		/* LDB */
		case 0x02:{
			NEXT_LATCHES.PC += 2;
			int dr = (ir >> 9) & (b0 | b1 | b2);
			int baseR = (ir >> 6) & (b0 | b1 | b2);
			int offset6 = SEXT16(ir & (b0 | b1 | b2 | b3 | b4 | b5), 6);
			int rawAddress = Low16bits(CURRENT_LATCHES.REGS[baseR] + SEXT16(offset6, 6));
			int data = load8Memory(rawAddress);
			NEXT_LATCHES.REGS[dr] = SEXT16(data, 8);
			setCondition(NEXT_LATCHES.REGS[dr]);
			break;
		}
		/* STB */
		case 0x03:{
			NEXT_LATCHES.PC += 2;
			int sr = (ir >> 9) & (b0 | b1 | b2);
			int baseR = (ir >> 6) & (b0 | b1 | b2);
			int offset6 = SEXT16(ir & (b0 | b1 | b2 | b3 | b4 | b5), 6);
			int rawAddress = Low16bits(CURRENT_LATCHES.REGS[baseR] + SEXT16(offset6, 6));
			int data = CURRENT_LATCHES.REGS[sr] & (b0 | b1 | b2 | b3 | b4 | b5 | b6 | b7);
			store8Memory(rawAddress, data);
			break;
		}
		/* JSR or JSRR */
		case 0x04:{
            NEXT_LATCHES.PC += 2;
            int temp = NEXT_LATCHES.PC;
            int bit11 = (ir >> 11) & b0;
            if (bit11 == 1){
                int offset11 = SEXT16((ir & (b0 | b1 | b2 | b3 | b4 | b5 | b6 | b7 | b8 | b9 | b10)), 11);
                NEXT_LATCHES.PC += SEXT16((offset11 << 1), 11);
            }
            else{
                int baseR = (ir >> 6) & (b0 | b1 | b2);
                NEXT_LATCHES.PC = CURRENT_LATCHES.REGS[baseR];
            }
			NEXT_LATCHES.PC = Low16bits(NEXT_LATCHES.PC);
            NEXT_LATCHES.REGS[7] = temp;
			break;
		}
		/* AND */
		case 0x05:{
			NEXT_LATCHES.PC += 2;
			int bit5 = (ir >> 5) & b0;
			int dr = (ir >> 9) & (b0 | b1 | b2);
			int sr1 = (ir >> 6) & (b0 | b1 | b2);
			int data;
			if (bit5 == 0){
				int sr2 = ir & (b0 | b1 | b2);
				data = Low16bits(CURRENT_LATCHES.REGS[sr1]) & Low16bits(CURRENT_LATCHES.REGS[sr2]);
			}
			else{
				int imm = ir & (b0 | b1 | b2 | b3 | b4);
				data = Low16bits(CURRENT_LATCHES.REGS[sr1]) & SEXT16(imm, 5);
			}
			NEXT_LATCHES.REGS[dr] = Low16bits(data);
			setCondition(NEXT_LATCHES.REGS[dr]);
			break;
		}
		/* LDW */
		case 0x06:{
			NEXT_LATCHES.PC += 2;
			int offset = SEXT16((ir & (b0 | b1 | b2 | b3 | b4 | b5)), 6);
			int dr = (ir >> 9) & (b0 | b1 | b2);
			int baseR = (ir >> 6) & (b0 | b1 | b2);
			int data = Low16bits(offset << 1);
			NEXT_LATCHES.REGS[dr] = load16Memory(Low16bits(CURRENT_LATCHES.REGS[baseR] + data));
			setCondition(NEXT_LATCHES.REGS[dr]);
			break;
		}
		/* STW */
		case 0x07:{
			NEXT_LATCHES.PC += 2;
			/* need review */
			int sr = (ir >> 9) & (b0 | b1 | b2);
			int baseR = (ir >> 6) & (b0 | b1 | b2);
			int offset6 = SEXT16((ir & (b0 | b1 | b2 | b3 | b4 | b5)), 6);
			int data = Low16bits(offset6 << 1);
			int temp = Low16bits(CURRENT_LATCHES.REGS[baseR] + data);
			store16Memory(temp, CURRENT_LATCHES.REGS[sr]);
			break;
		}
		/* RTI */
        /* Not contain any RTI instructions  */
		case 0x08:{
			break;
		}
		/* XOR or NOT*/
		case 0x09:{
			NEXT_LATCHES.PC += 2;
			int bit5 = (ir >> 5) & b0;
			int dr = (ir >> 9) & (b0 | b1 | b2);
			int sr1 = (ir >> 6) & (b0 | b1 | b2);
			int data;
			if (bit5 == 0){
				int sr2 = ir & (b0 | b1 | b2);
				data = Low16bits(CURRENT_LATCHES.REGS[sr1]) ^ Low16bits(CURRENT_LATCHES.REGS[sr2]);
			}
			else{
				int imm = ir & (b0 | b1 | b2 | b3 | b4);
				data = Low16bits(CURRENT_LATCHES.REGS[sr1]) ^ SEXT16(imm, 5);
			}
			NEXT_LATCHES.REGS[dr] = Low16bits(data);
			setCondition(NEXT_LATCHES.REGS[dr]);
			break;
		}
		/* NOT USED */
		case 0x0A:{

			break;
		}
		/* NOT USED */
		case 0x0B:{

			break;
		}
		/* JMP or RET */
		case 0x0C:{
			NEXT_LATCHES.PC += 2;
			int baseR = (ir >> 6) & (b0 | b1 | b2);
			NEXT_LATCHES.PC = Low16bits(CURRENT_LATCHES.REGS[baseR]);
			break;
		}
		/* SHF */
		case 0x0D:{
			NEXT_LATCHES.PC += 2;
            int dr = (ir >> 9) & (b0 | b1 | b2);
			int sr = (ir >> 6) & (b0 | b1 | b2);
            int bit5 = (ir >> 5) & (b0);
            int bit4 = (ir >> 4) & (b0);
            int amount4 = ir & (b0 | b1 | b2 | b3);
			int data = 0;
            if (!bit4){
                data = CURRENT_LATCHES.REGS[sr] << amount4;
            }
			else{
              if (!bit5){
                data = CURRENT_LATCHES.REGS[sr] >> amount4;
              }
              else{
                data = CURRENT_LATCHES.REGS[sr] >> amount4;
                data = SEXT16(data, 16 - amount4);  
              }
			}
			data = Low16bits(data);
			NEXT_LATCHES.REGS[dr] = data;
			setCondition(NEXT_LATCHES.REGS[dr]);
			break;
		}
		/* LEA */
		/* Please note that LEA does NOT set condition codes */
		case 0x0E:{
			NEXT_LATCHES.PC += 2;
			int bit9 = SEXT16((ir & (b0 | b1 | b2 | b3 | b4 | b5 | b6 | b7 | b8)), 9);
			int dr = (ir >> 9) & (b0 | b1 | b2);
			int data = Low16bits(CURRENT_LATCHES.PC + 2 + Low16bits(bit9 << 1));
			NEXT_LATCHES.REGS[dr] = Low16bits(data);
			break;
		}
		/* TRAP */
		case 0x0F:{
			NEXT_LATCHES.PC += 2;
			NEXT_LATCHES.REGS[7] = Low16bits(NEXT_LATCHES.PC);
			NEXT_LATCHES.PC = 0;
			break;
		}
	}
}
