#include <stdexcept>
#include "diskstack.h"
#include <iostream> //DEBUG!

DiskStack::DiskStack() {
	stack = new Disk[4];
	size = 0;
	capacity = 4;
}

DiskStack::DiskStack(DiskStack& ds) {	
	stack = new Disk[ds.capacity];
	this->capacity = ds.capacity;
	this->size = ds.size;
	for (int i = 0; i < ds.capacity; i++) {
		this->stack[i] = ds.stack[i];
	}
}

DiskStack::~DiskStack() {
	delete[] stack;
}

void DiskStack::push(Disk &d) {
	if (size == capacity) {
		//Reallocate stack
		capacity *= 2;
		Disk* expandStack = new Disk[capacity];
		for (int i = 0; i < size; i++) {
			expandStack[i] = stack[i];
		}
		delete[] stack;
		stack = expandStack;
	}
	stack[size] = d;
	size++;
}

Disk DiskStack::DiskStack::pop() {
	if (size == 0) {
		throw runtime_error("Stack size is not 0");
	}
	//stack[size].reset();
	size--;
	return stack[size];
}

Disk DiskStack::top() {
	if (size == 0) {
		//Throw something
	}
	return stack[size-1];
}

void DiskStack::swap() {
	if (size == 0) {
		//Throw something
	} else if (size == 1) {
		return; //Just keep the stack the same
	}
	Disk temp = stack[size];
	stack[size] = stack[size - 1];
	stack[size-1] = temp;
}

//Operator overloads
DiskStack& DiskStack::operator=(DiskStack &other) {
	if (this == &other) {
		return *this;
	}
	delete[] stack;
	stack = new Disk[other.capacity];
	this->capacity = other.capacity;
	this->size = other.size;
	for (int i = 0; i < other.capacity; i++) {
		this->stack[i] = other.stack[i];
	}
	return *this;
}

bool DiskStack::operator==(const DiskStack &other) const {
	if (this->size != other.size) {
		return false;
	}
	for (int i = 0; i < capacity; i++) {
		if (!(this->stack[i] == other.stack[i])) {
			return false;
		}
	}
	return true;
}

