#include "diskstack.h"
#include <stdlib.h>


datatype convertToType(unsigned short i) {
	switch (i) {
		case 0:
			return NONLIT;
		case 1:
			return INT;
		case 2:
			return CHAR;
		case 3:
			return BOOL;
		case 4:
			return DOUBLE;
	}
	return NONLIT;
}

char convertToChar(datatype t) {
	switch (t) {
		case INT:
			return '#';
		case CHAR:
			return '\'';
		case DOUBLE:
			return '.';
		case BOOL:
			return '=';
		default:
			return '*';
	}
}

// --- STACK FUNCTIONS ---

void initStack(stack* s) {
	s->contents = (disk**)malloc(sizeof(disk*) * 4);
	s->size = 0;
	s->capacity = 4;
}

void freeStack(stack* s) {
	while(s->size != 0) {
		pop(s);
	}
	free(s->contents);
}

void push(stack* s, disk* d) {
	if (s->size == s->capacity) {
		s->capacity *= 2;
		disk** newcontents = (disk**)realloc(s->contents, sizeof(disk*) * s->capacity);
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
	freeDisk(s->contents[s->size-1]);
	s->size--;
	return 0;
}

// Returns 1 if stack is empty, 0 if success
int top(stack* s, disk* d) {
	if (s->size == 0) {
		return 1;
	}
	d = s->contents[s->size - 1];
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

int drop(stack* s) { // TODO write these
	
	return 0;
}

int grab(stack* s) {
	return 0;
}

// --- DISK FUNCTIONS ---

void initDisk(disk* d, bool isLiteral) {
	d->literal = isLiteral;
	if (!isLiteral) {
		initStack(&(d->internalstack));
	} else {
		// Set default value to int 0 (is this already set?)
		d->type = INT;
		d->intvalue = 0;
		d->doublevalue = 0;
	}
}

void freeDisk(disk* d) {
	if (!d->literal) {
		freeStack(&(d->internalstack));
	}
	free(d);
}
