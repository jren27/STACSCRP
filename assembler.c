// This is a prototype of the assembler
// It assumes that everything is formatted in a very specific way
#include <stdio.h>
#include <stdlib.h> // do I need this?
#include <string.h>
#include <strings.h> // strcasecmp
#include <stdbool.h>
#include <ctype.h>
#include "src/diskstack.h"
#include "src/instruction.h"


// --- PASS 1 ---
// Remove comments, whitespace, and converts everything to instructions

// Copies next instruction section (ignoring whitespace) to buffer
// Newline is considered a section, indicates no other sections in line
// Also ignores comments
// Returns if EOF is reached
int readNextSection(char* buff, int n, FILE* file) {
	int copyIndex = 0;
	int temp = 0;
	bool isComment = false;
	while (copyIndex < n) {
		temp = fgetc(file);
		if (temp == EOF) {
			return 1;
		}
		if (temp == '\n') { // Handle newline
			if (copyIndex == 0) {
				buff[0] = '\n';
				buff[1] = '\0';
			} else {
				buff[copyIndex] = '\0';
				fseek(file, -1L, SEEK_CUR);
			}
			return 0;
		}
		if (temp == ' ' || temp == '\t') {
			break;
		}
		if (temp == ';') {
			buff[copyIndex++] = '\n';
			isComment = true;
			break;
		}
		buff[copyIndex++] = temp;
	}
	buff[copyIndex] = '\0';
	// Dump all whitespace before next section or newline
	while (temp != '\n') {	
		temp = fgetc(file);
		if (temp == EOF) {
			return 1;
		}
		if (!isComment && temp != ' ' && temp != '\t') {
			break;
		}
	}
	fseek(file, -1L, SEEK_CUR); // Rewind by 1, next char to be read will be the start of the next section
	return 0;
}

// This doesn't exactly go to the next line lol, more like the next instruction on the next line
int nextLine(FILE* file) {
	int temp = ' ';
	while (isspace(temp)) {
		temp = fgetc(file);
		if (temp == EOF) {
			return 1;
		}
	}
	fseek(file, -1L, SEEK_CUR);
	return 0;
}

// Returns 1 if parsing ever stops (EOF or error), 0 if not
int parseLine(FILE* in, instructionlist* list) {
	char buffer[50] = "";
	unsigned short stack, op;
	instruction* inst = malloc(sizeof(instruction));
	inst->d.type = NULL_TP;
	// Read first line
	if (readNextSection(buffer, 50, in)) {
		return 1;
	}

	if (buffer[0] == ';' || buffer[0] == '\n') { // Check if line is commented or whitespace
		return 0;
	}
	if (strlen(buffer) == 1) {
		stack = buffer[0] - '0';
		if (stack < 0 || stack > 7) {
			printf("ASSEMBLY ERROR: Invalid stack\n");
			return 1;
		}	
		if (readNextSection(buffer, 50, in)) {
			printf("ASSEMBLY ERROR: Early EOF reached\n");
			return 1;
		}
	} else { // Handle stack "carryover"
		if (list->tail != NULL) {
			stack = list->tail->d.instruction >> 8; // Is this the right way to do this?
		} else { // Default is 0
			stack = 0;
		}
	}
	
	// Read instruction
	
	if (buffer[0] == '\n') {
		printf("ASSEMBLY ERROR: Instruction not found\n");
	}

	if (!strcasecmp(buffer, "EXEC")) {
		op = EXEC_OP;
		// --- STACK OPERATIONS ---
	} else if (!strcasecmp(buffer, "POP")) {
		op = POP_OP;
	} else if (!strcasecmp(buffer, "PUSH")) {
		op = PUSH_OP;
		if (readNextSection(buffer, 50, in)) {
			printf("ASSEMBLY ERROR: Early EOF reached\n");
			return 1;
		}
		if (buffer[0] == '\n') {
			printf("ASSEMBLY ERROR: Argument not given\n");
		}
		if (!strcasecmp(buffer, "#")) {
			inst->d.type = INT_TP;
			fscanf(in, "%d", &inst->d.intvalue);
		} else if (!strcasecmp(buffer, "\'")) {
			inst->d.type = CHAR_TP;
			fscanf(in, "%c", &inst->d.intvalue);
		} else if (!strcasecmp(buffer, ".")) {
			inst->d.type = DOUB_TP;
			fscanf(in, "%lf", &inst->d.doublevalue);
		} else {
			printf("ASSEMBLY ERROR: Invalid argument type\n");
		}
		op |= inst->d.type;
	} else if (!strcasecmp(buffer, "SWAP")) {
		op = SWAP_OP;
	} else if (!strcasecmp(buffer, "DROP")) {
		op = DROP_OP;
	} else if (!strcasecmp(buffer, "ADD")) {
		op = ARTH_OP | ADD_AL;
	} else if (!strcasecmp(buffer, "SUB")) {
		op = ARTH_OP | SUB_AL;
	} else if (!strcasecmp(buffer, "MUL")) {
		op = ARTH_OP | MUL_AL;
	} else if (!strcasecmp(buffer, "DIV")) {
		op = ARTH_OP | DIV_AL;
	} else if (!strcasecmp(buffer, "AND")) {
		op = ARTH_OP | AND_AL;
	} else if (!strcasecmp(buffer, "LOR")) {
		op = ARTH_OP | LOR_AL;
	} else if (!strcasecmp(buffer, "NOT")) {
		op = ARTH_OP | NOT_AL;
	} else if (!strcasecmp(buffer, "XOR")) {
		op = ARTH_OP | XOR_AL;
		// --- COMPARATOR OPERATIONS ---
	} else if (!strcasecmp(buffer, "CMEQ")) {
		op = COMP_OP | EQ;
	} else if (!strcasecmp(buffer, "CMNE")) {
		op = COMP_OP | NE;
	} else if (!strcasecmp(buffer, "CMGT")) {
		op = COMP_OP | GT;
	} else if (!strcasecmp(buffer, "CMLE")) {
		op = COMP_OP | LE;
	} else if (!strcasecmp(buffer, "CMLT")) {
		op = COMP_OP | LT;
	} else if (!strcasecmp(buffer, "CMGE")) {
		op = COMP_OP | GE;	
	} else if (!strcasecmp(buffer, "LTRL")) {
		op = LTRL_OP;
	} else if (!strcasecmp(buffer, "TYPE")) {
		op = TYPE_OP;
	} else if (!strcasecmp(buffer, "RTYP")) { // TODO add argument handling for this later
		op = RTYP_OP;
	} else if (!strcasecmp(buffer, "IPUT")) {
		op = IPUT_OP;
	} else if (!strcasecmp(buffer, "OPUT")) {
		op = OPUT_OP;	
	} else if (!strcasecmp(buffer, "GOTO")) {
		op = GOTO_OP;
	} else if (!strcasecmp(buffer, "GOIF")) {
		op = GOTO_OP | COND_BR;
	} else if (!strcasecmp(buffer, "JUMP")) {
		op = JUMP_OP;
	} else if (!strcasecmp(buffer, "JPIF")) {
		op = JUMP_OP | COND_BR;
	} else {
		printf("ASSEMBLY ERROR: Invalid instruction\n");
		return 1;
	}
	inst->d.instruction = stack << 8 | op;
	pushInstruction(list, inst);
	nextLine(in);
	return 0;
}

// This assumes that list is empty
int passOne(instructionlist* list, FILE* in) {
	list->head = NULL;
	list->tail = NULL;
	while (!feof(in)) {
		if (parseLine(in, list)) {
			return 1;
		}
	}
	return 0;
}

int main() { // TODO use command line arguments
	FILE* in = fopen("test.stacscrp", "rb");
	//FILE* out = fopen("out.bin", "w+");
	//fputc(-1, out);
	
	instructionlist list;
	passOne(&list, in);
	
	// Testing
	instruction* currinst = list.head;
	while (currinst != NULL) {
		printf("%d\n", currinst->d.instruction >> 8);
		currinst = currinst->next;
	}

	// Clean up
	fclose(in);
	//fclose(out);
	freeInstructionList(&list);
	return 0;
}
