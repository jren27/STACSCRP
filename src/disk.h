#ifndef DISK
#define DISK
#include <string>

class DiskStack;


using namespace std;

class Disk {
	private:
		string identifier;
		DiskStack* stack = NULL;
		string value = "";
		bool literal;
	public:
		Disk();
		Disk(string identifier);
		Disk(string identifier, string value);
		DiskStack* getStack();
		string getIdentifier() { return this->identifier; }
		bool isLiteral() { return this->literal; }
		string getValue() { return this->value; }
		void setValue(string value) { this->value = value; }

		//Operator overloads
		Disk& operator=(const Disk &other);
		bool operator==(const Disk &other) const;
};

#endif
