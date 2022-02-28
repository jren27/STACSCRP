#include "disk.h"
#include "diskstack.h"
#include <iostream> // DEBUG!!!!

Disk::Disk(double value, bool literal) {
	this->value = value;
	this->literal = literal;
	if (!this->literal) {
		stack = new DiskStack();
	}
}

Disk::~Disk() {
	delete stack;
}

DiskStack* Disk::getStack() { return stack; }

void Disk::reset() {
	delete stack;
	stack = NULL;
	value = 0;
	literal = false;
}

//Operator overloads
Disk& Disk::operator=(const Disk &other) {
	if (this == &other) {
		return *this;
	}
	delete stack;
	if (other.stack == NULL) {
		stack = NULL;
	} else {
		this->stack = new DiskStack(*other.stack);
	}
	this->value = other.value;
	this->literal = other.literal;
	return *this;
}

bool Disk::operator==(const Disk &other) const {
	return (this->stack == other.stack) && (this->value == other.value);
}

