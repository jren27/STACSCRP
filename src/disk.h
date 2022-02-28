#ifndef DISK
#define DISK
#include <string>

class DiskStack;


using namespace std;

class Disk {
	private:
		DiskStack* stack = NULL;
		double value; // Doubles can store ints, chars, bools
		bool literal = false;
	public:
		Disk(double value = 0, bool literal = true);
		virtual ~Disk();
		DiskStack* getStack();
		bool isLiteral() { return literal; }
		double getValue() { return value; }
		char getValueChar() { return (char)value; }
		void setValue(double value) { this->value = value; }
		void setValueChar(char value) { this->value = (double)value; }
		void reset();

		//Operator overloads
		Disk& operator=(const Disk &other);
		bool operator==(const Disk &other) const;
};

#endif
