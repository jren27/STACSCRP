#ifndef STACVIRT
#define STACVIRT

#include "diskstack.h"
#include "disk.h"
#include <vector>
#include <string>

struct Operation {
	int stack;
	string codeword;
	vector<string> arguments;
};

// The virtual machine that STACSCRP runs on
class StacVirt {
	private:
		DiskStack* stacks;
		// temp is declared here to save time reallocating/freeing
		Disk grabbedDisk, temp;
		unsigned int lineNum;
		bool stop;
		vector<Operation> storedOps;

		Operation parseToOperation(vector<string> arguments);
		bool checkArguments(vector<string> arguments, int argCount);
		bool isTopLiteral(int stack);
		bool getTopTwoLiteral(int stack, double &top, double &secondTop);
		void replace(int stack);

	public:
		StacVirt();
		void appendLine(string &line);
		bool isStopped() { return stop; }
		unsigned int getLine() { return lineNum; }
		void executeLine();
};

#endif
