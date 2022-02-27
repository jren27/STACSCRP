#ifndef DISK
#define DISK
#include <string>

class DiskStack;


using namespace std;

class Disk {
	private:
		DiskStack* stack = NULL;
		string value = "";
		bool literal = false;
	public:
		Disk(string value = "NULL", bool literal = true);
		virtual ~Disk();
		DiskStack* getStack();
		bool isLiteral() { return literal; }
		string getValue() { return value; }
		void setValue(string value) { this->value = value; }
		void reset();

		//Operator overloads
		Disk& operator=(const Disk &other);
		bool operator==(const Disk &other) const;
};

#endif
