#ifndef STACVIRT
#define STACVIRT

#include "diskstack.h"
#include "disk.h"
#include <vector>
#include <string>

struct Operation {
	string codeword;
	vector<string> arguments;
};

// The virtual machine that STACSCRP runs on
class StacVirt {
	private:
		DiskStack* stacks;
		Disk grabbedDisk;
		unsigned int line;
		bool stop;
		bool checkArguments(vector<string> arguments, int argCount);

	public:
		StacVirt();
		bool isStopped() { return this->stop; }
		void executeLine(string &line);
};

#endif
