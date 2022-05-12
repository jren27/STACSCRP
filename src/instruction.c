#include "instruction.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void pushLine(linelist* list, line* line) {
	line->next = NULL;
	if (list->head == NULL) {
		line->prev = NULL;
		list->head = line;
		list->tail = list->head;
		return;
	}
	line->prev = list->tail;
	list->tail->next = line;
	list->tail = list->tail->next;
}

// Push to the back
void pushInstruction(instructionlist* list, instruction* inst) {
	inst->next = NULL;
	if (list->head == NULL) {
		inst->prev = NULL;
		list->head = inst;
		list->tail = list->head;
		return;
	}
	inst->prev = list->tail;
	list->tail->next = inst;
	list->tail = list->tail->next;
}

// Removes line l from linelist (l MUST BE IN list, BE VERY CAREFUL!)
void removeInstruction(instructionlist* list, instruction* inst) {
	if (inst == NULL) {
		return;
	}
	if (inst->next == NULL && inst->prev == NULL) {
		free(list->head);
		list->head = list->tail = NULL;
		return;
	}
	if (inst->next == NULL) {
		list->tail = list->tail->prev;
		free(list->tail);
		return;
	}
	if (inst->prev == NULL) {
		list->head = list->head->next;
		free(list->head);
		return;
	}
	inst->prev->next = inst->next;
	inst->next->prev = inst->prev;
	free(inst);
}

void freeInstructionList(instructionlist* list) {
	instruction* currinst = list->head;
	instruction* nextinst = currinst;
	
	while (currinst != NULL) {
		nextinst = nextinst->next;
		free(currinst);
		currinst = nextinst;
	}
}

