#include "disk.h"
#include "diskstack.h"

Disk::Disk() {
	this->identifier = "";
	this->literal = false;
}

Disk::Disk(string identifier) {
	this->identifier = identifier;
	this->literal = (identifier == "LTRL");
	if (this->literal) {
		stack = new DiskStack();
	}
}

Disk::Disk(string identifier, string value) {
	this->identifier = identifier;
	this->literal = true;
}

DiskStack* Disk::getStack() { return this->stack; }

//Operator overloads
Disk& Disk::operator=(const Disk &other) {
	if (this == &other) {
		return *this;
	}
	this->identifier = other.identifier;
	this->stack = other.stack;
	this->value = other.value;
	return *this;
}

bool Disk::operator==(const Disk &other) const {
	//lmao what
	return (this->identifier == other.identifier)
		&& (this->stack == other.stack)
		&& (this->value == other.value);
}

