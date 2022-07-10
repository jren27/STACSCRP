#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "src/diskstack.h"
#include "src/instruction.h"

// Sometimes things happen too fast, this slows output down for easier debugging
#define SLOWPRINT 0

int prepareProgramBook(FILE* f, stack* stacks) {
	// Read file to PB2
	disk* d;
	while (!feof(f)) {
		d = malloc(sizeof(disk));
		loadInstruction(d, f);
		push(&stacks[6], *d);
		free(d);
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
	push(&stacks[7], *d);
	return 0;
}

int seekProgramBook(stack* stacks, int i) {
	if (i < 0) {
		printf("RUNTIME ERROR: Attemped to seek to a negative-indexed instruction\n");
		return 1;
	}
	disk temp;
	while (i != stacks[5].size) {

		// Flip PB1 -> PB2
		if (i < stacks[5].size) {
		 	top(&stacks[5], &temp);
			pop(&stacks[5]);
			push(&stacks[6], temp);
			continue;
		}
		// Flip PB2 -> PB1
		if (top(&stacks[6], &temp)) {
			printf("RUNTIME ERROR: Instruction seek went out of range");
			return 1;
		}
		pop(&stacks[6]);
		push(&stacks[5], temp);
	}
	return 0;
}

// Arg 2 stays on top, update it with result
int get2Args(stack* stacks, int currstack, disk* arg1, disk** arg2) {
	if (top(&stacks[currstack], arg1)) {
		printf("RUNTIME ERROR: Stack is empty\n");
		return 1;
	}
	pop(&stacks[currstack]);
	// Different than top(), need to point arg2 DIRECTLY to top, not create a copy
	if (topDirect(&stacks[currstack], arg2)) {
		printf("RUNTIME ERROR: Stack only contains one argument\n");
		return 1;
	}
	return 0;
}

int execute(stack* stacks, disk* diskregister) {
	disk currdisk;
	if (top(&stacks[6], &currdisk)) {
		printf("RUNTIME ERROR: Reached EOF");
		return 1;
	}
	if (currdisk.instruction == EXEC_OP) {
		pop(&stacks[6]);
		return 0; // No need to update system stack
	}
	short currstack = currdisk.instruction >> 8;
	short currinst = currdisk.instruction & 0x00FF;
	bool increment = true;

	// For arth/comp stuff
	disk arg1, *arg2;
	disk newdisk;
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
			newdisk = currdisk;
			newdisk.isInstruction = false;
			push(&stacks[currstack], newdisk);
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
		case SIZE_OP:
			initDisk(&newdisk);
			newdisk.intvalue = stacks[currstack].size;
			push(&stacks[currstack], *diskregister);
			break;
		case ARTH_OP | ADD_AL: // --- ARITHMETIC INSTRUCTIONS ---
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			// oh boy
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to add two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					printf("RUNTIME ERROR: Cannot add two booleans\n");
					return 1;
				case CHAR_TP:
					arg2->charvalue += arg1.charvalue;
					break;
				case INT_TP:
					arg2->intvalue += arg1.intvalue;
					break;
				case DOUB_TP:
					arg2->doublevalue += arg1.doublevalue;
			}
			break;
		case ARTH_OP | SUB_AL:
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to subtract two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					printf("RUNTIME ERROR: Cannot subtract two booleans\n");
					return 1;
				case CHAR_TP:
					arg2->charvalue = arg1.charvalue - arg2->charvalue;
					break;
				case INT_TP:
					arg2->intvalue = arg1.intvalue - arg2->intvalue;
					break;
				case DOUB_TP:
					arg2->doublevalue = arg1.doublevalue - arg2->doublevalue;
			}
			break;
		case ARTH_OP | MUL_AL:
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to multiply two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					printf("RUNTIME ERROR: Cannot multiply two booleans\n");
					return 1;
				case CHAR_TP:
					arg2->charvalue *= arg1.charvalue;
					break;
				case INT_TP:
					arg2->intvalue *= arg1.intvalue;
					break;
				case DOUB_TP:
					arg2->doublevalue *= arg1.doublevalue;
			}
			break;
		case ARTH_OP | DIV_AL:
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to divide two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					printf("RUNTIME ERROR: Cannot divide two booleans\n");
					return 1;
				case CHAR_TP:
					arg2->charvalue /= arg1.charvalue;
					break;
				case INT_TP:
					arg2->intvalue /= arg1.intvalue;
					break;
				case DOUB_TP:
					arg2->doublevalue /= arg1.doublevalue;
			}
			break;
		case ARTH_OP | AND_AL:
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to AND two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					arg2->boolvalue = arg2->boolvalue && arg1.boolvalue;
					break;
				case CHAR_TP:
					arg2->charvalue &= arg1.charvalue;
					break;
				case INT_TP:
					arg2->intvalue &= arg1.intvalue;
					break;
				case DOUB_TP:
					printf("RUNTIME ERROR: Cannot AND two doubles\n");
					return 1;
			}
			break;
		case ARTH_OP | LOR_AL:
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to OR two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					arg2->boolvalue = arg2->boolvalue || arg1.boolvalue;
					break;
				case CHAR_TP:
					arg2->charvalue |= arg1.charvalue;
					break;
				case INT_TP:
					arg2->intvalue |= arg1.intvalue;
					break;
				case DOUB_TP:
					printf("RUNTIME ERROR: Cannot OR two doubles\n");
					return 1;
			}
			break;
		case ARTH_OP | NOT_AL:
			if (top(&stacks[currstack], &arg1)) {
				printf("RUNTIME ERROR: Stack is empty\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					arg1.boolvalue = !arg1.boolvalue;
					break;
				case CHAR_TP:
					arg1.charvalue = ~arg1.charvalue;
					break;
				case INT_TP:
					arg1.intvalue = ~arg1.intvalue;
					break;
				case DOUB_TP:
					printf("RUNTIME ERROR: Cannot NOT a double\n");
					return 1;
			}
			break;
		case ARTH_OP | XOR_AL:
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to XOR two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					arg2->boolvalue = arg2->boolvalue != arg1.boolvalue; // Apparently they do this in code golf
					break;
				case CHAR_TP:
					arg2->charvalue ^= arg1.charvalue;
					break;
				case INT_TP:
					arg2->intvalue ^= arg1.intvalue;
					break;
				case DOUB_TP:
					printf("RUNTIME ERROR: Cannot XOR two doubles\n");
					return 1;
			}
			break;
		case COMP_OP | EQ: // --- COMPARE INSTRUCTIONS ---
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to compare two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					arg2->boolvalue = arg1.boolvalue == arg2->boolvalue;
					break;
				case CHAR_TP:
					arg2->boolvalue = arg1.boolvalue == arg2->charvalue;
					break;
				case INT_TP:
					arg2->boolvalue = arg1.intvalue == arg2->intvalue;
					break;
				case DOUB_TP:
					arg2->boolvalue = arg1.intvalue == arg2->intvalue;
			}
			arg2->type = BOOL_TP;
			break;
		case COMP_OP | NE:
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to compare two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					arg2->boolvalue = arg1.boolvalue != arg2->boolvalue;
					break;
				case CHAR_TP:
					arg2->boolvalue = arg1.boolvalue != arg2->charvalue;
					break;
				case INT_TP:
					arg2->boolvalue = arg1.intvalue != arg2->intvalue;
					break;
				case DOUB_TP:
					arg2->boolvalue = arg1.intvalue != arg2->intvalue;
			}
			arg2->type = BOOL_TP;
			break;
		case COMP_OP | GT:
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to compare two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					arg2->boolvalue = arg1.boolvalue > arg2->boolvalue;
					break;
				case CHAR_TP:
					arg2->boolvalue = arg1.boolvalue > arg2->charvalue;
					break;
				case INT_TP:
					arg2->boolvalue = arg1.intvalue > arg2->intvalue;
					break;
				case DOUB_TP:
					arg2->boolvalue = arg1.intvalue > arg2->intvalue;
			}
			arg2->type = BOOL_TP;
			break;
		case COMP_OP | LE:
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to compare two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					arg2->boolvalue = arg1.boolvalue <= arg2->boolvalue;
					break;
				case CHAR_TP:
					arg2->boolvalue = arg1.boolvalue <= arg2->charvalue;
					break;
				case INT_TP:
					arg2->boolvalue = arg1.intvalue <= arg2->intvalue;
					break;
				case DOUB_TP:
					arg2->boolvalue = arg1.intvalue <= arg2->intvalue;
			}
			arg2->type = BOOL_TP;
			break;
		case COMP_OP | LT:
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to compare two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					arg2->boolvalue = arg1.boolvalue < arg2->boolvalue;
					break;
				case CHAR_TP:
					arg2->boolvalue = arg1.boolvalue < arg2->charvalue;
					break;
				case INT_TP:
					arg2->boolvalue = arg1.intvalue < arg2->intvalue;
					break;
				case DOUB_TP:
					arg2->boolvalue = arg1.intvalue < arg2->intvalue;
			}
			arg2->type = BOOL_TP;
			break;
		case COMP_OP | GE:
			if (get2Args(stacks, currstack, &arg1, &arg2)) {
				return 1;
			}
			if (arg1.type != arg2->type) {
				printf("RUNTIME ERROR: Attempted to compare two disks with non-matching types\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					arg2->boolvalue = arg1.boolvalue >= arg2->boolvalue;
					break;
				case CHAR_TP:
					arg2->boolvalue = arg1.boolvalue >= arg2->charvalue;
					break;
				case INT_TP:
					arg2->boolvalue = arg1.intvalue >= arg2->intvalue;
					break;
				case DOUB_TP:
					arg2->boolvalue = arg1.intvalue >= arg2->intvalue;
			}
			arg2->type = BOOL_TP;
			break;
		case TYPE_OP:
			if (top(&stacks[currstack], &arg1)) {
				printf("RUNTIME ERROR: Stack is empty\n");
				return 1;
			}
			initDisk(&newdisk);
			newdisk.type = CHAR_TP;
			switch (arg1.type) {
				case BOOL_TP:
					newdisk.charvalue = '=';
					break;
				case CHAR_TP:
					newdisk.charvalue = '\'';
					break;
				case INT_TP:
					newdisk.charvalue = '#';
					break;
				case DOUB_TP:
					newdisk.charvalue = '.';
			}
			if (arg1.isInstruction) {
				newdisk.charvalue = '!';
			}
			push(&stacks[currstack], newdisk);
			break;
		case RTYP_OP: // TODO write retype() in seperate function (expecting 12 cases, too big!)
			break;
		case IPUT_OP:
			initDisk(&newdisk);
			printf(">>> ");
			switch (currdisk.charvalue) {
				case '=':
					scanf("%d", &newdisk.intvalue);
					newdisk.boolvalue = newdisk.intvalue;
					newdisk.type = BOOL_TP;
					break;
				case '\'':
					scanf("%c", &newdisk.charvalue);
					newdisk.type = CHAR_TP;
					break;
				case '#':
					scanf("%d", &newdisk.intvalue);
					newdisk.type = INT_TP;
					break;
				case '.':
					scanf("%lf", &newdisk.doublevalue);
					newdisk.type = DOUB_TP;
			}
			push(&stacks[currstack], newdisk);
			break;
		case OPUT_OP:	
			if (top(&stacks[currstack], &arg1)) {
				printf("RUNTIME ERROR: Stack is empty\n");
				return 1;
			}
			switch (arg1.type) {
				case BOOL_TP:
					printf("%d", arg1.boolvalue);
				break;
				case CHAR_TP:
					printf("%c", arg1.charvalue);
				break;
				case INT_TP:
					printf("%d", arg1.intvalue);
				break;
				case DOUB_TP:
					printf("%lf", arg1.doublevalue);
			}
			break;
		case GOTO_OP | COND_BR:
			if (top(&stacks[currstack], &arg1)) {
				printf("RUNTIME ERROR: Stack is empty\n");
				return 1;
			}
			if (arg1.type != BOOL_TP) {
				printf("RUNTIME ERROR: Attempted to use a non-boolean for a GOIF conditional\n");
				return 1;
			}
			if (arg1.boolvalue == 0) { // Continue to GOTO if != 0
				break;
			}
		case GOTO_OP:
			topDirect(&stacks[7], &arg2);
			if (currdisk.type != INT_TP) {
				printf("RUNTIME ERROR: Cannot store non-integers to system stack\n");
				return 1;
			}
			arg2->intvalue = currdisk.intvalue;
			increment = false;
			break;
		case JUMP_OP | COND_BR:
			if (top(&stacks[currstack], &arg1)) {
				printf("RUNTIME ERROR: Stack is empty\n");
				return 1;
			}
			if (arg1.type != BOOL_TP) {
				printf("RUNTIME ERROR: Attempted to use a non-boolean for a JPIF conditional\n");
				return 1;
			}
			if (arg1.boolvalue == 0) { // Continue to JUMP if != 0
				break;
			}
		case JUMP_OP:
			initDisk(&newdisk);
			if (currdisk.type != INT_TP) {
				printf("RUNTIME ERROR: Cannot store non-integers to system stack\n");
				return 1;
			}
			newdisk.intvalue = currdisk.intvalue;
			push(&stacks[7], newdisk);
			//increment = false;
			break;
		default:
			printf("CRITICAL RUNTIME ERROR: Unknown instruction found\n");
			return 1;
	}
	
	if (stacks[7].size == 0) {
		return 1;
	}

	// Update system stack, seek to next instruction
	if (increment) {
		stacks[7].contents[stacks[7].size - 1].intvalue++;
	}
	seekProgramBook(stacks, stacks[7].contents[stacks[7].size - 1].intvalue);
#if SLOWPRINT
	usleep(100000);
#endif
	return 0;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: emulator [target]\n");
		return 1;
	}
	FILE* f = fopen(argv[1], "r");
	if (f == NULL) {
		printf("Target does not exist!\n");
		return 1;
	}
	// Init
	stack stacks[8];
	for (int i = 0; i < 8; i++) {
		initStack(&stacks[i]);
	}
	disk* diskregister = malloc(sizeof(disk));
	if (prepareProgramBook(f, stacks)) {
		return 1;
	}
	//printf("Successful load, running...\n");
	while (true) {
		if (execute(stacks, diskregister)) {
			break;
		}
	}

	// Clean up	
	fclose(f);
	for (int i = 0; i < 8; i++) {
		freeStack(&stacks[i]);
	}
	free(diskregister);
	return 0;
}
