#include "disk.h"
#include "diskstack.h"

Disk::Disk() {
	literal = false;
}

Disk::Disk(string value) {
	this->value = value;
	literal = true;
}

Disk::~Disk() {
	delete stack;
}

DiskStack* Disk::getStack() { return stack; }

void Disk::reset() {
	delete stack;
	stack = NULL;
	value = "";
	literal = false;
}

//Operator overloads
Disk& Disk::operator=(const Disk &other) {
	if (this == &other) {
		return *this;
	}
	this->stack = other.stack;
	this->value = other.value;
	return *this;
}

bool Disk::operator==(const Disk &other) const {
	return (this->stack == other.stack) && (this->value == other.value);
}

