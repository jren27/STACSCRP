#ifndef DISKSTACK
#define DISKSTACK

#include <exception>
#include "disk.h"

class Disk;


class DiskStack {
	private:
		Disk* stack;
		int size;
		int capacity;
	public:
		DiskStack();
		DiskStack(DiskStack& ds);
		virtual ~DiskStack();
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
