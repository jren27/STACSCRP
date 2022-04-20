#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "diskstack.h"
#include "instruction.h"

// Returns an array of instructions
instruction* loadAssembly(FILE* f, unsigned int* lineNumMax) {
	//TODO write this
	unsigned short stack = 0;
	unsigned short opcode = 0;
	unsigned short type = 0;
	// instruction array stuff
	instruction* instructions = (instruction*)malloc(sizeof(instruction) * 4);
	*lineNumMax = 0;
	unsigned int capacity = 4;
	while (!feof(f)) {
		if (*lineNumMax == capacity) {
			capacity *= 2;
			instruction* newinstructions = (instruction*)realloc(instructions, sizeof(instruction) * capacity);
			instructions = newinstructions;
		}
		fscanf(f, "%hu %hu %hu", &stack, &opcode, &type);
		instructions[*lineNumMax].stack = stack;
		instructions[*lineNumMax].op = toOperation(opcode);
		instructions[*lineNumMax].argtype = convertToType(type);
		switch (type) {
			case 0:
				break;
			case 1:
			case 2:
			case 3:
				fscanf(f, "%d", &instructions[*lineNumMax].argint);
				instructions[*lineNumMax].argdouble = 0;
				break;
			case 4:
				fscanf(f, "%lf", &instructions[*lineNumMax].argdouble);
				instructions[*lineNumMax].argint = 0;
				break;
		}
		(*lineNumMax)++;
	}
	return instructions;
}

int execute(instruction* instructions, unsigned int* lineNum, unsigned int* lineNumMax, stack* stacks, disk* grabbedDisk) {
	switch (instructions[*lineNum].op) {
		case POP:
			if (top(&stacks[instructions[*lineNum].stack], grabbedDisk)) {
				printf("RUNTIME ERROR on line %ls: Attempted to POP stack %d, but has size 0\n", lineNum, instructions[*lineNum].stack);
				return 1;
			}
			pop(&stacks[instructions[*lineNum].stack]);
			break;
		case TOP:
			if (top(&stacks[instructions[*lineNum].stack], grabbedDisk)) {
				printf("RUNTIME ERROR on line %ls: Attempted to TOP stack %d, but has size 0\n", lineNum, instructions[*lineNum].stack);
				return 1;
			}
			break;
		case PUSH:
			push(&stacks[instructions[*lineNum].stack], grabbedDisk);
			break;
		case SWAP:

		default:
			break;
	}
	return 0;
}

int main() {
	// TODO update to take command line input
	char* filename = "test.stacasmb";
	FILE* f = fopen(filename, "r");
	if (f == NULL) {
		printf("ERROR: file not found");
		return 1;
	}
	unsigned int lineNumMax;
	instruction* instructions = loadAssembly(f, &lineNumMax);
	fclose(f);
	return 0;
	// Actually run code here
	stack stacks[8];
	disk* grabbedDisk = NULL;
	unsigned int lineNum = 0;
	while (lineNum <= lineNumMax) {
		if (execute(instructions, &lineNum, &lineNumMax, stacks, grabbedDisk) != 0) {
			break;
		}
	}
	return 0;
}
