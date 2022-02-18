#include "diskstack.h"

DiskStack::DiskStack() {
	stack = new Disk[4];
	size = 0;
	capacity = 4;
}

void DiskStack::push(Disk* d) {
	if (this->size = this->capacity) {
		//Reallocate stack
		this->capacity *= 2;
		Disk* expandStack = new Disk[this->capacity];
		for (int i = 0; i < this->size; i++) {
			expandStack[i] = this->stack[i];
		}
		delete[] this->stack;
		this->stack = expandStack;
	}
	this->stack[this->size] = d;
	this->size++;
}

Disk* DiskStack::DiskStack::pop() {
	if (size == 0) {
		//Throw something
	}
	Disk* out = this->stack[this->size];
	size--;
	return out;
}

Disk* DiskStack::top() {
	if (size == 0) {
		//Throw something
	}
	return this->stack[this->size];
}

void DiskStack::swap() {
	if (size == 0) {
		//Throw something
	} else if (size == 1) {
		return; //Just keep the stack the same
	}
	Disk* temp = this->stack[this->size];
	this->stack[this->size] = this->stack[this->size - 1];
	this->stack[this->size-1] = temp;
}

//Operator overloads
DiskStack& DiskStack::operator=(DiskStack& other) {
	if (this == &other) {
		return *this;
	}
	delete[] this->stack;
	this->stack = new Disk[other.capacity];
	this->capacity = other.capacity;
	this->size = 0;
	for (int i = 0; i < other.capacity; i++) {
		this->stack[i] = other.stack[i]
	}
}

bool DiskStack::operator==(DiskStack& other) {
	if (this->size != other.size) {
		return false;
	}
	for (int i = 0; i < this.capacity; i++) {
		if (this->stack[i] == other.stack[i]) {
			return false;
		}
	}
	return true;
}

