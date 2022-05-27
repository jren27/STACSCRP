#include "instruction.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void pushLine(linelist* list, line* line, bool front) {
	line->next = NULL;
	line->prev = NULL;
	if (list->head == NULL) {
		line->prev = NULL;
		list->head = line;
		list->tail = list->head;
		return;
	}
	if (front) {
		line->next = list->head;
		list->head->prev = line;
		list->head = list->head->prev;
		return;
	}
	line->prev = list->tail;
	list->tail->next = line;
	list->tail = list->tail->next;
}

void freeLine(line* l) {
	if (l == NULL) {
		return;
	}
	for (int i = 0; i < 4; i++) {
		free(l->unparsed[i]);
	}
	free(l);
}

// Removes line l from linelist (l MUST BE IN list, BE VERY CAREFUL!)
// Returns previous line to make traversal easier
line* removeLine(linelist* list, line* l) {
	if (l == NULL) {
		return NULL;
	}
	if (l->next == NULL && l->prev == NULL) {
		freeLine(list->head);
		list->head = list->tail = NULL;
		return NULL;
	}
	if (l->next == NULL) {
		list->tail = list->tail->prev;
		freeLine(list->tail->next);
		list->tail->next = NULL;
		return list->tail;
	}
	if (l->prev == NULL) {
		list->head = list->head->next;
		freeLine(list->head->prev);
		list->head->prev = NULL;
		return list->head;
	}
	line* temp = l->prev;
	temp->next = l->next;
	l->next->prev = temp;
	freeLine(l);
	return temp;
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

void freeLineList(linelist* list) {
	line* currline = list->head;
	line* templine;
	if (currline == NULL) {
		return;
	}
	while (currline != NULL) {
		templine = currline->next;
		freeLine(currline);
		currline = templine;
	}
}

void freeInstruction(instruction* i) {
	if (i == NULL) {
		return;
	}
	free(i->d);
	free(i);
}

// Removes line l from linelist (l MUST BE IN list, BE VERY CAREFUL!)
void removeInstruction(instructionlist* list, instruction* inst) {
	if (inst == NULL) {
		return;
	}
	if (inst->next == NULL && inst->prev == NULL) {
		freeInstruction(list->head);
		list->head = list->tail = NULL;
		return;
	}
	if (inst->next == NULL) {
		list->tail = list->tail->prev;
		freeInstruction(list->tail);
		return;
	}
	if (inst->prev == NULL) {
		list->head = list->head->next;
		freeInstruction(list->head);
		return;
	}
	inst->prev->next = inst->next;
	inst->next->prev = inst->prev;
	freeInstruction(inst);
}

void freeInstructionList(instructionlist* list) {
	instruction* currinst = list->head;
	instruction* nextinst = currinst;
	
	while (currinst != NULL) {
		nextinst = nextinst->next;
		freeInstruction(currinst);
		currinst = nextinst;
	}
}

