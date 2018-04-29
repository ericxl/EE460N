#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

/* Note: MAX_LINE_LENGTH, OK, EMPTY_LINE, and DONE are defined values */
#define MAX_LABEL_LEN 20
#define MAX_LINE_LENGTH 255

enum {
	DONE, OK, EMPTY_LINE
};

enum OpCode
{
	ADD,
	AND,
	BR,
	BRN,
	BRZ,
	BRP,
	BRNZ,
	BRNP,
	BRZP,
	BRNZP,
	HALT,
	JMP,
	JSR,
	JSRR,
	LDB,
	LDW,
	LEA,
	NOP,
	NOT,
	RET,
	LSHF,
	RSHFL,
	RSHFA,
	RTI,
	STB,
	STW,
	XOR,
	TRAP,
	ORIG,
	FILL,
	END,
	NUMBER_OF_OP_CODES
};
enum Register
{
	R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, NUMBER_OF_REGISTERS
};

unsigned int startAddress;
unsigned short currentLine;
unsigned short pcLine;

struct {
	char labelName[MAX_LABEL_LEN + 1];
	int address;
} labelList[MAX_LINE_LENGTH];

int labelNum = 0;

char* getOpCode(int code) {
	switch (code) {
	case ADD: return "add"; break;
	case AND: return "and"; break;
	case BR: return "br"; break;
	case BRN: return "brn"; break;
	case BRZ: return "brz"; break;
	case BRP: return "brp"; break;
	case BRNZ: return "brnz"; break;
	case BRNP: return "brnp"; break;
	case BRZP: return "brzp"; break;
	case BRNZP: return "brnzp"; break;
	case HALT: return "halt"; break;
	case JMP: return "jmp"; break;
	case JSR: return "jsr"; break;
	case JSRR: return "jsrr"; break;
	case LDB: return "ldb"; break;
	case LDW: return "ldw"; break;
	case LEA: return "lea"; break;
	case NOP: return "nop"; break;
	case NOT: return "not"; break;
	case RET: return "ret"; break;
	case LSHF: return "lshf"; break;
	case RSHFL: return "rshfl"; break;
	case RSHFA: return "rshfa"; break;
	case RTI: return "rti"; break;
	case STB: return "stb"; break;
	case STW: return "stw"; break;
	case XOR: return "xor"; break;
	case TRAP: return "trap"; break;
	case ORIG: return ".orig"; break;
	case FILL: return ".fill"; break;
	case END: return ".end"; break;
	default: return "";
	}
}

int getOpcode(char *opcode) {
	if (!strcmp(opcode, "add")) return 1;
	else if (!strcmp(opcode, "and")) return 5;
	else if (!strcmp(opcode, "br")) return 0;
	else if (!strcmp(opcode, "brn")) return 0;
	else if (!strcmp(opcode, "brz")) return 0;
	else if (!strcmp(opcode, "brp")) return 0;
	else if (!strcmp(opcode, "brnz")) return 0;
	else if (!strcmp(opcode, "brnp")) return 0;
	else if (!strcmp(opcode, "brzp")) return 0;
	else if (!strcmp(opcode, "brnzp")) return 0;
	else if (!strcmp(opcode, "halt")) return 25;
	else if (!strcmp(opcode, "jmp")) return 12;
	else if (!strcmp(opcode, "jsr")) return 4;
	else if (!strcmp(opcode, "jsrr")) return 4;
	else if (!strcmp(opcode, "ldb")) return 2;
	else if (!strcmp(opcode, "ldw")) return 6;
	else if (!strcmp(opcode, "lea")) return 12;
	else if (!strcmp(opcode, "nop")) return 0;
	else if (!strcmp(opcode, "not")) return 9;
	else if (!strcmp(opcode, "ret")) return 12;
	else if (!strcmp(opcode, "lshf")) return 13;
	else if (!strcmp(opcode, "rshfl")) return 13;
	else if (!strcmp(opcode, "rshfa")) return 13;
	else if (!strcmp(opcode, "rti")) return 18;
	else if (!strcmp(opcode, "stb")) return 3;
	else if (!strcmp(opcode, "stw")) return 7;
	else if (!strcmp(opcode, "xor")) return 9;
	else if (!strcmp(opcode, ".orig")) return 20;
	else if (!strcmp(opcode, ".fill")) return 0;
	else if (!strcmp(opcode, ".end")) return 0;
	else if (!strcmp(opcode, "trap")) return 15;
	else return -1;
}

char* getReg(int reg)
{
	switch (reg)
	{
	case R0: return "r0"; break;
	case R1: return "r1"; break;
	case R2: return "r2"; break;
	case R3: return "r3"; break;
	case R4: return "r4"; break;
	case R5: return "r5"; break;
	case R6: return "r6"; break;
	case R7: return "r7"; break;
	case R8: return "r8"; break;
	case R9: return "r9"; break;
	case R10: return "r10"; break;
	case R11: return "r11"; break;
	default:return "";
	}
}

int toNum(char * pStr)
{
	char * t_ptr;
	char * orig_pStr;
	int t_length, k;
	int lNum, lNeg = 0;
	long int lNumLong;
	orig_pStr = pStr;
	if (*pStr == '#') {               /* decimal */
		pStr++;
		if (*pStr == '-') {              /* dec is negative */
			lNeg = 1;
			pStr++;
		}
		t_ptr = pStr;
		t_length = strlen(t_ptr);
		for (k = 0; k < t_length; k++) {
			if (!isdigit(*t_ptr)) {
				printf("Error: invalid decimal operand, %s\n", orig_pStr);
				exit(4);
			}
			t_ptr++;
		}
		lNum = atoi(pStr);
		if (lNeg)
			lNum = -lNum;

		return lNum;
	}
	else if (*pStr == 'x') {	/* hex     */
		pStr++;
		if (*pStr == '-') {				/* hex is negative */
			lNeg = 1;
			pStr++;
		}
		t_ptr = pStr;
		t_length = strlen(t_ptr);
		for (k = 0; k < t_length; k++) {
			if (!isxdigit(*t_ptr)) {
				printf("Error: invalid hex operand, %s\n", orig_pStr);
				exit(4);
			}
			t_ptr++;
		}
		lNumLong = strtol(pStr, NULL, 16);    /* convert hex string into integer */
		lNum = (lNumLong > INT_MAX) ? INT_MAX : lNumLong;
		if (lNeg)
			lNum = -lNum;
		return lNum;
	}
	else {
		printf("Error: invalid operand, %s\n", orig_pStr);
		exit(4);  /* This has been changed from error code 3 to error code 4, see clarification 12 */
	}
}

int isOpcode(char* code) {
	for (int i = 0; i < NUMBER_OF_OP_CODES; i++) {
		if (strcmp(code, getOpCode(i)) == 0) {
			return 0;
		}
	}
	return -1;
}

int readAndParseOne(FILE * pInfile, char * pLine, char ** pLabel, char ** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4) {
	char * lRet, *lPtr;
	int i;
	if (!fgets(pLine, MAX_LINE_LENGTH, pInfile)) {
		return(DONE);
	}
	for (i = 0; i < strlen(pLine); i++) {
		if (pLine[i] == ';') {
			pLine[i] = '\0';
		}
		pLine[i] = tolower(pLine[i]);

	}

	/* convert entire line to lowercase */
	*pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

	/* ignore the comments */
	lPtr = pLine;

	if (!(lPtr = strtok(pLine, "\t\n ,"))) {
		return(EMPTY_LINE);
	}

	int opCode = getOpcode(lPtr);

	if (opCode == -1 && lPtr[0] != '.') { /* found a label */
		*pLabel = lPtr;
		//add lebel name and address to labeltable
		strcpy(labelList[labelNum].labelName, *pLabel);
		labelList[labelNum].address = currentLine;
		labelNum += 1;

		if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);
	}

	*pOpcode = lPtr;

	if (!strcmp(*pOpcode, ".end")) {
		return DONE;
	}

	if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);

	*pArg1 = lPtr;

	if (opCode == 20) startAddress = toNum(*pArg1);

	if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);

	*pArg2 = lPtr;
	if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);

	*pArg3 = lPtr;
	if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);

	*pArg4 = lPtr;

	return(OK);
}

int readAndParseTwo(FILE * pInfile, char * pLine, char ** pLabel, char ** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4) {
	char * lRet, *lPtr;
	int i;
	if (!fgets(pLine, MAX_LINE_LENGTH, pInfile))
		return(DONE);
	for (i = 0; i < strlen(pLine); i++)
		pLine[i] = tolower(pLine[i]);

	/* convert entire line to lowercase */
	*pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

	/* ignore the comments */
	lPtr = pLine;

	while (*lPtr != ';' && *lPtr != '\0' &&
		*lPtr != '\n')
		lPtr++;

	*lPtr = '\0';
	if (!(lPtr = strtok(pLine, "\t\n ,")))
		return(EMPTY_LINE);

	if (isOpcode(lPtr) == -1 && lPtr[0] != '.') /* found a label */
	{
		*pLabel = lPtr;
		if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);
	}

	*pOpcode = lPtr;

	if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);

	*pArg1 = lPtr;

	if (!strcmp(*pOpcode, ".end")) {
		return DONE;
	}

	if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);

	*pArg2 = lPtr;
	if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);

	*pArg3 = lPtr;

	if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);

	*pArg4 = lPtr;

	return(OK);
}

int getRegister(char * str) {
	if (str[0] == 'r' && str[1] <= '7' && str[1] >= '0') {
		int temp = str[1] - '0';
		if (temp < 0 || temp > 7) {
			printf("Invalid Argument!");
			exit(4);
		}
		return temp;
	}
	else {
		return -1;
	}
	return 0;
}

int getRegAddress(char * pArg1) {
	for (int i = 0; i < MAX_LINE_LENGTH; i++) {
		if (!strcmp(labelList[i].labelName, pArg1)) {
			return labelList[i].address;
		}
	}
	return -1;
}

int compileLine(char * pLine, char * label, char * opcode, char * arg1, char * arg2, char * arg3, char * arg4) {
	int arg1Num, arg2Num, arg3Num = 0, arg4Add, offset, opCodeNum, conditCode, ans = 0;
	if (strcmp(getOpCode(ADD), opcode) == 0) {
		opCodeNum = 1;
		arg1Num = getRegister(arg1);
		arg2Num = getRegister(arg2);
		if (arg3[0] == 'r') {
			arg3Num = getRegister(arg3);
			return (((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 6) + arg3Num;
		}
		else if (arg3[0] == '#' || arg3[0] == 'x') {
			arg3Num = toNum(arg3);
			offset = arg3Num & 31;
			return (((((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 1) + 1) << 5) + (offset > 0 ? offset : (31 & offset));
		}
		else {
			printf("Invalid Argument!");
			exit(4);
		}

	}
	else if (strcmp(getOpCode(AND), opcode) == 0){
		opCodeNum = 3;
		arg1Num = getRegister(arg1);
		arg2Num = getRegister(arg2);
		if (arg3[0] == 'r') {
			arg3Num = getRegister(arg3);
			return (((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 6) + arg3Num;
		}
		else if (arg3[0] == '#' || arg3[0] == 'x') {
			arg3Num = toNum(arg3);
			offset = arg3Num & 31;
			return (((((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 1) + 1) << 5) + (offset > 0 ? offset : (31 & offset));
		}
		else {
			printf("Invalid Argument!");
			exit(4);
		}
	}
	else if (strcmp(getOpCode(BR), opcode) == 0) {
		opCodeNum = 0;
		conditCode = 7;
		arg1Num = getRegAddress(arg1);
		if (arg1Num == -1) {
			printf("Invalid Argument!");
			exit(4);
		}
		else {
			offset = (arg1Num - currentLine) / 2;
		}
		return (((opCodeNum << 3) + conditCode) << 9) + (offset > 0 ? offset : (511 & offset));
	}
	else if (strcmp(getOpCode(BRN), opcode) == 0) {
		opCodeNum = 0;
		conditCode = 4;
		arg1Num = getRegAddress(arg1);
		if (arg1Num == -1) {
			printf("Invalid Argument!");
			exit(4);
		}
		else {
			offset = (arg1Num - currentLine) / 2;
		}
		return (((opCodeNum << 3) + conditCode) << 9) + (offset > 0 ? offset : (511 & offset));
	}
	else if (strcmp(getOpCode(BRZ), opcode) == 0) {
		opCodeNum = 0;
		conditCode = 2;
		arg1Num = getRegAddress(arg1);
		if (arg1Num == -1) {
			printf("Invalid Argument!");
			exit(4);
		}
		else {
			offset = (arg1Num - currentLine) / 2;
		}
		return (((opCodeNum << 3) + conditCode) << 9) + (offset > 0 ? offset : (511 & offset));
	}
	else if (strcmp(getOpCode(BRP), opcode) == 0) {
		opCodeNum = 0;
		conditCode = 1;
		arg1Num = getRegAddress(arg1);
		if (arg1Num == -1) {
			printf("Invalid Argument!");
			exit(4);
		}
		else {
			offset = (arg1Num - currentLine) / 2;
		}
		return (((opCodeNum << 3) + conditCode) << 9) + (offset > 0 ? offset : (511 & offset));
	}
	else if (strcmp(getOpCode(BRNZ), opcode) == 0) {
		opCodeNum = 0;
		conditCode = 6;
		arg1Num = getRegAddress(arg1);
		if (arg1Num == -1) {
			printf("Invalid Argument!");
			exit(4);
		}
		else {
			offset = (arg1Num - currentLine) / 2;
		}
		return (((opCodeNum << 3) + conditCode) << 9) + (offset > 0 ? offset : (511 & offset));
	}
	else if (strcmp(getOpCode(BRNP), opcode) == 0) {
		opCodeNum = 0;
		conditCode = 5;
		arg1Num = getRegAddress(arg1);
		if (arg1Num == -1) {
			printf("Invalid Argument!");
			exit(4);
		}
		else {
			offset = (arg1Num - currentLine) / 2;
		}
		return (((opCodeNum << 3) + conditCode) << 9) + (offset > 0 ? offset : (511 & offset));
	}
	else if (strcmp(getOpCode(BRZP), opcode) == 0) {
		opCodeNum = 0;
		conditCode = 3;
		arg1Num = getRegAddress(arg1);
		if (arg1Num == -1) {
			printf("Invalid Argument!");
			exit(4);
		}
		else {
			offset = (arg1Num - currentLine) / 2;
		}
		return (((opCodeNum << 3) + conditCode) << 9) + (offset > 0 ? offset : (511 & offset));
	}
	else if (strcmp(getOpCode(BRNZP), opcode) == 0) {
		opCodeNum = 0;
		conditCode = 7;
		arg1Num = getRegAddress(arg1);
		if (arg1Num == -1) {
			printf("Invalid Argument!");
			exit(4);
		}
		else {
			offset = (arg1Num - currentLine) / 2;
		}
		return (((opCodeNum << 3) + conditCode) << 9) + (offset > 0 ? offset : (511 & offset));
	}
	else if (strcmp(getOpCode(HALT), opcode) == 0) {
		opCodeNum = 25;
		ans = toNum("x25");
		return (15 << 12) + ans;
	}
	else if (strcmp(getOpCode(JMP), opcode) == 0) {
		opCodeNum = 12;
		arg1Num = getRegister(arg1);
		return (opCodeNum << 12) + (arg1Num << 6);
	}
	else if (strcmp(getOpCode(JSR), opcode) == 0) {
		opCodeNum = 4;
		offset = (getRegAddress(arg1) - currentLine) / 2;
		return (((opCodeNum << 1) + 1) << 11) + (offset > 0 ? offset : (1024 + offset));
	}
	else if (strcmp(getOpCode(JSRR), opcode) == 0) {
		opCodeNum = 4;
		arg1Num = getRegister(arg1);
		return ((opCodeNum << 6) + arg1Num) << 6;
	}


	else if (strcmp(getOpCode(LDB), opcode) == 0) {
		opCodeNum = 2;
		arg1Num = getRegister(arg1);
		arg2Num = getRegister(arg2);
		if (arg3[0] == '#' || arg3[0] == 'x') {
			offset = toNum(arg3);
			if (offset > 31 || offset < -32) {
				printf("Invaid Argument!");
				exit(4);
			}
		}
		else {
			printf("Invaid Argument!");
			exit(4);
		}
		return (((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 6) + (offset > 0 ? offset : (64 + offset));
	}
	else if (strcmp(getOpCode(LDW), opcode) == 0) {
		opCodeNum = 6;
		arg1Num = getRegister(arg1);
		arg2Num = getRegister(arg2);
		arg3Num = toNum(arg3);
		offset = arg3Num;
		ans = (((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 6) + offset;
		return ans;
	}
	else if (strcmp(getOpCode(LEA), opcode) == 0) {
		opCodeNum = 14;
		arg1Num = getRegister(arg1);
		arg2Num = getRegAddress(arg2);
		offset = (arg2Num - currentLine) / 2;
		ans = (((opCodeNum << 3) + arg1Num) << 9) + offset;
		return ans;
	}
	else if (strcmp(getOpCode(NOP), opcode) == 0) {
		return 0;
	}
	else if (strcmp(getOpCode(NOT), opcode) == 0) {
		opCodeNum = 9;
		arg1Num = getRegister(arg1);
		arg2Num = getRegister(arg2);
		return (((((9 << 3) + arg1Num) << 3) + arg2Num) << 6) + 63;
	}
	else if (strcmp(getOpCode(RET), opcode) == 0) {
		opCodeNum = 12;
		arg1Num = 7;
		return (opCodeNum << 12) + (arg1Num << 6);
	}
	else if (strcmp(getOpCode(LSHF), opcode) == 0) {
		opCodeNum = 13;
		arg1Num = getRegister(arg1);
		arg2Num = getRegister(arg2);
		arg3Num = toNum(arg3);
		if (arg3Num < -8 || arg3Num > 15) {
			printf("Wrong Shift Amount!");
			exit(4);
		}
		return (((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 6) + arg3Num;
	}
	else if (strcmp(getOpCode(RSHFL), opcode) == 0) {
		opCodeNum = 13;
		arg1Num = getRegister(arg1);
		arg2Num = getRegister(arg2);
		arg3Num = toNum(arg3);
		if (arg3Num < -8 || arg3Num > 15) {
			printf("Wrong Shift Amount!");
			exit(4);
		}
		return (((((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 2) + 1) << 4) + arg3Num;
	}
	else if (strcmp(getOpCode(RSHFA), opcode) == 0) {
		opCodeNum = 13;
		arg1Num = getRegister(arg1);
		arg2Num = getRegister(arg2);
		arg3Num = toNum(arg3);
		if (arg3Num < -8 || arg3Num > 15) {
			printf("Wrong Shift Amount!");
			exit(4);
		}
		return (((((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 2) + 3) << 4) + arg3Num;
	}
	else if (strcmp(getOpCode(RTI), opcode) == 0) {
		opCodeNum = 8;
		return opCodeNum << 12;
	}

	//done
	else if (strcmp(getOpCode(STB), opcode) == 0) {
		opCodeNum = 3;
		arg1Num = getRegister(arg1);
		arg2Num = getRegister(arg2);
		if (arg3[0] == '#' || arg3[0] == 'x') {
			offset = toNum(arg3);
			if (offset < -32 || offset > 63) {
				printf("Invalid argument!");
				exit(4);
			}
		}
		else if (arg3[0] == 'r') {
			offset = (getRegAddress(arg3) - currentLine) / 2;
		}
		else {
			printf("Wrong Argument!");
		}
		return (((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 6) + offset;
	}

	else if (strcmp(getOpCode(STW), opcode) == 0) {
		opCodeNum = 7;
		arg1Num = getRegister(arg1);
		arg2Num = getRegister(arg2);
		if (arg3[0] == '#' || arg3[0] == 'x') {
			offset = toNum(arg3);
			if (offset < -32 || offset > 31) {
				printf("Invalid argument!");
				exit(4);
			}
		}
		else {
			printf("Wrong Argument!");
			exit(4);
		}
		return (((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 6) + offset;
	}
	else if (strcmp(getOpCode(XOR), opcode) == 0) {
		opCodeNum = 9;
		arg1Num = getRegister(arg1);
		arg2Num = getRegister(arg2);
		if (arg3[0] == '#' || arg3[0] == 'x') {
			arg3Num = toNum(arg3);
			if (arg3Num > 15 || arg3Num < -16) {
				printf("Invalid argument!");
				exit(4);
			}
			return (((((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 1) + 1) << 5) + arg3Num;
		}
		else if(getRegister(arg3)) {
			arg3Num = getRegister(arg3);
			return (((((opCodeNum << 3) + arg1Num) << 3) + arg2Num) << 6) + arg3Num;
		}
		else {
			printf("Invalid argument!");
			exit(4);
		}
	}
	else if (strcmp(getOpCode(TRAP), opcode) == 0) {
		opCodeNum = 15;
		if (arg1[0] == 'x') {
			arg2Num = toNum(arg1);
			return (opCodeNum << 12) + arg2Num;
		}
		else {
			printf("Wrong Argument!");
			exit(4);
		}
		return 0;
	}
	else if (strcmp(getOpCode(ORIG), opcode) == 0)
	{
		startAddress = toNum(arg1);
		return startAddress;
	}
	else if (strcmp(getOpCode(FILL), opcode) == 0) {
		return toNum(arg1) & 65535;
	}
	else if (strcmp(getOpCode(END), opcode) == 0) {
		return 0;
	}


	if (strlen(opcode) == 0){
		printf ("Invalid Argument!");
		exit(4);
	}

	printf("Invalid Argument!");
	exit(4);
}



int main(int argc, char* argv[]) {
	
	if (argc != 3) {
		printf("Wrong Input!");
		exit(4);
	}

	char *prgName = argv[0];
	char *iFileName = argv[1];
	char *oFileName = argv[2];

	char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1, *lArg2, *lArg3, *lArg4;
	int *digOpCode = 0;

	int lRet;

	FILE * lInfile;

	lInfile = fopen(iFileName, "r");	/* open the input file */

	int start = 0;

	do {
		lRet = readAndParseOne(lInfile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);
		if (lRet != DONE && lRet != EMPTY_LINE) {
			if (start == 0 && strcmp(lOpcode, ".orig")) {
				printf("Wrong .ORIG!");//need add test to the start address
				exit(4);
			}
			if (!strcmp(lOpcode, ".orig")) {
				start = 1;
				currentLine = startAddress;
				continue;
			}
			currentLine += 2;
		}
	} while (lRet != DONE);

	FILE * pOutfile;
	pOutfile = fopen(oFileName, "w");
	rewind(lInfile);
	int lInstr = 0;
	start = 0;
	currentLine = startAddress;
	do {
		lRet = readAndParseTwo(lInfile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);
		if (getOpcode(lArg1) != -1 || getOpcode(lArg2) != -1 || getOpcode(lArg3) != -1 || getOpcode(lArg4) != -1) {
			printf("Invalid Argument!");
			exit(4);
		}
		if (lRet != DONE && lRet != EMPTY_LINE) {
			if (start == 0 && !strcmp(lOpcode, ".orig")) {
				int temp = toNum(lArg1);
				if (temp < 0 || temp > 65535 || temp % 2) {
					printf("Wrong .ORIG Address!");
					exit(4);
				}
			}
			printf("");
			lInstr = compileLine(lLine, lLabel, lOpcode, lArg1, lArg2, lArg3, lArg4);
			printf("0x%.4X\n", lInstr);
			fprintf(pOutfile, "0x%.4X\n", lInstr); /* where lInstr is declared as an int */
			currentLine += 2;
		}

	} while (lRet != DONE);

	fclose(lInfile);
	fclose(pOutfile);
}
