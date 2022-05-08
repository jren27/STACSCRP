// This is a prototype of the assembler
// It assumes that everything is formatted in a very specific way
#include <stdio.h>
#include <stdlib.h> // do I need this?
#include <string.h>
#include <strings.h> // strcasecmp
#include <stdbool.h>
#include <ctype.h>
#include "src/instruction.h"

void readToWhitespace(char* buff, int n, FILE* file) {
	int copyIndex = 0;
	bool beginCopy = false;
	int temp = 0;
	while (copyIndex < n) {
		temp = fgetc(file);
		if (temp == EOF) {
			return;
		}
		// This could look a bit nicer
		if (temp == ' ' || temp == '\t' || temp == '\n') {
			if (beginCopy) {
				buff[copyIndex] = '\0';
				printf("%s ", buff); // TODO DELETE THIS WHEN FULL ASSEMBLER COMPLETE
				return;
			}
		} else {
			if (!beginCopy) {
				beginCopy = true;
			}
			buff[copyIndex] = temp;
			copyIndex++;
		}
	}
	buff[copyIndex] = '\0';
}

// This assumes that list is empty
void storeLines(linelist* list, FILE* in) {
	list->head = NULL;
	list->tail = NULL;
	char buffer[80];
	while (!feof(in)) {
		fgets(buffer, 80, in);
		pushLine(list, buffer);
	}
}

void parseFile(FILE* in, FILE* out, instruction nextInstruction) {
	char buffer[50] = "";
	unsigned short stack, op, argtype = NULL_TP;
	unsigned long arg;
	printf("\n------\n");
	// Read stack
	readToWhitespace(buffer, 50, in);
	stack = buffer[0] - '0';

	// Read instruction
	readToWhitespace(buffer, 50, in);
	if (!strcasecmp(buffer, "EXEC")) {
		op = EXEC_OP;
		// --- STACK OPERATIONS ---
	} else if (!strcasecmp(buffer, "POP")) {
		op = POP_OP;
	} else if (!strcasecmp(buffer, "PUSH")) {
		op = PUSH_OP;
		readToWhitespace(buffer, 50, in);
		if (!strcasecmp(buffer, "#")) {
			op |= INT_TP;
			fscanf(in, "%d", arg);
		} else if (!strcasecmp(buffer, "\'")) {
			op |= CHAR_TP;
			fscanf(in, "%c", &arg);
		} else if (!strcasecmp(buffer, ".")) {
			op |= DOUB_TP;
			fscanf(in, "%lf", &arg);
		}
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
	} else if (!strcasecmp(buffer, "RTYP")) {
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
	}
}

// --- PASS 1 ---
// Remove comments, whitespace

// Returns true if line is only whitespace
bool _isWhitespace(char* line) {
	for (int i = 0; i < strlen(line); i++) {
		if (!isspace(line[i])) {
			return false;
		}
	}
	return true;
}

// Truncates line to remove any trailing whitespaces and comments
void _removeComment(char* line) {
	char* end = &line[0];
	for (int i = 0; i < strlen(line); i++) {
		if (line[i] == ';') {
			break;
		}
		if (!isspace(line[i])) {
			end = &line[i+1];
		}
	}
	*end = '\0';
}

void PassOne(linelist* list) {
	line* currline = list->head;
	while (currline != NULL) {
		// Including second line because commenting out stuff is pretty common
		if (_isWhitespace(currline->buff) || currline->buff[0] == ';') {
			removeLine(list, currline);
		} else {
			_removeComment(currline->buff);
		}
		currline = currline->next;
	}
	// There's an extra last line here, get rid of it!
	if (list->tail != NULL) {
		list->tail = list->tail->prev;
		free(list->tail->next);
		list->tail->next = NULL;
	}
}

// --- PASS 2 ---
// Parse everything into instructions

// Returns 0 if no errors, 1 if error occurs
// Assumes instlist is empty
int PassTwo(linelist* list, instructionlist* instlist) {
	line* currline = list->head;
	while (currline != NULL) {
		
	}
	return 0;
}

int main() { // TODO use command line arguments
	FILE* in = fopen("test.stacscrp", "r");
	//FILE* out = fopen("out.bin", "w+");
	linelist list;
	storeLines(&list, in);
	line* currline = list.head;
	while (currline != NULL) {
		printf("%s", currline->buff);
		currline = currline->next;
	}
	PassOne(&list);
	printf("--- PASS ONE ---\n");
	currline = list.head;
	while (currline != NULL) {
		printf("%s\n", currline->buff);
		currline = currline->next;
	}
	//fputc(-1, out);
	fclose(in);
	//fclose(out);
	return 0;
}
