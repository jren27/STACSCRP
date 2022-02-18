#include "disk.h"

Disk::Disk(string identifier) {
	this->identifier = identifier;
	this->literal = false;
}

//Operator overloads
Disk& Disk::operator=(Disk& other) {
	if (this == &other) {
		return *this;
	}
	this->identifier = other.identifier;
	this->stack = other.stack;
	return *this;
}

bool Disk::operator==(Disk* other) {
	return (this->identifier == other->identifier && this->stack == other.stack);
}

LiteralDisk::LiteralDisk() {
	this->identifier = "LTRL";
	this->stack = NULL;
	this->isLiteral = true;
}

//Operator overloads
LiteralDisk& LiteralDisk::operator=(LiteralDisk& other) {
	if (this == &other) {
		return *this;
	}
	this->value = other->value;
	return *this;
}
bool LiteralDisk::operator==(LiteralDisk* other) {
	return (this->value = other->value);
}
