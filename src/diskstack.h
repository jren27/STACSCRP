// diskstack.h
// Contains struct and function declarations for stack and disk

#ifndef DISK_STACK
#define DISK_STACK
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct disk {
	uint8_t type; // Stores binary data from stacstrm
	bool boolvalue;
	char charvalue;
	int intvalue; // Can also store bool and char values
	double doublevalue;
	bool isInstruction;
	unsigned short instruction;
} disk;

typedef struct stack {
	disk* contents;
	unsigned int size;
	unsigned int capacity;
} stack;

// --- STACK FUNCTIONS ---
void initStack(stack* s);
void freeStack(stack* s);
void push(stack* s, disk d);
int pop(stack* s);
int top(stack* s, disk* d);
int topDirect(stack* s, disk** d); // The same as top, but points directly to the top of s 
int swap(stack* s);

// --- DISK FUNCTIONS ---
void initDisk(disk* d);
void loadInstruction(disk* d, FILE* stream);

#endif
