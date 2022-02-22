#include "stacvirt.h"
#include <iostream>
#include <sstream>


bool StacVirt::checkArguments(vector<string> arguments, int argCount) {
	if (arguments.size() != argCount + 1) {
		cout << "ERROR: line " << line << "\n";
		cout << "Expected " << argCount << " arguments, got " << arguments.size() << " instead\n";
		this->stop = true;
	}
	return !this->stop;
}

StacVirt::StacVirt() {
	this->stacks = new DiskStack[8];
	this->line = 0;
	this->stop = false;
}

void StacVirt::executeLine(string &line) {
	//Parse line
	stringstream linestream;
	linestream.str(line);
	vector<string> arguments;
	string argument;
	std::size_t prev = 0, pos;
	while ((pos = line.find_first_of(" \t", prev)) != std::string::npos) {
		if (pos > prev) {
			arguments.push_back(line.substr(prev, pos-prev));
		}
		prev = pos+1;
	}
	if (prev < line.length()) {
		arguments.push_back(line.substr(prev, std::string::npos));
	}
	/*
	while (linestream.str().size() != 0) {
		cout << linestream.str() << endl;
		getline(linestream, argument, '\t');
		op.arguments.push_back(argument);
	}
	*/

	//TODO finish this bigass if statement
	if (arguments[0] == "new" && checkArguments(arguments, 1)) {
		this->grabbedDisk = Disk(arguments[1]);
	} else if (arguments[0] == "lnew" && checkArguments(arguments, 1)) {
		this->grabbedDisk = Disk("LTRL", arguments[1]);
	}

	this->line++;
}
