#ifndef DISK
#define DISK
#include <string>
#include "diskstack.h"

using namespace std;

class Disk {
	private:
		string identifier;
		DiskStack stack;
		bool literal;
	public:
		Disk(string identifier);
		DiskStack getStack() { return this->stack; }
		string getIdentifier() { return this->identifier; }
		bool isLiteral() { return this->literal; }
		
		//Operator overloads
		Disk& operator=(Disk& other);
		bool operator==(Disk* other);
};

class LiteralDisk : public Disk {
	private:
		string value; // IE: #10, .3.14, 'y', etc.
	public:
		LiteralDisk();
		string getValue() { return this->value; }
		void setValue(string value) { this->value = value; }

		//Operator overloads
		LiteralDisk& operator=(LiteralDisk& other);
		bool operator==(LiteralDisk* other);
};

#endif
