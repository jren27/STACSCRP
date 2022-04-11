// diskstack.h
// Contains struct and function declarations for stack and disk
// Also contains datatype, an enum for the type of data a disk is storing and relevant functions

#ifndef DISK_STACK
#define DISK_STACK
#include <stdbool.h>

struct disk;

typedef enum datatype {
	NONLIT, INT, CHAR, BOOL, DOUBLE
} datatype;

typedef struct stack {
	struct disk** contents;
	unsigned int size;
	unsigned int capacity;
} stack;

typedef struct disk {
	datatype type;
	int intvalue; // Can also store bool and char values
	double doublevalue;
	stack internalstack;
	bool literal;
} disk;

// --- DATATYPE FUNCTIONS ---
datatype convertToType(unsigned short i);
char convertToChar(datatype t);

// --- STACK FUNCTIONS ---
void initStack(stack* s);
void freeStack(stack* s);
void push(stack* s, disk* d);
int pop(stack* s);
int top(stack* s, disk* d);

// --- DISK FUNCTIONS ---
void initDisk(disk* d, bool isLiteral);
void freeDisk(disk* d);

#endif
