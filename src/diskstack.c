#include <stdlib.h>
#include "diskstack.h"
#include "instruction.h"

// --- STACK FUNCTIONS ---

void initStack(stack* s) {
	s->contents = malloc(sizeof(disk) * 4);
	s->size = 0;
	s->capacity = 4;
}

void freeStack(stack* s) {
	free(s->contents);
}

void push(stack* s, disk d) {
	if (s->size == s->capacity) {
		s->capacity *= 2;
		disk* newcontents = (disk*)realloc(s->contents, sizeof(disk) * s->capacity);
		s->contents = newcontents;
	}
	s->contents[s->size] = d;
	s->size++;
}

// Returns 1 if stack is empty, 0 if success
int pop(stack* s) {
	if (s->size == 0) {
		return 1;
	}
	s->size--;
	return 0;
}

// Returns 1 if stack is empty, 0 if success
int top(stack* s, disk* d) {
	if (s->size == 0) {
		return 1;
	}
	*d = s->contents[s->size - 1];
	return 0;
}

// Returns 1 if stack is empty, 0 if no error
// (not success because swap doesn't do anything for size 1 stack
int swap(stack* s) {
	if (s->size == 0) {
		return 1;
	}
	if (s->size != 1) {
		// If size == 1, do nothing to the stack
		disk temp = s->contents[s->size - 2];
		s->contents[s->size - 2] = s->contents[s->size - 1];
		s->contents[s->size - 1] = temp;
	}
	return 0;
}

// --- DISK FUNCTIONS ---

void initDisk(disk* d) {
	// Set default value to int 0 (is this already set?)
	d->type = INT_TP;
	d->intvalue = 0;
	d->doublevalue = 0;
	d->isInstruction = false;
	d->instruction = '\0';
}

// Instruction format: stack | instruction | arg datatype | arg value (if not NULL)
void loadInstruction(disk* d, FILE* stream) {
	initDisk(d);
	d->isInstruction = true;
	fread(&d->instruction, sizeof(short), 1, stream);
	d->type = fgetc(stream); // Datatype
	switch (d->type) {
		case NULL_TP:
			return;
		case INT_TP:
			fread(&(d->intvalue), sizeof(int), 1, stream);
			break;
		case CHAR_TP:
			fread(&(d->intvalue), sizeof(char), 1, stream);
			break;
		case DOUB_TP:
			fread(&(d->doublevalue), sizeof(double), 1, stream);
			break;
	}
}
