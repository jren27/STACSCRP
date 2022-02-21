#ifndef DISKSTACK
#define DISKSTACK

#include "disk.h"

class Disk;


class DiskStack {
	private:
		Disk* stack;
		int size;
		int capacity;
	public:
		DiskStack();
		~DiskStack() { delete[] this->stack; }
		void push(Disk &d);
		Disk pop();
		Disk top();
		void swap();
		int getSize() { return this->size; }

		//Operator overloads
		DiskStack& operator=(DiskStack &other);
		bool operator==(const DiskStack &other) const;
};

#endif
