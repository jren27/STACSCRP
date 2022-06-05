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
		for (int i = 0; i < 4; i++) {
			templine->unparsed[i] = malloc(sizeof(char) * 20);
		}
		for (int i = 0; i < 5; i++) {
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

// --- PASS 1 ---
// - Handle DEFN replacements
// - Remove empty lines
// - Remove excess EXEC statements

int pass1(linelist* list) {
	line* currline = list->head;
	treenode* defntree = NULL; // Use to replace any values
	char defnbuff[50];
	bool firstexec = false;
	while (currline != NULL) {
		if (!strcasecmp(currline->unparsed[0], "DEFN")) {
			char keyword[50] = "@";
			strncat(keyword, currline->unparsed[1], 49);
			defntree = insertNode(defntree, keyword, currline->unparsed[2]);
			currline = removeLine(list, currline);
			continue;
		}
		if (currline->unparsed[0][0] == '\n') {
			currline = removeLine(list, currline);
			continue;
		}
		if (!strcasecmp(currline->unparsed[0], "EXEC")) {
			if (firstexec) {
				currline = removeLine(list, currline);
				continue;
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
	freeMap(defntree);
	return 0;
}

// --- PASS 2 ---
// - Store MARK locations

bool isDigitString(char* buff) {
	while (buff != NULL) {
		if (!isdigit(*buff)) {
			return false;
		}
		buff++;
	}
	return true;
}

// atoi() and strtol() don't really handle errors well...
bool stringtoint(char* buff, int* value) {
	unsigned int index = 0;
	char tempchar = buff[index++];
	*value = 0;
	int tempint;
	while (tempchar != '\0') {
		if (!isdigit(tempchar)) {
			return false;
		}
		tempint = tempchar - '0';
		*value *= 10;
		*value += tempint;
		tempchar = buff[index++];
	}
	return true;
}


bool stringtodouble(char* buff, double* value) {
	unsigned int index = 0;
	char tempchar = buff[index++];
	*value = 0;
	double tempdouble;
	while (tempchar != '\0') {
		if (tempchar == '.') {
			break;
		} else if (!isdigit(tempchar)) {
			return false;
		}
		tempdouble = tempchar - '0';
		*value *= 10;
		*value += tempdouble;
		tempchar = buff[index++];
	}
	if (tempchar == '\0') {
		return true;
	}
	// Decimal handling
	unsigned int decimalpos = 1;
	tempchar = buff[index++];
	while (tempchar != '\0') {	
		if (!isdigit(tempchar)) {
			return false;
		}
		tempdouble = tempchar - '0';
		tempdouble /= (10 ^ decimalpos);
		*value += tempdouble;
		tempchar = buff[index++];
	}
	return true;
}

// Returns 1 if error, 0 if not
int parseLine(line* line, instructionlist* list) {
	disk* tempdisk = malloc(sizeof(disk));
	tempdisk->isInstruction = true;

	// Edge case: EXEC
	if (!strcasecmp(line->unparsed[0], "EXEC")) {
		tempdisk->instruction = EXEC_OP;
		// Would a goto make more sense?
		instruction* inst = malloc(sizeof(instruction));
		inst->d = tempdisk;
		if (list->head == NULL) {
			inst->pos = 0;
		} else {
			inst->pos = list->tail->pos + 1;
		}
		pushInstruction(list, inst);
		return 0;
	}

	unsigned short unparsedpos = 0;
	bool hasArgument = false;
	int stack = 0;
	char markkey[50];
	markkey[0] = '\0'; // in case
	if (stringtoint(line->unparsed[unparsedpos], &stack)) {
		unparsedpos++;
	} else {
		if (list->tail != NULL && list->tail->d->instruction != EXEC_OP) {
			stack = (list->tail->d->instruction >> 8);
		}
	}
	if (stack < 0 || stack > 7) {
		printf("ASSEMBLY ERROR: Stack number undefined\n");
		return 1;
	}
	
	// Instruction handling
	if (!strcasecmp(line->unparsed[unparsedpos], "POP")) {
		tempdisk->instruction = POP_OP;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "TOP")) {
		tempdisk->instruction = TOP_OP;	
	} else if (!strcasecmp(line->unparsed[unparsedpos], "PUSH")) {
		tempdisk->instruction = PUSH_OP;
		hasArgument = true;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "SWAP")) {
		tempdisk->instruction = SWAP_OP;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "DROP")) {
		tempdisk->instruction = DROP_OP;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "ADD")) {
		tempdisk->instruction = ARTH_OP | ADD_AL;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "SUB")) {
		tempdisk->instruction = ARTH_OP | SUB_AL; 
	} else if (!strcasecmp(line->unparsed[unparsedpos], "MUL")) {
		tempdisk->instruction = ARTH_OP | MUL_AL;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "DIV")) {
		tempdisk->instruction = ARTH_OP | DIV_AL;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "AND")) {
		tempdisk->instruction = ARTH_OP | AND_AL;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "LOR")) {
		tempdisk->instruction = ARTH_OP | LOR_AL;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "NOT")) {
		tempdisk->instruction = ARTH_OP | NOT_AL;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "XOR")) {
		tempdisk->instruction = ARTH_OP | XOR_AL;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "CMEQ")) {
		tempdisk->instruction = COMP_OP | EQ;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "CMNE")) {
		tempdisk->instruction = COMP_OP | NE;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "CMGT")) {
		tempdisk->instruction = COMP_OP | GT;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "CMLE")) {
		tempdisk->instruction = COMP_OP | LE;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "CMLT")) {
		tempdisk->instruction = COMP_OP | LT;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "CMGE")) {
		tempdisk->instruction = COMP_OP | GE;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "TYPE")) {
		tempdisk->instruction = TYPE_OP;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "RTYP")) {
		tempdisk->instruction = RTYP_OP;
		hasArgument = true;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "IPUT")) {
		tempdisk->instruction = IPUT_OP;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "OPUT")) {
		tempdisk->instruction = OPUT_OP;
	} else if (!strcasecmp(line->unparsed[unparsedpos], "GOTO")) {
		tempdisk->instruction = GOTO_OP;
		if (line->unparsed[unparsedpos++ + 1][0] == '\n') {
			printf("ASSEMBLY ERROR: Expected MARK not found");
			return 1;
		}
		strcpy(markkey, line->unparsed[unparsedpos]);
	} else if (!strcasecmp(line->unparsed[unparsedpos], "GOIF")) {
		tempdisk->instruction = GOTO_OP | COND_BR;
		if (line->unparsed[unparsedpos++ + 1][0] == '\n') {
			printf("ASSEMBLY ERROR: Expected MARK not found");
			return 1;
		}
		strcpy(markkey, line->unparsed[unparsedpos]);
	} else if (!strcasecmp(line->unparsed[unparsedpos], "JUMP")) {
		tempdisk->instruction = JUMP_OP;
		if (line->unparsed[unparsedpos++ + 1][0] == '\n') {
			printf("ASSEMBLY ERROR: Expected MARK not found");
			return 1;
		}
		strcpy(markkey, line->unparsed[unparsedpos]);
	} else if (!strcasecmp(line->unparsed[unparsedpos], "JPIF")) {
		tempdisk->instruction = JUMP_OP | COND_BR;
		if (line->unparsed[unparsedpos++ + 1][0] == '\n') {
			printf("ASSEMBLY ERROR: Expected MARK not found");
			return 1;
		}
		strcpy(markkey, line->unparsed[unparsedpos]);
	} else if (!strcasecmp(line->unparsed[unparsedpos], "EXEC")) {
		tempdisk->instruction = EXEC_OP;
	} else {
		printf("ASSEMBLY ERROR: Unknown instruction\n");
		free(tempdisk);
		return 1;
	}

	tempdisk->instruction |= (stack << 8);

	unparsedpos++;
	if (hasArgument) {
		if (line->unparsed[unparsedpos][0] == '\n') {
			printf("ASSEMBLY ERROR: Expected argument not found\n");
			return 1;
		}
		char* bufferpos = line->unparsed[unparsedpos];
		switch (*(bufferpos++)) {
			case '=':
				tempdisk->type = BOOL_TP;
				if (strlen(bufferpos) != 1) {
					printf("ASSEMBLY ERROR: Invalid boolean argument\n");
					return 1;
				}
				if (bufferpos[0] == '0' || toupper(bufferpos[0]) == 'F') {
					tempdisk->boolvalue = false;
				} else if (bufferpos[0] == '1' || toupper(bufferpos[0]) == 'T') {
					tempdisk->boolvalue = true;
				} else {
					printf("ASSEMBLY ERROR: Invalid boolean argument\n");
					return 1;
				}
				break;
			case '#':
				tempdisk->type = INT_TP;
				if (!stringtoint(bufferpos, &(tempdisk->intvalue))) {
					printf("ASSEMBLY ERROR: Invalid integer argument\n");
					return 1;
				}
				break;
			case '\'':
				tempdisk->type = CHAR_TP;
				if (strlen(bufferpos) != 1) {
					printf("ASSEMBLY ERROR: Invalid character argument\n");
					return 1;
				}
				tempdisk->charvalue = *bufferpos;
				break;
			case '.':
				if (!stringtodouble(bufferpos, &(tempdisk->doublevalue))) {
					printf("ASSEMBLY ERROR: Invalid double argument\n");
					return 1;
				}
				tempdisk->type = DOUB_TP;
				break;
			default:
				printf("ASSEMBLY ERROR: Invalid argument type\n");
				return 1;
		}
	} else {
		if (line->unparsed[unparsedpos][0] != '\n') {
			printf("ASSEMBLY ERROR: Unexpected argument found\n");
			return 1;
		}
		tempdisk->type = NULL_TP;
	}

	// Now put it all together
	instruction* inst = malloc(sizeof(instruction));
	inst->d = tempdisk;
	strncpy(inst->markkey, markkey, 50);
	if (list->head == NULL) {
		inst->pos = 0;
	} else {
		inst->pos = list->tail->pos + 1;
	}
	pushInstruction(list, inst);
	return 0;
}

int pass2(instructionlist* ilist, linelist* llist, treenode** markmap) {
	ilist->head = ilist->tail = NULL;
	line* l = llist->head;
	while (l != NULL) {
		// Store marks
		if (!strcasecmp(l->unparsed[0], "MARK")) {
			char tempvalue[50];
			if (ilist->tail != NULL) {
				sprintf(tempvalue, "%d", ilist->tail->pos + 1);
			} else {
				strcpy(tempvalue, "0");
			}
			*markmap = insertNode(*markmap, l->unparsed[1], tempvalue);
			l = l->next;
			continue;
		}
		if (parseLine(l, ilist)) {
			return 1;
		}
		l = l->next;
	}
	// TODO check if MARK is at tail and adjust
	return 0;
}

// Pass 3: Rework all JUMPS and GOTOS to use instruction positions instead of MARKS

int pass3(instructionlist* ilist, treenode* marktree) {
	instruction* inst = ilist->head;
	char temp[50];
	while (inst != NULL) {
		if ((inst->d->instruction & 0x00FF) == JUMP_OP || (inst->d->instruction & 0x00FF) == GOTO_OP) {
			if (getNode(marktree, inst->markkey, temp)) {
				printf("ASSEMBLY ERROR: Undefined MARK\n");
				return 1;
			}
			if (!stringtoint(temp, &inst->d->intvalue)) {
				printf("ASSEMBLY ERROR: MARK is not an integer\n");
				return 1;
			}
			if (inst->d->intvalue < 0 || inst->d->intvalue >= ilist->tail->pos) {
				printf("ASSEMBLY ERROR: MARK is out of range of valid lines\n");
				return 1;
			}
		}
		inst = inst->next;
	}
	return 0;
}

void writeinstructionlist(FILE* out, instructionlist* ilist) {
	// Remember to write BACKWARDS!
	instruction* inst = ilist->tail;
	while (inst != NULL) {
		fwrite(&(inst->d->instruction), sizeof(short), 1, out);
		fwrite(&(inst->d->type), sizeof(char), 1, out);
		switch (inst->d->type) {
			case BOOL_TP:
				fwrite(&(inst->d->boolvalue), sizeof(bool), 1, out);
				break;
			case CHAR_TP:
				fwrite(&(inst->d->charvalue), sizeof(char), 1, out);
				break;
			case INT_TP:
				fwrite(&(inst->d->intvalue), sizeof(int), 1, out);
				break;
			case DOUB_TP:
				fwrite(&(inst->d->doublevalue), sizeof(double), 1, out);
				break;
		}
		inst = inst->prev;
	}
}

int main() { // TODO use command line arguments
	FILE* in = fopen("test.stacscrp", "rb");
	FILE* out = fopen("out.bin", "w+");
	//fputc(-1, out);
	
	linelist llist;
	if (prePass(&llist, in)) {
		return 1;
	}

	if (pass1(&llist)) {
		return 1;
	}
	
	// Test
	printf("\nPASS 1:\n");
	line* currline = llist.head;
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
	instructionlist ilist;
	treenode* marktree = NULL;
	if (pass2(&ilist, &llist, &marktree)) {
		return 1;
	}

	printf("\nPASS 2:\n");
	instruction* currinst = ilist.head;
	while (currinst != NULL) {
		printf("%x\n", currinst->d->instruction);
		currinst = currinst->next;
	}
	
	if (pass3(&ilist, marktree)) {
		return 1;
	}
	
	writeinstructionlist(out, &ilist);

	// Clean up
	fclose(in);
	fclose(out);
	freeLineList(&llist);
	freeInstructionList(&ilist);
	freeMap(marktree);
	return 0;
}
