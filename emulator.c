#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "src/diskstack.h"
#include "src/instruction.h"

int prepareProgramBook(FILE* f, stack* stacks) {
	// Read file to PB2
	disk* d;
	while (!feof(f)) {
		d = malloc(sizeof(disk));
		loadInstruction(d, f);
		push(&stacks[6], *d);
	}

	// Flip through PB2 until EXEC reached
	if (top(&stacks[6], d)) {
		printf("RUNTIME ERROR: Read file contains no instructions\n");
		return 1;
	}
	while (d->instruction != EXEC_OP) {
		pop(&stacks[6]);
		push(&stacks[5], *d);
		if (top(&stacks[6], d)) {
			printf("RUNTIME ERROR: EXEC instruction not found\n");
			return 1;
		}
	}
	//pop(&stacks[6]);
	initDisk(d);
	d->type = INT_TP;
	d->intvalue = stacks[5].size;
	return 0;
}

int seekProgramBook(stack* stacks, int i) {
	if (i < 0) {
		printf("RUNTIME ERROR: Attemped to seek to a negative-indexed instruction\n");
		return 1;
	}
	disk* temp = NULL;
	while (i != stacks[5].size) {

		// Flip PB1 -> PB2
		if (i < stacks[5].size) {
		 	top(&stacks[5], temp);
			pop(&stacks[5]);
			push(&stacks[6], *temp);
		}
		// Flip PB2 -> PB1
		if (top(&stacks[6], temp)) {
			printf("RUNTIME ERROR: Instruction seek went out of range");
			return 1;
		}
		pop(&stacks[6]);
		push(&stacks[5], *temp);
	}
	return 0;
}

// Arg 2 stays on top, update it with result
int get2Args(stack* stacks, int currstack, disk** arg1, disk** arg2) {
	if (top(&stacks[currstack], *arg1)) {
		printf("RUNTIME ERROR: Stack is empty\n");
		return 1;
	}
	pop(&stacks[currstack]);
	if (top(&stacks[currstack], *arg2)) {
		printf("RUNTIME ERROR: Stack only contains one argument\n");
		return 1;
	}
	return 0;
}

int execute(stack* stacks, disk* diskregister) {
	disk* currdisk = NULL;
	if (top(&stacks[6], currdisk)) {
		printf("RUNTIME ERROR: Reached EOF");
		return 1;
	}
	if (currdisk->instruction == EXEC_OP) {
		pop(&stacks[6]);
		return 0; // No need to update system stack
	}
	short currstack = currdisk->instruction >> 8;
	short currinst = currdisk->instruction | 0x00FF;
	bool increment = true;

	// For arth/comp stuff
	disk* arg1, *arg2;
	switch (currinst) { //TODO this bigass thing
		case POP_OP:
			if (pop(&stacks[currstack])) {
				printf("RUNTIME ERROR: Stack is empty\n");
				return 1;
			}
			break;
		case TOP_OP:
			if (top(&stacks[currstack], diskregister)) {
				printf("RUNTIME ERROR: Stack is empty\n");
				return 1;
			}
			break;
		case PUSH_OP:
			// TODO figure this out
			break;
		case SWAP_OP:
			if (swap(&stacks[currstack])) {
				printf("RUNTIME ERROR: STACK is empty\n");
				return 1;
			}
			break;
		case DROP_OP:
			push(&stacks[currstack], *diskregister);
			break;
		case ARTH_OP | ADD_AL: // TODO test this, implement all the other ARTH and COMP instructions later
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			// oh boy
			if (arg1->type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to add two disks with non-matching types\n");
				return 1;
			}
			switch (arg1->type) {
				case INT_TP:
				case CHAR_TP:
					arg2->intvalue += arg1->intvalue;
					break;
				case DOUB_TP:
					arg2->doublevalue += arg1->doublevalue;
			}
			break;
	}

	// Update system stack, seek to next instruction
	
	return 0;
}

int main() {

	FILE* f = fopen("out.bin", "r");

	// Init
	stack stacks[8];
	for (int i = 0; i < 8; i++) {
		initStack(&stacks[i]);
	}
	disk* diskregister = NULL;
	if (prepareProgramBook(f, stacks)) {
		return 1;
	}
	
	while (true) {
		if (execute(stacks, diskregister)) {
			break;
		}
	}


	printf("Success!");
	


	// Clean up	
	fclose(f);
	for (int i = 0; i > 8; i++) {
		freeStack(&stacks[i]);
	}
	return 0;
}
