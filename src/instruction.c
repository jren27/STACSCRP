#include "instruction.h"
#include <string.h>
#include <stdlib.h>

// Push to the back
void pushLine(linelist* list, char* buffer) {
	line* newline = malloc(sizeof(line));
	strncpy(newline->buff, buffer, 80);
	newline->next = NULL;
	if (list->head == NULL) {
		newline->prev = NULL;
		list->head = newline;
		list->tail = list->head;
		return;
	}
	newline->prev = list->tail;
	list->tail->next = newline;
	list->tail = list->tail->next;
}

// Removes line l from linelist (l MUST BE IN list, BE VERY CAREFUL!)
void removeLine(linelist* list, line* l) {
	if (l == NULL) {
		return;
	}
	if (l->next == NULL && l->prev == NULL) {
		free(list->head);
		list->head = list->tail = NULL;
		return;
	}
	if (l->next == NULL) {
		list->tail = list->tail->prev;
		free(list->tail);
		return;
	}
	if (l->prev == NULL) {
		list->head = list->head->next;
		free(list->head);
		return;
	}
	l->prev->next = l->next;
	l->next->prev = l->prev;
	free(l);
}
