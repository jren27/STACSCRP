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
#include "src/map.h"

// --- PREPASS ---
// Store program in line struct for further processing

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

// This assumes that list is empty
int prePass(linelist* list, FILE* in) {
	list->head = NULL;
	list->tail = NULL;
	unsigned int lineNum = 1;
	nextLine(in); // Prep in case file starts with empty space
	while (!feof(in)) {
		printf("line %d: ", lineNum);
		line* templine = malloc(sizeof(line));
		for (int i = 0; i < 5; i++) {
		templine->unparsed[i] = malloc(sizeof(char) * 20);
			if (readNextSection(templine->unparsed[i], 20, in)) {
				return 1;
			}
			if (!strcmp(templine->unparsed[i], "\n")) {
				printf("END\n");
				break;
			}
			printf("%s ", templine->unparsed[i]);
			// Only happens if newline is never reached
			if (i == 5) {
				printf("ASSEMBLY ERROR: More arguments than expected found\n");
				return 1;
			}
		}
		nextLine(in);
		templine->lineNum = lineNum++;
		pushLine(list, templine, false);
	}
	return 0;
}


/* IMPLEMENT THIS IN LATER PASSES
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
	// Handle instruction position
	inst->pos = (list->head == NULL) ? 1 : list->tail->pos + 1;
	pushInstruction(list, inst);
	nextLine(in);
	return 0;
} */

// --- PASS 1 ---
// - Handle DEFN replacements
// - Remove empty lines
// - Remove excess EXEC statements

int pass1(linelist* list) {
	line* currline = list->head;
	treenode* defntree = NULL; // Use to replace any values
	char defnbuff[50];
	bool firstexec;
	while (currline != NULL) {
		if (!strcasecmp(currline->unparsed[0], "DEFN")) {
			char keyword[50] = "@";
			strncat(keyword, currline->unparsed[1], 50);
			defntree = insertNode(defntree, keyword, currline->unparsed[2]);
			currline = removeLine(list, currline);
		}
		if (currline->unparsed[0][0] == '\n') {
			currline = removeLine(list, currline);
		}
		if (!strcasecmp(currline->unparsed[0], "EXEC")) {
			if (firstexec) {
				currline = removeLine(list, currline);
			} else {
				firstexec = true;
			}
		}
	
		// Replace using DEFN tree
		for (int i = 0; i < 4; i++) {
			if (currline->unparsed[i][0] == '\n') {
				break;
			}
			if (!getNode(defntree, currline->unparsed[i], defnbuff)) {
				strcpy(currline->unparsed[i], defnbuff);
			}
		}

		currline = currline->next;

	}
	if (!firstexec) {
		printf("NOTE: EXEC instruction not found, automatically adding to beginning of program\n");
		currline = malloc(sizeof(line));
		currline->unparsed[0] = malloc(sizeof(char) * 10);
		currline->unparsed[1] = malloc(sizeof(char) * 10);
		strcpy(currline->unparsed[0], "EXEC");
		strcpy(currline->unparsed[1], "\n");
		pushLine(list, currline, true);
	}
	return 0;
}

// --- PASSES 2-4 ---
// Pass 2: Store all MARK locations, delete from linelist
// Pass 3: Parse all lines to instructions
// Pass 4: Rework all JUMPS and GOTOS to use instruction positions instead of MARKS

int main() { // TODO use command line arguments
	FILE* in = fopen("test.stacscrp", "rb");
	//FILE* out = fopen("out.bin", "w+");
	//fputc(-1, out);
	
	linelist list;
	if (prePass(&list, in)) {
		return 1;
	}
	pass1(&list);
	
	// Test
	printf("\nPASS 1:\n");
	line* currline = list.head;
	while (currline != NULL) {
		for (int i = 0; i < 4; i++) {
			if (currline->unparsed[i][0] == '\n') {
				printf("END\n");
				break;
			}
			printf("%s ", currline->unparsed[i]);
		}
		currline = currline->next;
	}

	// Clean up
	fclose(in);
	//fclose(out);
	//freeInstructionList(&list);
	return 0;
}
